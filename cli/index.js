#!/usr/bin/env node
import inquirer from "inquirer";
import generateTest from "./commands/generateTest.js";
import runTest from "./commands/runTest.js";
import retryBuild from "./commands/retryBuild.js";
import viewLogs from "./commands/logViewer.js";
import retryTest from "./commands/retryTest.js";
import runProject from "./commands/runProject.js";
import refineTest from "./commands/refineTest.js";
import coverage from "./commands/coverage.js";
import updateCMake from "./commands/updateCMake.js";

const menu = async () => {
  const { action } = await inquirer.prompt([
    {
      name: "action",
      type: "list",
      message: "Select an action:",
      choices: [
        "Generate Test",
        "Run Project",
        "Refine Test",
        "Coverage",
        "Update CMake",
        "Run Test",
        "Retry Test",
        "Retry Build",
        "View Logs",
        "Exit"
      ],
    },
  ]);

  switch (action) {
    case "Generate Test":
      await generateTest();
      break;
    case "Run Project":
      await runProject();
      break;
    case "Refine Test":
      await refineTest();
      break;
    case "Coverage":
      await coverage();
      break;
    case "Update CMake":
      await updateCMake();
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
