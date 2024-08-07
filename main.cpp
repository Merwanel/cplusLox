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

#include "./chunk.hpp"
#include "./debug.cpp"

int main(int argc, char const *argv[]) {
    std::stringstream buffer;
    // coutbuf = std::cout.rdbuf(buffer.rdbuf());
    std::streambuf * coutbuf = capture_stdout(buffer) ;

    Chunk chunk;
    chunk.push_back(OP_RETURN);
    disassembleChunk(chunk, "test chunk");
    
    reset_stdout(coutbuf);
    std::cout << "---------" << std::endl;
    std::cout << buffer.str();
    std::cout << "---------" << std::endl;

    return 0;
}