import ModelClient, { isUnexpected } from "@azure-rest/ai-inference";
import { AzureKeyCredential } from "@azure/core-auth";
import dotenv from "dotenv";
dotenv.config();

const token = process.env.GITHUB_TOKEN;
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
