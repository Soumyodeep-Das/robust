import fs from "fs-extra";
import path from "path";
import ora from "ora";
import { fileURLToPath } from "url";
import { buildPrompt, promptSourceFile } from "../utils/promptBuilder.js";
import { callLLM } from "../utils/apiClient.js";

const __dirname = path.dirname(fileURLToPath(import.meta.url));

function detectLanguage(filename) {
  return filename.endsWith(".c") ? "c" : "cpp";
}

function extractYamlInstructions(language) {
  // Strict YAML instructions for LLM test generation
  const yaml = {
    language: language,
    strict: true,
    objective:
      "Given a C or C++ source file, perform the following:\n1. Extract all public functions/classes and place them into a proper .h/.hpp header file.\n2. Modify the source file to remove `main()` if present, and ensure function definitions remain valid.\n3. Generate a corresponding unit test file using the correct framework (Unity for C, GoogleTest for C++).\n4. Ensure tests include the generated header file, not the source file directly.",
    c: {
      framework: "unity",
      details: [
        "Use the Unity C unit testing framework for all C tests. Do NOT use assert.h.",
        "All test files must include #include \"unity/unity.h\" at the top, and #include the generated header file.",
        "Define empty setUp() and tearDown() functions: void setUp(void) {} void tearDown(void) {}.",
        "All assertions must use Unity macros (e.g., TEST_ASSERT_TRUE, TEST_ASSERT_FALSE, TEST_ASSERT_EQUAL_INT, etc).",
        "Register all test functions in a single main() using UNITY_BEGIN(), RUN_TEST(test_func), and UNITY_END().",
        "Do not define multiple main() functions or duplicate code.",
        "Example structure:\n\n#include \"unity/unity.h\"\n#include \"<header>.h\"\nvoid setUp(void) {}\nvoid tearDown(void) {}\nvoid test_func() { TEST_ASSERT_TRUE(...); }\nint main(void) { UNITY_BEGIN(); RUN_TEST(test_func); return UNITY_END(); }",
        "Do not use assert.h, printf, or any custom test runners.",
        "Ensure all required Unity functions and macros are used correctly for compatibility with the build system.",
        "If build fails due to missing setUp/tearDown or Unity macros, regenerate the test file to fix it."
      ]
    },
    cpp: {
      framework: "googletest",
      details: [
        "Use GoogleTest (gtest/gtest.h) for all C++ unit tests. Do NOT use assert.h or custom frameworks.",
        "Create <original_filename>.hpp with all public class/method declarations.",
        "Modify <original_filename>.cpp to only contain method definitions. Do not include main() in the source if generating tests.",
        "Generate test_<original_filename>.cpp using #include \"<original_filename>.hpp\".",
        "All test cases must use TEST or TEST_F macros from GoogleTest.",
        "Do not use assert.h, printf, or any custom test runners.",
        "Ensure the test file compiles and links with GoogleTest and the build system.",
        "If build fails due to missing includes or GoogleTest macros, regenerate the test file to fix it."
      ]
    },
    build_fixes: {
      if_fails: [
        "Use build logs to detect:\n    - missing includes\n    - multiple definitions of main\n    - linkage/type issues",
        "Fix includes, convert inline implementations to external, add missing headers.",
        "Rebuild and retry test generation after fixing."
      ],
      if_passes: [
        "Run gcov or lcov to collect test coverage.",
        "Send uncovered lines/functions back to LLM.",
        "Ask the LLM to refine the test file for higher coverage.",
        "Eliminate any redundant or duplicate tests."
      ]
    },
    output: {
      c: {
        generate: [
          "<original_filename>.h",
          "<original_filename>.c",
          "test_<original_filename>.c"
        ]
      },
      cpp: {
        generate: [
          "<original_filename>.hpp",
          "<original_filename>.cpp",
          "test_<original_filename>.cpp"
        ]
      }
    }
  };
  return yaml;
}

export default async function generateTest() {
  const selectedFiles = await promptSourceFile();

  for (const inputPath of selectedFiles) {
    if (!fs.existsSync(inputPath)) {
      console.error("❌ Source file not found:", inputPath);
      continue;
    }

    const filename = path.basename(inputPath);
    const sourceCode = fs.readFileSync(inputPath, "utf-8");

    const lang = detectLanguage(filename);
    const yamlObject = extractYamlInstructions(lang);
    const yamlBlock = `---\n${JSON.stringify(yamlObject, null, 2)}\n---`;

    const prompt = `Use the following YAML strict instruction to guide the header + test generation:\n<yaml>\n${yamlBlock}\n</yaml>\n\nSource File: ${filename}\n<code>\n${sourceCode}\n</code>`;

    let spinner;
    try {
      spinner = ora(`🧠 Generating test and header for ${filename}...`).start();
      const response = await callLLM(prompt);
      spinner.succeed(`✅ LLM response received for ${filename}`);
      fs.ensureDirSync("tests");
      fs.ensureDirSync("src");

      // Parse and save response (assumes response contains 3 blocks with file names)
      const fileBlocks = response.split(/(?=^\/\*\*\* File: )/m);

      for (const block of fileBlocks) {
        const match = block.match(/^\/\*\*\* File: (.*?) \*\*\*\/(?:\r?\n|\r)([\s\S]*)/);
        if (match) {
          const outputFile = match[1].trim();
          const content = match[2].trim();
          const dir = outputFile.startsWith("test_") ? "tests" : "src";
          const fullPath = path.join(dir, outputFile);
          const fileSpinner = ora(`💾 Writing ${fullPath}...`).start();
          try {
            fs.writeFileSync(fullPath, content);
            fileSpinner.succeed(`✅ Saved: ${fullPath}`);
          } catch (fileErr) {
            fileSpinner.fail(`❌ Failed to write: ${fullPath}`);
            console.error(fileErr.message);
          }
        }
      }
    } catch (err) {
      if (spinner) spinner.fail(`❌ Failed to generate files for ${filename}`);
      console.error(err.message);
    }
  }
}
