#include <fstream>
#include <iostream>
#include <sstream>

#include "lexer.h"

namespace lynx {

    void print_usage() {
        std::cout << "Usage: lync <source_file.lnx>\n";
    }

    std::string get_file_content(const std::string& path) {
        std::stringstream buffer;
        std::ifstream file{path};
        if(!file.good()) {
            std::cerr << "Error: File \"" << path << "\" not found. Exiting...\n";
            return "";
        }
        while(!file.eof()) {
            std::string line{};
            std::getline(file, line);
            buffer << line << '\n';
        }
        return buffer.str();
    }

}

int main(int argc, char** argv) {
    if(argc != 2) {
        lynx::print_usage();
        return 0;
    }
    auto code = lynx::get_file_content(argv[1]);
    if(code == "") {
        return 1;
    }
    lynx::Lexer lexer{std::move(code)};
    if(const auto errors_reported = lexer.errors_reported()) {
        std::cout << "Reported " << errors_reported << ". Exiting...\n";
        return 2;
    }
    return 0;
}

