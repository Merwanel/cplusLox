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
int main(int argc, char const *argv[]) {
    Chunk chunk;
    writeConstant(chunk, 1, 1);
    writeConstant(chunk, 2, 1);
    writeChunk(chunk, OP_ADD, 1);
    writeConstant(chunk, 3, 2);
    writeChunk(chunk, OP_MULTIPLY, 2);
    writeConstant(chunk, 4, 3);
    writeChunk(chunk, OP_SUBTRACT, 3);
    writeConstant(chunk, 5, 3);
    writeChunk(chunk, OP_DIVIDE, 4);
    
    writeChunk(chunk, OP_RETURN, 1234);
    
    disassembleChunk(chunk, "test chunk");
    VM vm = initVM(chunk);
    interpret(vm);
   
    return 0;
}