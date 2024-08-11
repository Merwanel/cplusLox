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
    
    for(int i = 0 ; i < 4 ; i++) {
        writeConstant(chunk, i, i);
    }
    
    disassembleChunk(chunk, "test chunk");
    // printChunk(chunk);
    VM vm ;
    interpret(vm, chunk);
   
    return 0;
}