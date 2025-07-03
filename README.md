# Robust

> 🧠 AI-powered Unit Test Generator for C/C++ Codebases

[![License](https://img.shields.io/github/license/Soumyodeep-Das/robust?style=flat-square)](LICENSE)
[![Node.js](https://img.shields.io/badge/node-%3E%3D18-blue?logo=node.js&style=flat-square)](https://nodejs.org/)
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat-square)](CONTRIBUTING.md)

**Robust** is a powerful CLI tool that automatically generates GoogleTest-compatible unit tests for your C and C++ projects using LLMs hosted via GitHub’s model marketplace.  
Built for automation, edge-case coverage, and modern dev flows.

---

## 🚀 Features

- ✨ AI-powered unit test generation for C and C++
- 🔍 **Recursive source discovery**: No need to move files—finds `.cpp`/`.cc` files in any project structure
- 🧪 Generates `test_*.cpp` (GoogleTest) and `test_*.c` (Unity) with header/source/test split
- 🗂️ Output auto-organized into `src/`, `tests/`, `include/`, `logs/`, `coverage/`
- ⚡ **One-command automation**: `robust generate --all` for instant test generation
- 🏗️ **Build, run, and test**: `robust run` automates CMake or build.sh and runs all tests
- 🔄 **Refinement loop**: `robust refine` sends failed builds/tests to LLM for auto-repair
- 📈 **Code coverage integration**: `robust coverage` collects and uses gcov/lcov data to improve tests
- 🛠️ **CMake auto-integration**: `robust update-cmake` adds new tests to your build config
- 💬 Interactive CLI menu for all commands

---

## 🧠 Tech Stack

| Layer            | Technology                     |
|------------------|-------------------------------|
| CLI Interface    | Node.js + Inquirer.js         |
| AI Inference     | GitHub Models via Azure SDK   |
| Env Handling     | dotenv                         |
| File Ops         | fs-extra                       |
| C++ Test Engine  | GoogleTest (gtest)             |
| Optional Testing | Keploy                         |

---

## 📦 Quickstart

### ✅ Clone and Initialize

```bash
git clone https://github.com/Soumyodeep-Das/robust.git
cd robust
npm install
npm link   # (optional, for global usage)
```

You’ll be asked to choose between:

- 🌐 Global Install (`robust` usable from anywhere)
- 📁 Local Install (`npx robust` usable in current folder)

---

## ⚙️ Setup Guide

### 🔑 1. Set up `.env`

Create a `.env` file in the root with your GitHub token:

```env
GITHUB_TOKEN=your_personal_token_here
```

> Your token must have `models:read` permission  
> 👉 [Generate one here](https://github.com/settings/tokens)

---

### 🔧 2. Install GoogleTest

```bash
sudo apt install libgtest-dev cmake
cd /usr/src/gtest
sudo cmake . && sudo make
sudo cp lib/*.a /usr/lib
```

---

### 🧩 2. Using robust in a C++ Project (Any Repo)

#### **Step-by-step workflow:**

1. **Clone your target C++ repository:**
   ```bash
   git clone <repo_url>
   cd <repo>
   ```
2. **(Optional) Initialize robust config:**
   ```bash
   robust init --project <your_project_name>
   ```
3. **Generate tests for all source files:**
   ```bash
   robust generate --all
   ```
   - Recursively finds all `.cpp`/`.cc` files (ignores `test/`, `third_party/`, etc.)
   - Generates header, cleaned source, and GoogleTest/Unity test files into `src/` and `tests/`
4. **Update your CMake config:**
   ```bash
   robust update-cmake
   ```
5. **Build and run tests:**
   ```bash
   robust run
   ```
   - Runs `build.sh` or CMake/Make as appropriate
   - Executes all test binaries, saves logs
6. **Refine tests after build/test failures:**
   ```bash
   robust refine
   ```
   - Sends failures/logs to LLM for suggestions/fixes (scaffolded for future automation)
7. **Collect and improve code coverage:**
   ```bash
   robust coverage
   ```
   - Runs `gcov`/`lcov`, parses coverage, prompts LLM to improve tests

**Or use the interactive menu:**
```bash
robust
```
And select actions from:
- Generate Test
- Run Project
- Refine Test
- Coverage
- Update CMake
- ...

---

---

## 🖥 Usage

### Launch CLI

```bash
robust         # if globally installed
npx robust     # if locally installed
```

### Options:

```
? Select an action:
❯ Generate Test
  Run Test
  Retry Test
  Retry Build
  View Logs
  Exit
```

You’ll be prompted to select individual files or all `.c/.cpp` files inside `/src`.

---

### 🧪 Build & Run Tests

```bash
make run_c         # Builds and runs C tests (Unity, bin/test_output_c)
make run_cpp       # Builds and runs C++ tests (GoogleTest, bin/test_output_cpp)
make run           # Builds and runs both C and C++ tests (if present)
```

Or use the CLI for a unified experience:

```bash
robust             # Launch CLI, select "Run Test" for C or C++
```

---

### 🔁 Retry Failed Builds

```bash
robust retry
```

Retries failed test generations by analyzing `logs/build.log`.

---

### ⚡ Keploy Integration (Optional)

```bash
curl --silent -O -L https://keploy.io/install.sh && source install.sh

keploy record -c "./bin/test"
keploy test -c "./bin/test"
```

---

## 🔍 Example

### Example: Unity-based C Test

#### `src/sample_c_codebase.c`

```c
#include "sample_c_codebase.h"
// ... your C functions ...
```

#### `tests/test_sample_c_codebase.c`

```c
#include "unity/unity.h"
#include "sample_c_codebase.h"

void setUp(void) {}
void tearDown(void) {}

void test_validate_email() {
    TEST_ASSERT_TRUE(validate_email("test@example.com"));
    TEST_ASSERT_FALSE(validate_email("invalid@email"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_validate_email);
    return UNITY_END();
}
```

---

### Example: GoogleTest C++ Test

#### `src/math.cpp`

```cpp
int divide(int a, int b) {
  if (b == 0) throw std::invalid_argument("division by zero");
  return a / b;
}
```

#### `tests/test_math.cpp`

```cpp
#include "gtest/gtest.h"
#include "math.hpp"

TEST(DivideTest, ValidDivision) {
  EXPECT_EQ(divide(10, 2), 5);
}

TEST(DivideTest, DivideByZero) {
  EXPECT_THROW(divide(10, 0), std::invalid_argument);
}
```

---

## 🛠 Makefile (Provided)

```makefile
# Build and run C tests (Unity)
run_c: bin/test_output_c
	@echo "Running C tests (Unity):"
	./bin/test_output_c || true

bin/test_output_c: $(SRC_C_FILES) $(TEST_C_FILES) tests/unity/unity.c
	@mkdir -p bin logs
	gcc -Wall -Wextra -Iinclude -Isrc -Itests -Itests/unity $^ -o bin/test_output_c 2> logs/build.log

# Build and run C++ tests (GoogleTest)
run_cpp: bin/test_output_cpp
	@echo "Running C++ tests (GoogleTest):"
	./bin/test_output_cpp || true

bin/test_output_cpp: $(SRC_CPP_FILES) $(TEST_CPP_FILES)
	@mkdir -p bin logs
	g++ -std=c++17 -Wall -Wextra -Iinclude -Isrc -pthread $^ -lgtest -lgtest_main -o bin/test_output_cpp 2> logs/build.log

# Run both
run: run_cpp run_c
```

---

## 📁 File Structure

```
robust/
├── bin/                  # Test binaries: test_output_c (C), test_output_cpp (C++)
├── cli/                  # CLI logic and commands
│   ├── init.js
│   ├── index.js
│   └── commands/
├── utils/                # Helpers: prompt, logging, model
├── src/                  # Your C/C++ source files
├── tests/                # Generated test_*.c (Unity) and test_*.cpp (GoogleTest) files
│   └── unity/            # Unity C test framework (unity.h, unity.c, unity_internals.h)
├── logs/                 # Logs for retry, errors
├── Makefile
├── .env
├── package.json
└── README.md
```

---

## 🛠 Troubleshooting & Regenerating Tests

### Common C Test Build Errors (Unity)
- **undefined reference to `setUp`/`tearDown`**: Your test file must define empty `void setUp(void) {}` and `void tearDown(void) {}` functions.
- **Missing Unity includes or macros**: All C tests must include `#include "unity/unity.h"` and use `TEST_ASSERT_*` macros, not `assert.h`.
- **Multiple `main()` functions**: Only one `main()` per C test file, registering all tests with `UNITY_BEGIN`, `RUN_TEST`, and `UNITY_END`.

### Common C++ Test Build Errors (GoogleTest)
- **Missing `gtest/gtest.h` include**: All C++ tests must include GoogleTest headers and use `TEST`/`TEST_F` macros.
- **Custom/main functions in test files**: Let GoogleTest manage the test runner; do not define your own `main()`.

### How to Regenerate or Fix Tests
- Use the CLI's `Retry Build` or `Retry Test` options to automatically fix test files based on build logs.
- Edit test files to match the examples above, then re-run `make run_c` or `make run_cpp`.

---

## 🤝 Contributing

We welcome all PRs, suggestions, and improvements!  
See [`CONTRIBUTING.md`](CONTRIBUTING.md) to get started.

---

## 📜 License

This project is licensed under the **MIT License**. See [`LICENSE`](LICENSE) for details.

---

## 🌟 Support & Share

```bash
git clone https://github.com/Soumyodeep-Das/robust.git
cd robust
npx . init
```

If you find this useful, please ⭐ star the repo and share it with your developer friends!
```
