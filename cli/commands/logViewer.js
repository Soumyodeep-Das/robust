import fs from "fs";

function viewLogs() {
  const logs = fs.readFileSync("logs/build.log", "utf-8");
  console.log("🪵 Build Logs:\n", logs);
}

export default viewLogs;
