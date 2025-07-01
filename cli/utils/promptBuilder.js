import fs from "fs";
import path from "path";
import inquirer from "inquirer";

export async function promptSourceFile() {
  const srcDir = path.resolve("src");
  const files = fs.readdirSync(srcDir).filter(file =>
    file.endsWith(".cpp") || file.endsWith(".c")
  );

  const choices = [
    ...files.map(file => ({ name: file, value: path.join(srcDir, file) })),
    new inquirer.Separator(),
    { name: "🗂 All files in /src", value: "ALL" }
  ];

  const { selectedFile } = await inquirer.prompt([
    {
      type: "list",
      name: "selectedFile",
      message: "📄 Choose a C/C++ file to generate tests for:",
      choices
    }
  ]);

  if (selectedFile === "ALL") {
    return files.map(file => path.join(srcDir, file));
  }

  return [selectedFile];
}

export function buildPrompt(sourceCode, filename = "main.cpp") {
  return `
The user has given the following source code (${filename}):
\`\`\`cpp
${sourceCode}
\`\`\`

Generate a complete GoogleTest-based unit test file named \`test_${filename}\` that:
- Uses appropriate \`#include\` headers
- Uses \`TEST()\` blocks for each function
- Avoids duplicate or redundant tests
- Compiles correctly
- Avoids writing main() again

Output only the contents of the test file (no explanation).
`;
}
