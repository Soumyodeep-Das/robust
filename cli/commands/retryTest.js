import fs from "fs-extra";
import path from "path";
import { callLLM } from "../utils/apiClient.js";
import { promptSourceFile } from "../utils/promptBuilder.js";
import ora from "ora";

export default async function retryTest() {
  // Prompt user for source file
  const srcFiles = await promptSourceFile(path.resolve("src"));
  if (!srcFiles.length) {
    console.error("❌ No source files selected.");
    return;
  }
  const srcPath = srcFiles[0];

  // Prompt user for test file
  const testFiles = await promptSourceFile(path.resolve("tests"));
  if (!testFiles.length) {
    console.error("❌ No test files selected.");
    return;
  }
  const testPath = testFiles[0];

  const logPath = path.join("logs", "build.log");
  if (!fs.existsSync(srcPath) || !fs.existsSync(logPath)) {
    console.error("❌ Required files missing. Ensure you have the selected source file and logs/build.log.");
    return;
  }

  const code = fs.readFileSync(srcPath, "utf-8");
  const logs = fs.readFileSync(logPath, "utf-8");

  const prompt = `
You previously generated a GoogleTest-based test file for the following C++ code:

\`\`\`cpp
${code}
\`\`\`

However, the build failed with the following error log:
\`\`\`
${logs}
\`\`\`

Please regenerate the test file \`${path.basename(testPath)}\` in a corrected way so that it compiles and passes. Avoid any test_main() if already in main.cpp.
Output only the test file content.
`;

  const spinner = ora('🔁 Retrying test generation using AI...').start();
  try {
    const improvedTest = await callLLM(prompt);
    fs.writeFileSync(testPath, improvedTest);
    spinner.succeed(`✅ Retried test written to: ${testPath}`);
  } catch (err) {
    spinner.fail('❌ Retry failed: ' + err.message);
  }
}

