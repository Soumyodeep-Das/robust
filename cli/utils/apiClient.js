import ModelClient, { isUnexpected } from "@azure-rest/ai-inference";
import { AzureKeyCredential } from "@azure/core-auth";
import * as dotenvFlow from "dotenv-flow";
import path from "path";
import { fileURLToPath } from "url";

// Get __dirname (ESM-safe)
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// Always load .env from project root, no matter where `robust` is called from
const projectRoot = path.resolve(__dirname, "../../");
dotenvFlow.config({ path: projectRoot, silent: true }); // silent = suppress missing .env warning

// Validate and extract token
const token = process.env.GITHUB_TOKEN;
if (!token || token.trim() === "") {
  throw new Error("Missing API key: Set GITHUB_TOKEN in your .env file.");
}

// Configure GitHub-hosted model
const endpoint = "https://models.github.ai/inference";
const model = "mistral-ai/mistral-medium-2505";

const client = ModelClient(endpoint, new AzureKeyCredential(token));

export async function callLLM(prompt) {
  const response = await client.path("/chat/completions").post({
    body: {
      messages: [
        {
          role: "system",
          content: `You are a strict, reliable, and security-focused unit test generator for C/C++ code. 
For every request, output all required files in the following format:

/*** File: <filename> ***/
<file content>

Do this for each file (header, source, and test) required by the user's instructions. Do not include any explanations or extra text.`,
        },
        {
          role: "user",
          content: prompt,
        },
      ],
      temperature: 0.3,
      top_p: 1.0,
      model: model,
    },
  });

  if (isUnexpected(response)) {
    throw new Error(response.body.error.message || "LLM response error");
  }

  return response.body.choices[0].message.content;
}
