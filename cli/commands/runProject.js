import fs from "fs-extra";
import path from "path";
import { spawn } from "child_process";

/**
 * Build and run tests for the C++ project. Captures build and test logs.
 * If build fails, logs are saved for LLM refinement.
 * TODO: Integrate with LLM for auto-fix loop.
 */
export default async function runProject() {
  const buildDir = path.resolve("build");
  fs.ensureDirSync("logs");
  const buildLog = fs.createWriteStream("logs/build.log");
  const testLog = fs.createWriteStream("logs/test.log");

  // Prefer build.sh if present, else try cmake/make
  let buildCmd = null, buildArgs = [], buildCwd = process.cwd();
  if (fs.existsSync("build.sh")) {
    buildCmd = "bash";
    buildArgs = ["build.sh"];
  } else if (fs.existsSync("CMakeLists.txt")) {
    fs.ensureDirSync(buildDir);
    buildCmd = "cmake";
    buildArgs = [".."];
    buildCwd = buildDir;
  } else {
    console.error("No build.sh or CMakeLists.txt found. Cannot build project.");
    return;
  }

  console.log("🔨 Building project...");
  const buildProc = spawn(buildCmd, buildArgs, { cwd: buildCwd });
  buildProc.stdout.pipe(buildLog);
  buildProc.stderr.pipe(buildLog);

  await new Promise((resolve) => buildProc.on("close", resolve));

  // Check for build success
  if (fs.existsSync(path.join(buildDir, "Makefile"))) {
    // Run make
    console.log("🏗️  Running make...");
    const makeProc = spawn("make", [], { cwd: buildDir });
    makeProc.stdout.pipe(buildLog);
    makeProc.stderr.pipe(buildLog);
    await new Promise((resolve) => makeProc.on("close", resolve));
  }

  // Find and run test binaries in build/tests or build/
  const testDirs = [path.join(buildDir, "tests"), buildDir];
  let testsRun = false;
  for (const dir of testDirs) {
    if (fs.existsSync(dir)) {
      const files = fs.readdirSync(dir);
      for (const file of files) {
        if (file.startsWith("test") && fs.statSync(path.join(dir, file)).mode & 0o111) {
          console.log(`🚦 Running test binary: ${file}`);
          const testProc = spawn(path.join(dir, file), []);
          testProc.stdout.pipe(testLog);
          testProc.stderr.pipe(testLog);
          await new Promise((resolve) => testProc.on("close", resolve));
          testsRun = true;
        }
      }
    }
  }
  if (!testsRun) {
    console.warn("No test binaries found to run.");
  }
  console.log("✅ Build and test run complete. Logs saved in logs/.");
}
