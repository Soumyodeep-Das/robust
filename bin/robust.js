#!/usr/bin/env node

const command = process.argv[2];

if (command === "init") {
  import("../cli/init.js");
} else {
  import("../cli/index.js");
}
