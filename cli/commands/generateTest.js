import fs from "fs-extra";
import path from "path";
import { fileURLToPath } from "url";
import { buildPrompt, promptSourceFile } from "../utils/promptBuilder.js";
import { callLLM } from "../utils/apiClient.js";

const __dirname = path.dirname(fileURLToPath(import.meta.url));

export default async function generateTest() {
  const selectedFiles = await promptSourceFile(); // ✅ New prompt logic

  for (const inputPath of selectedFiles) {
    if (!fs.existsSync(inputPath)) {
      console.error("❌ Source file not found:", inputPath);
      continue;
    }

    const filename = path.basename(inputPath);
    const outputPath = path.join("tests", `test_${filename}`);
    const sourceCode = fs.readFileSync(inputPath, "utf-8");

    const prompt = buildPrompt(sourceCode, filename);

    try {
      const testOutput = await callLLM(prompt);
      fs.ensureDirSync("tests");
      fs.writeFileSync(outputPath, testOutput);
      console.log("✅ Test generated and saved to:", outputPath);
    } catch (err) {
      console.error(`❌ Failed to generate test for ${filename}:`, err.message);
    }
  }
}
