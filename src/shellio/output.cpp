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
    std::string echo(const std::string& args) {
        std::stringstream output;
        output << args << "\n";
        return output.str();
    }

    std::string not_found(const std::string& cmd, const std::string& args) {
        std::stringstream output;
        bool foundAny = false;

        if (const char* pathEnv = getenv("PATH")) {
            std::stringstream ss(pathEnv);
            std::string dir;

            while (std::getline(ss, dir, ':')) {
                std::filesystem::path candidate = std::filesystem::path(dir) / cmd;

                if (std::filesystem::exists(candidate) &&
                    std::filesystem::is_regular_file(candidate) &&
                    access(candidate.c_str(), X_OK) == 0) {
                    if (cmd=="cat" || cmd=="ls") {
                        const std::string command = cmd + ' ' += args;

                        std::array<char, 256> buffer{};
                        FILE* pipe = popen(command.c_str(), "r");

                        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                            output << buffer.data();
                        }
                        pclose(pipe);
                    }
                    else {
                        output << run_exec_with_args(cmd, args);
                    }
                    foundAny = true;
                    break;
                }
            }
        }

        if (!foundAny) {
            std::cout << cmd << ": command not found\n";
        }

        return output.str();
    }

    std::string type(const std::unordered_map<std::string, std::function<std::string(const std::string&)>>& cmds, const std::string& cmd) {
        std::stringstream output;

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

            if (!foundAny) std::cout << cmd << ": not found" << "\n";
        }

        return output.str();
    }

    std::string pwd() {
        std::stringstream output;
        output << std::filesystem::current_path().string() << "\n";
        return output.str();
    }

    std::string cd(const std::string& path) {
        const std::stringstream output;

        if (path == "~") {
            if (const char* pathEnv = getenv("HOME"); pathEnv != nullptr) {
                std::filesystem::current_path(pathEnv);
            }
            else {
                std::cout << "cd: " << path << ": No such file or directory\n";
            }
        }
        else if (std::filesystem::exists(path)) {
            std::filesystem::current_path(path);
        }
        else {
            std::cout << "cd: " << path << ": No such file or directory\n";
        }

        return output.str();
    }


    std::string run_exec_with_args(const std::string& path, const std::string& args) {
        std::stringstream output;

        int fds[2];
        pipe(fds);

        const pid_t pid = fork();

        if (pid == 0) {
            close(fds[0]);
            dup2(fds[1], STDOUT_FILENO);
            dup2(fds[1], STDERR_FILENO);
            close(fds[1]);

            std::vector<std::string> parts;
            for (auto part_view : std::views::split(args, ' ')) {
                parts.emplace_back(part_view.begin(), part_view.end());
            }

            std::vector<char*> argv;
            argv.push_back(const_cast<char*>(path.c_str()));
            for (auto& p : parts) argv.push_back(const_cast<char*>(p.c_str()));
            argv.push_back(nullptr);

            execvp(path.c_str(), argv.data());
            _exit(127);
        }

        close(fds[1]);

        char buffer[256];
        ssize_t n;

        while ((n = read(fds[0], buffer, sizeof(buffer))) > 0) {
            output.write(buffer, n);
        }

        close(fds[0]);
        waitpid(pid, nullptr, 0);

        return output.str();
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
