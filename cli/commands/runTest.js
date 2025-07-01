import { execSync } from "child_process";
import readline from "readline";
import retryBuild from "./retryBuild.js";

function askRetryQuestion(question) {
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });

  return new Promise(resolve => {
    rl.question(question, answer => {
      rl.close();
      resolve(answer.trim().toLowerCase());
    });
  });
}

export async function runTest() {
  try {
    console.log("🔧 Compiling and running tests using Make...");
    execSync("make run", { stdio: "inherit" });
  } catch (err) {
    console.error("❌ Build or test run failed. See logs in logs/build.log");

    const answer = await askRetryQuestion("🔁 Retry with AI fix? (Y/n): ");
    if (answer === "y" || answer === "yes") {
      await retryBuild();

      // Ask if user wants to re-run test
      const rerun = await askRetryQuestion("▶️ Re-run test after fix? (Y/n): ");
      if (rerun === "y" || rerun === "yes") {
        runTest();
      } else {
        console.log("📌 You can manually run: `make run` later.");
      }
    } else {
      console.log("🚫 Skipping retry.");
    }
  }
}

export default runTest;
