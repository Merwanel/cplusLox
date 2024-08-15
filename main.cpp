// g++ main.cpp -o main.o && ./main.o
// or
// gcc main.cpp -lstdc++ -o main.o && ./main.o


#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "./common.hpp"

#include "./chunk.cpp"
#include "./debug.cpp"
#include "./vm.cpp"


void printChunk(const Chunk &chunk) {

    std::cout << "chunk.code = ";
    for (size_t i = 0; i < chunk.code.size(); i++)
    {
        std::cout << (int)chunk.code[i] << ", ";
    }
    std::cout << std::endl;
    std::cout << "chunk.constants = ";
    for (size_t i = 0; i < chunk.constants.size(); i++)
    {
        std::cout << chunk.constants[i] << ", ";
    }
    std::cout << std::endl;
    std::cout << "chunk.lines = ";
    for (size_t i = 0; i < chunk.lines.size(); i++)
    {
        std::cout << "{" << chunk.lines[i].offset << "," << chunk.lines[i].line << "}, ";
    }
    std::cout << std::endl;
}

void repl(VM& vm) {
    std::string line;

    for (;;) {
        std::cout << "> " ;
        if (!std::getline (std::cin, line)) {
            std::cout << std::endl;
            break;
        }
        std::cout << line << std::endl;
        interpret(vm, line);
    }
}

static std::string readFile(const std::string path) {
    std::ifstream t(path);
    if (t.fail()) {
        throw std::runtime_error("File '" + path + "' does not exist or is not openable") ;
    }
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::cout <<  buffer.str() << std::endl ; 
    return buffer.str();
}

void runFile(VM& vm, const std::string path, const bool print_scanner) {
    std::string source = readFile(path);
    if (print_scanner) {
        print_tokens(source);
        return;
    }
    InterpretResult result = interpret(vm, source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, char const *argv[]) {
    Chunk chunk;
    VM vm = initVM(chunk);
    if (argc == 1) {
        repl(vm);
    } else if (argc == 2) {
        runFile(vm, (std::string)argv[1], false);
    } else if (argc == 3 && (std::string)argv[2] == "-tokens") {
        runFile(vm, (std::string)argv[1], true);
    } else {
        std::cerr << "Usage: clox [path] \n\t-tokens : only print the tokens" << std::endl ;
        exit(64);
    }
    
    return 0;
}