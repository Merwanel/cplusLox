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

#include "./chunk.cpp"
#include "./debug.cpp"

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
    uint8_t constant = addConstant(chunk, 1.2) ;
    writeChunk(chunk, OP_CONSTANT, 123);
    writeChunk(chunk, constant, 123);

    constant = addConstant(chunk, 65.7) ;
    writeChunk(chunk, OP_CONSTANT, 123);
    writeChunk(chunk, constant, 123);

    writeChunk(chunk, OP_RETURN, 124);
    printChunk(chunk);

    disassembleChunk(chunk, "test chunk");
    
    return 0;
}