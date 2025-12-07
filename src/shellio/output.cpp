/**
 * @file output.cpp
 * @brief [Brief description of the file]
 *
 * Detailed description of the file purpose and contents.
 *
 * @date 06/12/2025 A
 * @author princesharma
 */
 #include "output.h"

namespace shell {
    std::pair<std::string, std::string> echo(const std::string &args) {
        std::stringstream output, error;
        output << args << "\n";
        return std::make_pair(output.str(), error.str());
    }

    void not_found(const std::string &cmd, const std::string &input) {
        bool foundAny = false;

        if (const char* pathEnv = getenv("PATH")) {
            std::stringstream ss(pathEnv);
            std::string dir;

            while (std::getline(ss, dir, ':')) {
                std::filesystem::path candidate = std::filesystem::path(dir) / cmd;

                if (std::filesystem::exists(candidate) &&
                    std::filesystem::is_regular_file(candidate) &&
                    access(candidate.c_str(), X_OK) == 0) {

                    std::array<char, 256> buffer{};
                    FILE* pipe = popen(input.c_str(), "r");

                    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                        std::cout << buffer.data();
                    }
                    foundAny = true;
                    break;
                }
            }
        }

        if (!foundAny) {
            std::cout << cmd << ": command not found\n";
        }
    }

    std::pair<std::string, std::string> type(
        const std::unordered_map<std::string, std::function<std::pair<std::string, std::string>(const std::string &)> >
        &cmds, const std::string &cmd) {
        std::stringstream output, error;

        if (cmds.contains(cmd)) {
            output << cmd << " is a shell builtin" << "\n";
        }
        else {
            bool foundAny = false;

            if (const char* pathEnv = getenv("PATH"); pathEnv != nullptr) {
                std::stringstream ss(pathEnv);
                std::string dir;

                while (std::getline(ss, dir, ':')) {
                    std::filesystem::path candidate = std::filesystem::path(dir) / cmd;

                    if (std::filesystem::exists(candidate) && std::filesystem::is_regular_file(candidate)) {
                        if (access(candidate.c_str(), X_OK) == 0) {
                            output << cmd << " is " << candidate.string() << "\n";
                            foundAny = true;
                            break;
                        }
                    }
                }
            }

            if (!foundAny) error << cmd << ": not found" << "\n";
        }

        return std::make_pair(output.str(), error.str());
    }

    std::pair<std::string, std::string> pwd() {
        std::stringstream output, error;
        output << std::filesystem::current_path().string() << "\n";
        return std::make_pair(output.str(), error.str());
    }

    std::pair<std::string, std::string> cd(const std::string &path) {
        std::stringstream output, error;

        if (path == "~") {
            if (const char* pathEnv = getenv("HOME"); pathEnv != nullptr) {
                std::filesystem::current_path(pathEnv);
            }
            else {
                error << "cd: " << path << ": No such file or directory\n";
            }
        }
        else if (std::filesystem::exists(path)) {
            std::filesystem::current_path(path);
        }
        else {
            error << "cd: " << path << ": No such file or directory\n";
        }

        return std::make_pair(output.str(), error.str());
    }

    void writeOrCreateFile(const std::string& filePath, const std::string& content) {
        std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
        outFile << content;
        outFile.close();
    }

    void print(const std::string& content) {
        std::cout << content;
    }
} // shell
