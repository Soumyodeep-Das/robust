import { execSync, spawnSync } from "child_process";
import readline from "readline";
import inquirer from "inquirer";
import { promptSourceFile } from "../utils/promptBuilder.js";
import ora from "ora";
import retryBuild from "./retryBuild.js";
import fs from "fs";
import path from "path";
import chalk from "chalk";

function askRetryQuestion(question) {
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });

  return new Promise((resolve) => {
    rl.question(question, (answer) => {
      rl.close();
      resolve(answer.trim().toLowerCase());
    });
  });
}

export async function runTest() {
  // Use the new promptSourceFile logic for test selection
  let chosenTest = null;
  let testFiles = await promptSourceFile(path.resolve("tests"));
  if (testFiles.length === 1) {
    chosenTest = path.basename(testFiles[0]);
  } else if (testFiles.length > 1) {
    chosenTest = "ALL";
  } else {
    console.log(chalk.yellow("No test files selected."));
    return;
  }

  const compileCommand = buildCompileCommand(chosenTest);

  let spinner;
  try {
    spinner = ora(chalk.blue("🔧 Compiling and running tests using Make...")).start();
    execSync(compileCommand, { stdio: "inherit" });
    spinner.succeed(chalk.green("✅ Test compilation and execution finished."));

    // Run the test executable and show output
    // Determine which test binary to run based on file extension
    let testOutputPath = null;
    let testType = null;
    if (chosenTest === "ALL") {
      // Run both if present
      const cppBin = path.join(process.cwd(), 'bin', 'test_output_cpp');
      const cBin = path.join(process.cwd(), 'bin', 'test_output_c');
      let ranAny = false;
      let combinedOutput = '';
      if (fs.existsSync(cppBin) && fs.statSync(cppBin).mode & 0o111) {
        console.log(chalk.blue('\n🧪 Running C++ tests (GoogleTest):'));
        fs.mkdirSync('logs', { recursive: true });
        const runResult = spawnSync(cppBin, [], { encoding: 'utf-8' });
        const output = (runResult.stdout || '') + (runResult.stderr || '');
        combinedOutput += '--- C++ Test Output ---\n' + output + '\n';
        process.stdout.write(output);
        if (typeof runResult.status === 'number') {
          if (runResult.status === 0) {
            console.log(chalk.green('✅ All C++ tests passed.'));
          } else {
            console.log(chalk.red(`❌ C++ test binary exited with code ${runResult.status}. Some tests may have failed.`));
          }
        } else if (runResult.signal) {
          console.log(chalk.red(`❌ C++ test binary terminated by signal: ${runResult.signal}`));
        } else {
          console.log(chalk.yellow('⚠️ C++ test binary finished with unknown status.'));
        }
        ranAny = true;
      }
      if (fs.existsSync(cBin) && fs.statSync(cBin).mode & 0o111) {
        console.log(chalk.blue('\n🧪 Running C tests (Unity):'));
        fs.mkdirSync('logs', { recursive: true });
        const runResult = spawnSync(cBin, [], { encoding: 'utf-8' });
        const output = (runResult.stdout || '') + (runResult.stderr || '');
        combinedOutput += '--- C Test Output ---\n' + output + '\n';
        process.stdout.write(output);
        if (typeof runResult.status === 'number') {
          if (runResult.status === 0) {
            console.log(chalk.green('✅ All C tests passed.'));
          } else {
            console.log(chalk.red(`❌ C test binary exited with code ${runResult.status}. Some tests may have failed.`));
          }
        } else if (runResult.signal) {
          console.log(chalk.red(`❌ C test binary terminated by signal: ${runResult.signal}`));
        } else {
          console.log(chalk.yellow('⚠️ C test binary finished with unknown status.'));
        }
        ranAny = true;
      }
      if (!ranAny) {
        console.log(chalk.yellow('⚠️ No test binaries found or not executable.'));
      }
      // Write combined output to logs/test.log
      fs.mkdirSync('logs', { recursive: true });
      fs.writeFileSync('logs/test.log', combinedOutput);
    } else if (chosenTest.endsWith('.c')) {
      testOutputPath = path.join(process.cwd(), 'bin', 'test_output_c');
      testType = 'C (Unity)';
    } else if (chosenTest.endsWith('.cpp') || chosenTest.endsWith('.cc')) {
      testOutputPath = path.join(process.cwd(), 'bin', 'test_output_cpp');
      testType = 'C++ (GoogleTest)';
    }
    if (testOutputPath) {
      const resultSpinner = ora(chalk.blue(`🦾 Running test binary for ${testType}...`)).start();
      try {
        resultSpinner.stop(); // stop spinner before live output
        fs.mkdirSync('logs', { recursive: true });
        const runResult = spawnSync(testOutputPath, [], { encoding: 'utf-8' });
        const output = (runResult.stdout || '') + (runResult.stderr || '');
        fs.writeFileSync('logs/test.log', output);
        process.stdout.write(output);
        if (typeof runResult.status === 'number') {
          if (runResult.status === 0) {
            console.log(chalk.green(`✅ All ${testType} tests passed.`));
          } else {
            console.log(chalk.red(`❌ ${testType} test binary exited with code ${runResult.status}. Some tests may have failed.`));
          }
        } else if (runResult.signal) {
          console.log(chalk.red(`❌ ${testType} test binary terminated by signal: ${runResult.signal}`));
        } else {
          console.log(chalk.yellow(`⚠️ ${testType} test binary finished with unknown status.`));
        }
      } catch (e) {
        resultSpinner.fail(chalk.red(`❌ Failed to run ${testType} test binary (execution error).`));
        console.error(chalk.red('--- Execution Error Diagnostics ---'));
        console.error(e);
        console.error(chalk.magenta('cwd:'), process.cwd());
        console.error(chalk.magenta('env.PATH:'), process.env.PATH);
        console.error(chalk.magenta('testOutputPath:'), testOutputPath);
        try {
          const stat = fs.statSync(testOutputPath);
          console.error(chalk.magenta('testOutputPath stat:'), stat);
        } catch (statErr) {
          console.error(chalk.red('Could not stat testOutputPath:'), statErr);
        }
        console.error(chalk.red('-----------------------------------'));
      }
    }

    // Show coverage if available
    const coveragePath = path.join(process.cwd(), 'bin', 'coverage.txt');
    if (fs.existsSync(coveragePath)) {
      const covSpinner = ora(chalk.blue('📊 Fetching coverage report...')).start();
      try {
        const coverage = fs.readFileSync(coveragePath, 'utf8');
        covSpinner.succeed(chalk.green('🟩 Coverage Report:'));
        console.log(chalk.cyan('--------------------\n' + coverage + '\n--------------------'));
      } catch (e) {
        covSpinner.fail(chalk.red('❌ Failed to read coverage report.'));
      }
    }
  } catch (err) {
    if (spinner) spinner.fail(chalk.red("❌ Build or test run failed. See logs in logs/build.log"));

    const answer = await askRetryQuestion(chalk.magenta("🔁 Retry with AI fix? (Y/n): "));
    if (answer === "y" || answer === "yes") {
      await retryBuild(testFiles && testFiles.length === 1 ? testFiles[0] : chosenTest);

      const rerun = await askRetryQuestion(chalk.magenta("▶️ Re-run test after fix? (Y/n): "));
      if (rerun === "y" || rerun === "yes") {
        runTest();
      } else {
        console.log(chalk.yellow("📌 You can manually run the test later."));
      }
    } else {
      console.log(chalk.yellow("🚫 Skipping retry."));
    }
  }
}

