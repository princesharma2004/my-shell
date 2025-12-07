/**
 * @file input.cpp
 * @brief [Brief description of the file]
 *
 * Detailed description of the file purpose and contents.
 *
 * @date 06/12/2025 A
 * @author princesharma
 */
 #include "input.h"

namespace shell {
    std::string read_input() {
        std::string command;

        std::cout << "$ ";
        std::getline(std::cin, command);

        return command;
    }

    std::tuple<std::string, std::string, std::string, std::string> break_input(const std::string& input) {
        std::string outputFile, errorFile;

        const auto& [cmd, argsStart] = clean_cmd(input);
        std::string args = (argsStart+1<input.size())? input.substr(argsStart + 1): "";

        if (size_t pos; (pos = args.find("2>")) != std::string::npos) {
            errorFile = args.substr(pos + 2);
            if (!errorFile.empty() && errorFile[0] == ' ') {
                errorFile.erase(0, 1);
            }

            args = args.substr(0, pos);
        }

        if (size_t pos; (pos = args.find("1>")) != std::string::npos) {
            outputFile = args.substr(pos + 2);
            if (!outputFile.empty() && outputFile[0] == ' ') {
                outputFile.erase(0, 1);
            }

            args = args.substr(0, pos);
        }

        if (size_t pos; (pos = args.find('>')) != std::string::npos) {
            outputFile = args.substr(pos + 1);
            if (!outputFile.empty() && outputFile[0] == ' ') {
                outputFile.erase(0, 1);
            }

            args = args.substr(0, pos);
        }
        return {cmd, args, outputFile, errorFile};
    }

    std::string clean_args(const std::string& input) {
        std::string args, arg;
        char quote = ' ';
        bool quote_start = false, spacial_value = false;
        bool escaped = false;

        for (size_t i = 0; i < input.size(); ++i) {
            char c = input[i];
            char next;
            if ((i + 1 < input.size())) {
                next = input[i + 1];
            }
            else {
                next = 0;
            }

            if (spacial_value) {
                arg += c;
                spacial_value = false;
                if (!quote_start) escaped = true;
                continue;
            }

            if (c == '\\') {
                if (quote_start) {
                    if (quote == '"') {
                        if (next == '"' || next == '\\' || next == '$' || next == '`') {
                            spacial_value = true;
                        } else {
                            arg += '\\';
                        }
                    } else {
                        arg += '\\';
                    }
                } else {
                    spacial_value = true;
                }
                continue;
            }

            if ((c == '\'' || c == '"') && !escaped) {
                if (quote_start) {
                    if (c == quote) {
                        quote_start = false;
                    } else {
                        arg += c;
                    }
                } else {
                    quote_start = true;
                    quote = c;
                }
            }
            else if (c == ' ' && !quote_start) {
                if (!arg.empty()) {
                    if (!args.empty()) args += ' ';
                    args += arg;
                    arg.clear();
                }
            }
            else {
                arg += c;
            }

            escaped = false;
        }

        if (!arg.empty()) {
            if (!args.empty()) args += ' ';
            args += arg;
        }

        return args;
    }

    std::pair<std::string, int> clean_cmd(const std::string& input) {
        std::string cmd;
        char quote = ' ';
        bool quote_start = false, spacial_value = false;
        bool escaped = false;

        for (size_t i = 0; i < input.size(); ++i) {
            const char c = input[i];
            char next;
            if ((i + 1 < input.size())) {
                next = input[i + 1];
            }
            else {
                next = 0;
            }

            if (spacial_value) {
                cmd += c;
                spacial_value = false;
                if (!quote_start) escaped = true;
                continue;
            }

            if (c == '\\') {
                if (quote_start) {
                    if (quote == '"') {
                        if (next == '"' || next == '\\' || next == '$' || next == '`') {
                            spacial_value = true;
                        } else {
                            cmd += '\\';
                        }
                    } else {
                        cmd += '\\';
                    }
                } else {
                    spacial_value = true;
                }
                continue;
            }

            if ((c == '\'' || c == '"') && !escaped) {
                if (quote_start) {
                    if (c == quote) {
                        quote_start = false;
                    } else {
                        cmd += c;
                    }
                } else {
                    quote_start = true;
                    quote = c;
                }
            }
            else if (c == ' ' && !quote_start) {
                if (!cmd.empty()) {
                    return make_pair(cmd, i);
                }
            }
            else {
                cmd += c;
            }

            escaped = false;
        }

        return std::make_pair(cmd, input.size());
    }
}
