import fs from "fs";
import path from "path";
import inquirer from "inquirer";

export async function promptSourceFile(startDir = process.cwd()) {
  let currentDir = path.resolve(startDir);
  while (true) {
    // List files and directories in currentDir
    const entries = fs.readdirSync(currentDir, { withFileTypes: true });
    const files = entries.filter(e => e.isFile() && (e.name.endsWith('.c') || e.name.endsWith('.cpp') || e.name.endsWith('.cc')));
    const dirs = entries.filter(e => e.isDirectory());

    const choices = [];
    if (currentDir !== path.parse(currentDir).root) {
      choices.push({ name: '⬆️  Go up one directory', value: '..' });
    }
    dirs.forEach(dir => {
      choices.push({ name: `📁 ${dir.name}/`, value: { type: 'dir', name: dir.name } });
    });
    files.forEach(file => {
      choices.push({ name: `📄 ${file.name}`, value: { type: 'file', name: file.name } });
    });
    if (files.length > 0) {
      choices.push(new inquirer.Separator());
      choices.push({ name: `🗂 All .c/.cpp/.cc files in ${path.relative(startDir, currentDir) || '.'}`, value: { type: 'all' } });
    }
    if (choices.length === 0) {
      choices.push({ name: 'No C/C++ files or subdirectories found here', value: '__none__', disabled: true });
    }

    const { selected } = await inquirer.prompt([
      {
        type: 'list',
        name: 'selected',
        message: `Choose a directory or C/C++ file in: ${path.relative(startDir, currentDir) || '.'}`,
        choices
      }
    ]);

    if (selected === '..') {
      currentDir = path.dirname(currentDir);
      continue;
    }
    if (selected && selected.type === 'dir') {
      currentDir = path.join(currentDir, selected.name);
      continue;
    }
    if (selected && selected.type === 'file') {
      return [path.join(currentDir, selected.name)];
    }
    if (selected && selected.type === 'all') {
      // Recursively find all .c/.cpp/.cc files in currentDir
      const allFiles = [];
      function walk(dir) {
        const ents = fs.readdirSync(dir, { withFileTypes: true });
        for (const ent of ents) {
          if (ent.isDirectory()) {
            walk(path.join(dir, ent.name));
          } else if (ent.name.endsWith('.c') || ent.name.endsWith('.cpp') || ent.name.endsWith('.cc')) {
            allFiles.push(path.join(dir, ent.name));
          }
        }
      }
      walk(currentDir);
      if (allFiles.length === 0) {
        console.log('No C/C++ files found in this directory or subdirectories.');
        continue;
      }
      return allFiles;
    }
  }
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
