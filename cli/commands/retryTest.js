import fs from "fs-extra";
import path from "path";
import { callLLM } from "../utils/apiClient.js";

export default async function retryTest() {
  const srcPath = path.join("src", "main.cpp");
  const testPath = path.join("tests", "test_main.cpp");
  const logPath = path.join("logs", "build.log");

  if (!fs.existsSync(srcPath) || !fs.existsSync(logPath)) {
    console.error("❌ Required files missing. Ensure you have `src/main.cpp` and `logs/build.log`.");
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

Please regenerate the test file \`test_main.cpp\` in a corrected way so that it compiles and passes. Avoid any test_main() if already in main.cpp.
Output only the test file content.
`;

  try {
    const improvedTest = await callLLM(prompt);
    fs.writeFileSync(testPath, improvedTest);
    console.log("✅ Retried test written to:", testPath);
  } catch (err) {
    console.error("❌ Retry failed:", err.message);
  }
}
