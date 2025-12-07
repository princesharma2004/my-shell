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
#include <fstream>
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
    std::string echo(const std::string& args);
    std::string not_found(const std::string& cmd, const std::string& args);
    std::string type(const std::unordered_map<std::string, std::function<std::string(const std::string&)>>& cmds, const std::string& cmd);
    std::string pwd();
    std::string cd(const std::string& path);

    std::string run_exec_with_args(const std::string& path, const std::string& args);
    void writeOrCreateFile(const std::string& filePath, const std::string& content);
    void print(const std::string& content);
} // shell

#endif //OUTPUT_H
