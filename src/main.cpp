#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <utility>

#include "shellio.h"

void setup_cmds(std::unordered_map<std::string, std::function<std::pair<std::string, std::string>(const std::string&)>>& cmds);

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::unordered_map<std::string, std::function<std::pair<std::string, std::string>(const std::string&)>> cmds;
  setup_cmds(cmds);

  while (true) {
    std::string input = shell::read_input();
    auto [cmd, args, outputFile, errorFile] = std::move(shell::break_input(input));

    if (cmd=="exit") break;

    if (auto it = cmds.find(cmd); it != cmds.end()) {
       auto [output, error] = it->second(args);

      if (outputFile.empty()) {
        shell::print(output);
      } else {
        shell::writeOrCreateFile(outputFile, output);
      }

      if (errorFile.empty()) {
        shell::print(error);
      } else {
        shell::writeOrCreateFile(errorFile, error);
      }
    } else {
      shell::not_found(cmd, input);
    }
  }

  return 0;
}

void setup_cmds(std::unordered_map<std::string, std::function<std::pair<std::string, std::string>(const std::string&)>>& cmds) {
  cmds = {
    { "exit", [&](const std::string&) {return std::make_pair("", "");} },
    { "echo", [&](const std::string& args) { return shell::echo(shell::clean_args(args)); } },
    { "pwd",  [&](const std::string&) { return shell::pwd(); } },
    { "type", [&](const std::string& args) { return shell::type(cmds, args); } },
    { "cd",   [&](const std::string& args) { return shell::cd(args); } },
  };
}
