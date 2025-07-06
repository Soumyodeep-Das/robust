import fs from "fs-extra";
import path from "path";

/**
 * Scan generated tests and update CMakeLists.txt to include them.
 */
export default async function updateCMake() {
  const testsDir = path.resolve("tests");
  const cmakePath = path.resolve("CMakeLists.txt");
  if (!fs.existsSync(testsDir)) {
    console.error("❌ tests/ directory not found.");
    return;
  }
  if (!fs.existsSync(cmakePath)) {
    console.error("❌ CMakeLists.txt not found.");
    return;
  }

  // 1. Find all test_*.cpp and test_*.cc files
  const testFiles = fs.readdirSync(testsDir)
    .filter(f => /^test_.*\.(cpp|cc)$/.test(f));
  if (testFiles.length === 0) {
    console.log("No generated test files found in tests/.");
    return;
  }

  // 2. Read CMakeLists.txt
  let cmakeContent = fs.readFileSync(cmakePath, "utf-8");
  let updated = false;
  let added = [];

  // 3. For each test file, add add_executable and add_test if missing
  for (const testFile of testFiles) {
    const testName = path.basename(testFile, path.extname(testFile));
    const exeLine = `add_executable(${testName} tests/${testFile})`;
    const testLine = `add_test(NAME ${testName} COMMAND ${testName})`;
    if (!cmakeContent.includes(exeLine)) {
      cmakeContent += `\n${exeLine}`;
      updated = true;
      added.push(exeLine);
    }
    if (!cmakeContent.includes(testLine)) {
      cmakeContent += `\n${testLine}`;
      updated = true;
      added.push(testLine);
    }
  }

  // 4. Write back if updated
  if (updated) {
    fs.writeFileSync(cmakePath, cmakeContent);
    console.log(`✅ Updated CMakeLists.txt with ${added.length / 2} test(s):\n` + added.join("\n"));
  } else {
    console.log("CMakeLists.txt already up to date with all generated tests.");
  }
}
