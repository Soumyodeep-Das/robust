# Robust

> 🧠 AI-powered Unit Test Generator for C/C++ Codebases

[![License](https://img.shields.io/github/license/Soumyodeep-Das/robust?style=flat-square)](LICENSE)
[![Node.js](https://img.shields.io/badge/node-%3E%3D18-blue?logo=node.js&style=flat-square)](https://nodejs.org/)
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat-square)](CONTRIBUTING.md)

**Robust** is a powerful CLI tool that automatically generates GoogleTest-compatible unit tests for your C and C++ projects using LLMs hosted via GitHub’s model marketplace.  
Built for automation, edge-case coverage, and modern dev flows.

---

## 🚀 Features

- ✨ AI-powered unit test generation
- 🧪 Generates `test_*.cpp` files using **GoogleTest**
- ⌨️ Works with `.c` and `.cpp` codebases
- 💬 Interactive CLI via `inquirer`
- 🔁 Retry failed builds using logs
- ⚙️ Optional **Keploy** integration for test validation
- 🛠 Makefile support for building tests
- 🔐 GitHub Model Inference API integration

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
npx . init
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
make test   # Builds and runs tests using g++
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

### `src/math.cpp`

```cpp
int divide(int a, int b) {
  if (b == 0) throw std::invalid_argument("division by zero");
  return a / b;
}
```

### `tests/test_math.cpp`

```cpp
#include "gtest/gtest.h"
#include "math.cpp"

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
all:
	g++ -std=c++17 -isystem /usr/include/gtest -pthread tests/test_*.cpp -lgtest -lgtest_main -o bin/test

run:
	./bin/test

test: all run
```

---

## 📁 File Structure

```
robust/
├── bin/                  # Entry for CLI (robust command)
├── cli/                  # init & main prompt logic
│   ├── init.js
│   ├── index.js
│   └── commands/
├── utils/                # Helpers: prompt, logging, model
├── src/                  # Your C/C++ source files
├── tests/                # Generated test_*.cpp files
├── logs/                 # Logs for retry, errors
├── Makefile
├── .env
├── package.json
└── README.md
```

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

---