function buildCompileCommand(chosenTest) {
  // Returns the appropriate build command for the selected test
  const cBaseCommand = "gcc -Wall -Wextra -Iinclude -Isrc -Itests -Itests/unity";
  const cOutputPath = "-o bin/test_output_c 2> logs/build.log";
  const cUnitySrc = "tests/unity/unity.c";

  const cppBaseCommand = "g++ -std=c++17 -Wall -Wextra -Iinclude -Isrc -pthread";
  const cppOutputPath = "-o bin/test_output_cpp 2> logs/build.log";
  const gtestFlags = "-lgtest -lgtest_main";

  if (chosenTest === "ALL") {
    // Build both C and C++ binaries
    const srcCFiles = fs.readdirSync("src").filter(f => f.endsWith(".c")).map(f => `src/${f}`).join(" ");
    const testCFiles = getTestFileChoices().filter(f => f.value.endsWith('.c')).map(f => `tests/${f.value}`).join(" ");
    const cCmd = `${cBaseCommand} ${srcCFiles} ${testCFiles} ${cUnitySrc} ${cOutputPath}`;

    const srcCppFiles = fs.readdirSync("src").filter(f => f.endsWith(".cpp")).map(f => `src/${f}`).join(" ");
    const testCppFiles = getTestFileChoices().filter(f => f.value.endsWith('.cpp')).map(f => `tests/${f.value}`).join(" ");
    const cppCmd = `${cppBaseCommand} ${srcCppFiles} ${testCppFiles} ${gtestFlags} ${cppOutputPath}`;

    // Return both commands separated by '&&' so both are built
    return `${cppCmd} && ${cCmd}`;
  } else if (chosenTest.endsWith('.c')) {
    // Build C test binary
    const testFilePath = chosenTest.startsWith('/') ? chosenTest : path.resolve('tests', chosenTest);
    const relatedSrc = inferSourceFileFromTest(testFilePath);
    return `${cBaseCommand} ${relatedSrc} ${testFilePath} ${cUnitySrc} ${cOutputPath}`;
  } else if (chosenTest.endsWith('.cpp') || chosenTest.endsWith('.cc')) {
    // Build C++ test binary
    const testFilePath = chosenTest.startsWith('/') ? chosenTest : path.resolve('tests', chosenTest);
    const relatedSrc = inferSourceFileFromTest(testFilePath);
    return `${cppBaseCommand} ${relatedSrc} ${testFilePath} ${gtestFlags} ${cppOutputPath}`;
  } else {
    throw new Error(`Unknown test file type for buildCompileCommand: ${chosenTest}`);
  }
}

// Removed getTestFileChoices; replaced by promptSourceFile logic.

function inferSourceFileFromTest(testFilePath) {
  const baseName = path.basename(testFilePath).replace(/^test_/, "");
  const sourcePath = path.join("src", baseName);
  return fs.existsSync(sourcePath) ? sourcePath : "";
}

export default runTest;