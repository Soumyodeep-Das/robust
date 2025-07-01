#!/usr/bin/env node
import inquirer from "inquirer";
import generateTest from "./commands/generateTest.js";
import runTest from "./commands/runTest.js";
import retryBuild from "./commands/retryBuild.js";
import viewLogs from "./commands/logViewer.js";
import retryTest from "./commands/retryTest.js";

const menu = async () => {
  const { action } = await inquirer.prompt([
    {
      name: "action",
      type: "list",
      message: "Select an action:",
      choices: ["Generate Test", "Run Test", "Retry Test", "Retry Build", "View Logs", "Exit"],
    },
  ]);

  switch (action) {
    case "Generate Test":
      await generateTest();
      break;
    case "Run Test":
      await runTest();
      break;
    case "Retry Test":
      await retryTest();
      break;
    case "Retry Build":
      await retryBuild();
      break;
    case "View Logs":
      await viewLogs();
      break;
    default:
      console.log("Exiting...");
  }
};

menu();
