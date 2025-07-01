import fs from "fs-extra";
import path from "path";
import { callLLM } from "../utils/apiClient.js";

async function retryBuild() {
  const testFile = path.join("tests", "test_main.cpp");
  const logFile = path.join("logs", "build.log");

  if (!fs.existsSync(logFile)) {
    console.error("No logs found to retry.");
    return;
  }

  const logs = fs.readFileSync(logFile, "utf-8");
  const failedTest = fs.readFileSync(testFile, "utf-8");

  const retryPrompt = `
The following test failed to compile/run:

<test_file>
${failedTest}
</test_file>

Error logs:
<logs>
${logs}
</logs>

Fix the test file to be buildable and valid using GoogleTest. Output only fixed test_main.cpp content.
`;

  const regenerated = await callLLM(retryPrompt);
  fs.writeFileSync(testFile, regenerated);
  console.log("🔁 Regenerated test file based on build failure.");
}

export default retryBuild;
