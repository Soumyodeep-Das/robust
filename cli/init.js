import inquirer from "inquirer";
import { execSync } from "child_process";
import path from "path";

async function init() {
  const { scope } = await inquirer.prompt([
    {
      type: "list",
      name: "scope",
      message: "📦 How would you like to install Robust?",
      choices: [
        { name: "🌐 Global (use `robust` from anywhere)", value: "global" },
        { name: "📁 Local (only inside this folder)", value: "local" }
      ]
    }
  ]);

  if (scope === "global") {
    try {
      console.log("📦 Installing globally...");
      execSync("npm install -g .", { stdio: "inherit" });
      console.log("✅ You can now run `robust` from anywhere!");
    } catch (err) {
      console.error("❌ Global install failed:", err.message);
    }
  } else {
    try {
      console.log("📦 Installing locally...");
      execSync("npm install", { stdio: "inherit" });
      console.log("✅ Installed locally. Use `npx robust` inside this folder.");
    } catch (err) {
      console.error("❌ Local install failed:", err.message);
    }
  }
}

init();
