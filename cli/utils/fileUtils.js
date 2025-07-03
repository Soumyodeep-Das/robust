import fs from "fs-extra";
import path from "path";

// Default directories to exclude from recursive search
export const DEFAULT_EXCLUDE_DIRS = [
  "test",
  "tests",
  "third_party",
  ".git",
  "build",
  "scripts",
  "node_modules",
  "coverage",
  "logs"
];

/**
 * Recursively find all source files with given extensions, excluding specified directories.
 * @param {string} rootPath - The directory to start searching from.
 * @param {string[]} extensions - Array of file extensions to match (e.g., ['.cpp', '.cc']).
 * @param {string[]} excludeDirs - Directory names to exclude from search.
 * @returns {Promise<string[]>} - List of absolute file paths.
 */
export async function findSourceFiles(rootPath, extensions = [".cpp", ".cc"], excludeDirs = DEFAULT_EXCLUDE_DIRS) {
  const results = [];
  async function walk(dir) {
    const entries = await fs.readdir(dir, { withFileTypes: true });
    for (const entry of entries) {
      const entryPath = path.join(dir, entry.name);
      if (entry.isDirectory()) {
        if (excludeDirs.includes(entry.name)) continue;
        await walk(entryPath);
      } else if (extensions.some(ext => entry.name.endsWith(ext))) {
        results.push(entryPath);
      }
    }
  }
  await walk(rootPath);
  return results;
}
