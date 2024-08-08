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

int main(int argc, char const *argv[]) {
    Chunk chunk;
    uint8_t constant = addConstant(chunk, 1.2) ;
    writeChunk(chunk, OP_CONSTANT);
    writeChunk(chunk, constant);

    constant = addConstant(chunk, 65.7) ;
    writeChunk(chunk, OP_CONSTANT);
    writeChunk(chunk, constant);
    
    writeChunk(chunk, OP_RETURN);
    disassembleChunk(chunk, "test chunk");
    
    return 0;
}