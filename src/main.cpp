#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <utility>

#include "shellio.h"

void setup_cmds(std::unordered_map<std::string, std::function<void(const std::string&)>>& cmds);

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::unordered_map<std::string, std::function<void(const std::string&)>> cmds;
  setup_cmds(cmds);

  while (true) {
    auto [cmd, args] = std::move(shell::read_input());

    if (cmd=="exit") break;

    if (auto it = cmds.find(cmd); it != cmds.end()) {
      it->second(args);
    } else {
      shell::not_found(cmd, args);
    }
  }

  return 0;
}

void setup_cmds(std::unordered_map<std::string, std::function<void(const std::string&)>>& cmds) {
  cmds = {
    { "exit", [&](const std::string&) {} },
    { "echo", [&](const std::string& args) { shell::echo(shell::clean_args(args)); } },
    { "pwd",  [&](const std::string&) { shell::pwd(); } },
    { "type", [&](const std::string& args) { shell::type(cmds, args); } },
    { "cd",   [&](const std::string& args) { shell::cd(args); } },
  };
}
