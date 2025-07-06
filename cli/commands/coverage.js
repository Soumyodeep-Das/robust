import fs from "fs-extra";
import path from "path";
import { spawnSync } from "child_process";
import { callLLM } from "../utils/apiClient.js";

/**
 * Run gcov/lcov to collect coverage, parse uncovered lines, and prompt LLM to improve tests.
 */
export default async function coverage() {
  fs.ensureDirSync("coverage");

  // 1. Run lcov to collect coverage data
  const lcovCmd = "lcov --capture --directory . --output-file coverage/coverage.info";
  const genhtmlCmd = "genhtml coverage/coverage.info --output-directory coverage/html";
  const lcovResult = spawnSync(lcovCmd, { shell: true, encoding: "utf-8" });
  if (lcovResult.error) {
    console.error("❌ Failed to run lcov:", lcovResult.error);
    return;
  }
  if (lcovResult.status !== 0) {
    console.error("❌ lcov failed:", lcovResult.stderr || lcovResult.stdout);
    return;
  }
  console.log("✅ lcov coverage data collected.");

  // 2. Parse coverage.info to find uncovered lines/functions
  const coverageInfoPath = path.join("coverage", "coverage.info");
  if (!fs.existsSync(coverageInfoPath)) {
    console.error("❌ coverage.info not found. Did lcov run correctly?");
    return;
  }
  const coverageInfo = fs.readFileSync(coverageInfoPath, "utf-8");
  const uncovered = [];
  let currentFile = null;
  coverageInfo.split(/\r?\n/).forEach(line => {
    if (line.startsWith("SF:")) {
      currentFile = line.replace("SF:", "").trim();
    } else if (line.startsWith("DA:")) {
      const [lineNum, count] = line.replace("DA:", "").split(",");
      if (parseInt(count, 10) === 0 && currentFile) {
        uncovered.push({ file: currentFile, line: parseInt(lineNum, 10) });
      }
    }
  });

  if (uncovered.length === 0) {
    console.log("🎉 All lines are covered by tests!");
    return;
  }
  console.log(`🔎 Found ${uncovered.length} uncovered lines.`);

  // 3. Build prompt for LLM
  let prompt = `You are an expert C++ test engineer. The following lines in the codebase are not covered by tests:\n`;
  uncovered.forEach(u => {
    prompt += `- File: ${u.file}, Line: ${u.line}\n`;
  });
  prompt += `\nSuggest additional or improved GoogleTest test cases to cover these lines. Output only the new or improved test code, no explanations.`;

  // 4. Send to LLM for suggestions
  try {
    const suggestions = await callLLM(prompt);
    const suggestionsPath = path.join("coverage", "llm_suggestions.cpp");
    fs.writeFileSync(suggestionsPath, suggestions);
    console.log(`💡 LLM suggestions written to: ${suggestionsPath}`);
    console.log("\n--- LLM Suggestions ---\n" + suggestions + "\n----------------------");
  } catch (err) {
    console.error("❌ Failed to get suggestions from LLM:", err.message);
  }
}
