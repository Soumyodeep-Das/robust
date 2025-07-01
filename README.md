# Robust

> AI-powered Unit Test Generator for C/C++ Codebases

[![License](https://img.shields.io/github/license/yourname/robust?style=flat-square)](LICENSE)
[![Node.js](https://img.shields.io/badge/node-%3E%3D18-blue?logo=node.js\&style=flat-square)](https://nodejs.org/)
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat-square)](CONTRIBUTING.md)

Robust is a powerful, CLI-based AI tool to automatically generate unit tests for your C and C++ projects. It uses state-of-the-art LLMs (hosted via GitHub's model marketplace) to analyze source code and generate comprehensive, edge-case focused test files using GoogleTest. The project also integrates logging, retry flows, build automation, and optional test execution with [Keploy](https://keploy.io).

---

## 🚀 Features

* ✨ AI-powered unit test generation (via GitHub Model Inference API)
* ✍ GoogleTest compatible test files
* ⌚ Support for `.c` and `.cpp` files
* ▶ CLI-based interactive prompts
* 🌐 Local or global install via `robust init`
* 🔄 Retry mechanism from logs
* ⚒ Optional integration with Keploy for automated test validation
* ✅ Makefile-based test builds

---

## 🧠 Technologies Used

* **Node.js** for CLI backend
* **Inquirer.js** for interactive prompts
* **@azure-rest/ai-inference** for LLM API calls
* **dotenv** for environment config
* **fs-extra** for file ops
* **GoogleTest (gtest)** for C++ unit testing
* **Keploy** for capturing API-level test cases (optional)

---

## 📂 Directory Structure

```bash
robust/
├── bin/                  # Entry point for CLI (robust command)
├── cli/                  # CLI command logic
│   ├── index.js          # Main CLI dispatcher
│   ├── init.js           # Local/global install logic
│   └── commands/         # Subcommands: generate, run, retry...
├── utils/                # Prompt builder, API client, log utils
├── tests/                # Generated test_*.cpp files
├── logs/                 # build.log and retry logs
├── src/                  # Your C/C++ source files
├── Makefile              # Build & test runner
├── .env                  # GitHub token for inference
├── package.json          # NPM project config
└── README.md             # You're here
```

---

## 📦 Installation & Setup

### 1. Clone Repository

```bash
git clone https://github.com/yourname/robust.git
cd robust
```

### 2. Run the Installer

```bash
robust init
```

You will be prompted to choose:

* 🌐 **Global install** (use `robust` from anywhere)
* 📁 **Local install** (use `npx robust` inside project only)

This also runs `npm install` automatically.

### 3. Set Up `.env`

Create a `.env` file in the root:

```env
GITHUB_TOKEN=your_personal_token_here
```

> Your token must have `models:read` permission. [Generate token](https://github.com/settings/tokens)

### 4. Install GoogleTest (gtest)

```bash
sudo apt install libgtest-dev cmake
cd /usr/src/gtest
sudo cmake . && sudo make
sudo cp *.a /usr/lib
```

---

## ⚙️ Usage

### Generate Tests

```bash
robust
# OR if locally installed:
npx robust
```

Then select:

```
? Select an action:
✔ Generate Test
  Run Test
  Retry Test
  View Logs
```

Choose the `.cpp`/`.c` file you want to generate tests for, or select ALL in `/src`.

### Run Tests (via Makefile)

```bash
make test
```

### Retry on Build Failure

```bash
robust retry
```

This will read from `logs/build.log` and retry test generation.

### Keploy Integration (Optional)

Install Keploy:

```bash
curl --silent -O -L https://keploy.io/install.sh && source install.sh
```

Capture tests:

```bash
keploy record -c "<your-cpp-binary>"
```

Run captured tests:

```bash
keploy test -c "<your-cpp-binary>"
```

---

## 📋 Example

### Source Code (`src/sample.cpp`):

```cpp
int divide(int a, int b) {
  if (b == 0) throw std::invalid_argument("division by zero");
  return a / b;
}
```

### Generated Test (`tests/test_sample.cpp`):

```cpp
#include "gtest/gtest.h"
#include "sample.cpp"

TEST(DivideTest, HandlesNormalDivision) {
  EXPECT_EQ(divide(10, 2), 5);
}

TEST(DivideTest, HandlesDivideByZero) {
  EXPECT_THROW(divide(10, 0), std::invalid_argument);
}
```

---

## 🛠️ Makefile

```makefile
all:
	g++ -std=c++17 -isystem /usr/include/gtest -pthread tests/test_*.cpp -lgtest -lgtest_main -o bin/test

run:
	./bin/test

test: all run
```

---

## 🤝 Contributing

We welcome contributions! Please see [`CONTRIBUTING.md`](CONTRIBUTING.md) for how to get started.

---

## 📄 License

MIT License. See [LICENSE](LICENSE).

---

## 🌟 Star & Share

If you found this useful, please consider giving us a star on GitHub and spreading the word!

```bash
git clone https://github.com/yourname/robust
cd robust
robust init
```
