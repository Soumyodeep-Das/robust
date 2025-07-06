import fs from "fs-extra";
import path from "path";
import { callLLM } from "../utils/apiClient.js";
import { promptSourceFile } from "../utils/promptBuilder.js";

/**
 * Refine tests by sending failed build/test logs and source to LLM for suggestions/fixes.
 * Iteratively improves tests, removes duplicates, and fixes formatting.
 * TODO: Implement actual LLM refinement loop.
 */
export default async function refineTest() {
  // Prompt user for test file to refine
  const testFiles = await promptSourceFile(path.resolve("tests"));
  if (!testFiles.length) {
    console.error("❌ No test files selected.");
    return;
  }
  const testPath = testFiles[0];

  // Prompt user for source file to refine
  const srcFiles = await promptSourceFile(path.resolve("src"));
  if (!srcFiles.length) {
    console.error("❌ No source files selected.");
    return;
  }
  const srcPath = srcFiles[0];

  const buildLog = fs.readFileSync("logs/build.log", "utf-8");
  const testLog = fs.readFileSync("logs/test.log", "utf-8");

  // Load YAML instructions
  let yamlInstructions = '';
  const yamlPath = path.resolve("instructions.yaml");
  if (fs.existsSync(yamlPath)) {
    yamlInstructions = fs.readFileSync(yamlPath, "utf-8");
  } else {
    console.warn("⚠️ instructions.yaml not found. Proceeding without YAML instructions.");
  }

  // Load test and source file contents
  const testCode = fs.readFileSync(testPath, "utf-8");
  const srcCode = fs.readFileSync(srcPath, "utf-8");

  // Build the prompt for the LLM
  const prompt = `
You are an expert C++ developer and test engineer. You are given:
- The original C++ source file:
\n---SOURCE FILE---\n
${srcCode}

\n---END SOURCE FILE---\n
- The current test file (Google Test):
\n---TEST FILE---\n
${testCode}

\n---END TEST FILE---\n
- The build log (if any):
\n---BUILD LOG---\n
${buildLog}

\n---END BUILD LOG---\n
- The test log (if any):
\n---TEST LOG---\n
${testLog}

\n---END TEST LOG---\n
- YAML instructions for test generation and refinement:
\n---YAML INSTRUCTIONS---\n
${yamlInstructions}
\n---END YAML INSTRUCTIONS---\n

Your task:
1. Refine and improve the test file so that it compiles and passes all tests, following the YAML instructions strictly.
2. Remove any duplicate or trivial tests, improve formatting, and ensure all relevant edge cases are covered.
3. If there are build or test failures, fix the issues in the test file.
4. Output only the improved test file content (no explanations).
`;

  // Add ora spinner for LLM call
  const ora = (await import('ora')).default;
  const spinner = ora('🤖 Refining test using AI...').start();
  try {
    const improvedTest = await callLLM(prompt);
    fs.writeFileSync(testPath, improvedTest);
    spinner.succeed(`✅ Refined test written to: ${testPath}`);
  } catch (err) {
    spinner.fail('❌ Refinement failed: ' + err.message);
  }
}

