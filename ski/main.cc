#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/tokenizer.h"
#include "../include/parser.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: ski <ski-program-path>\n";
    return 1;
  }
  std::string ski_prog_path = argv[1];
  std::filesystem::path path(ski_prog_path);
  std::string ski_filename = path.filename().string();

  std::ifstream ski_prog_fs(ski_prog_path);
  if (!ski_prog_fs) {
    std::cerr << "Failed to open file: " << ski_prog_path << "\n";
    return 1;
  }

  std::stringstream buffer;
  buffer << ski_prog_fs.rdbuf();

  Ski::Tokenizer tokenizer(buffer.str(), ski_filename);

  Ski::Parser parser(std::move(tokenizer.tokenize()), ski_filename);
  auto ski_ast = parser.parse();

  std::cout << static_cast<std::string>(*ski_ast) << "\n";
  std::cout << "SKI Interpreter\n";
}
