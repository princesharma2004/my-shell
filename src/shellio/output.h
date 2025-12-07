/**
 * @file output.h
 * @brief [Brief description of the file]
 *
 * Detailed description of the file purpose and contents.
 *
 * @date 06/12/2025 A
 * @author princesharma
 */
 #ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include  <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <filesystem>
#include <unistd.h>
#include <sstream>
#include <cstdlib>
#include <memory>
#include <array>
#include <ranges>
#include <sys/wait.h>


namespace shell{
    void echo(const std::string& args);
    void not_found(const std::string& cmd, const std::string& args);
    void type(const std::unordered_map<std::string, std::function<void(const std::string&)>>& cmds, const std::string& cmd);
    void pwd();
    void cd(const std::string& path);

    void run_exec_with_args(const std::string& path, const std::string& args);
} // shell

#endif //OUTPUT_H
