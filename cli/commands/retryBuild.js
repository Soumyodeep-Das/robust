import fs from "fs-extra";
import path from "path";
import { callLLM } from "../utils/apiClient.js";
import ora from "ora";

async function retryBuild(testFilePath) {
  const logFile = path.join("logs", "build.log");

  if (!fs.existsSync(logFile)) {
    console.error("No logs found to retry.");
    return;
  }
  if (!fs.existsSync(testFilePath)) {
    console.error(`Test file not found: ${testFilePath}`);
    return;
  }

  const logs = fs.readFileSync(logFile, "utf-8");
  const failedTest = fs.readFileSync(testFilePath, "utf-8");

  const retryPrompt = `
The following test failed to compile/run:

<test_file>
${failedTest}
</test_file>

Error logs:
<logs>
${logs}
</logs>

Fix the test file to be buildable and valid using GoogleTest. Output only fixed test file content for ${path.basename(testFilePath)}.
`;

  const spinner = ora('🔁 Retrying test generation using AI...').start();
  try {
    const regenerated = await callLLM(retryPrompt);
    fs.writeFileSync(testFilePath, regenerated);
    spinner.succeed(`🔁 Regenerated test file based on build failure: ${testFilePath}`);
  } catch (err) {
    spinner.fail('❌ Retry failed: ' + err.message);
  }
}

export default retryBuild;
