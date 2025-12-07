/**
 * @file input.h
 * @brief [Brief description of the file]
 *
 * Detailed description of the file purpose and contents.
 *
 * @date 06/12/2025 A
 * @author princesharma
 */
 #ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>


namespace shell {
    std::pair<std::string, std::string> read_input();

    std::string clean_args(const std::string& input);
    std::pair<std::string, int> clean_cmd(const std::string& input);
} // shell

#endif //INPUT_H
