#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <utility>

#include "shellio.h"

void setup_cmds(std::unordered_map<std::string, std::function<std::string(const std::string&)>>& cmds);

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::unordered_map<std::string, std::function<std::string(const std::string&)>> cmds;
  setup_cmds(cmds);

  while (true) {
    auto [cmd, args, file] = std::move(shell::read_input());

    if (cmd=="exit") break;

    std::string output;
    if (auto it = cmds.find(cmd); it != cmds.end()) {
       output = it->second(args);
    } else {
      output = shell::not_found(cmd, args);
    }

    if (file.empty()) {
      shell::print(output);
    } else {
      shell::writeOrCreateFile(file, output);
    }
  }

  return 0;
}

void setup_cmds(std::unordered_map<std::string, std::function<std::string(const std::string&)>>& cmds) {
  cmds = {
    { "exit", [&](const std::string&) {return "";} },
    { "echo", [&](const std::string& args) { return shell::echo(shell::clean_args(args)); } },
    { "pwd",  [&](const std::string&) { return shell::pwd(); } },
    { "type", [&](const std::string& args) { return shell::type(cmds, args); } },
    { "cd",   [&](const std::string& args) { return shell::cd(args); } },
  };
}
