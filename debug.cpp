#include <stdio.h>
#include <string>
#include <iomanip>

#include "./debug.hpp"

/** 
 * capture only std::cout, not printf
 */
std::streambuf * capture_stdout(std::stringstream& buffer) {
    return std::cout.rdbuf(buffer.rdbuf());
}

void reset_stdout(std::streambuf * coutbuf) {
    std::cout.rdbuf(coutbuf);
}

void disassembleChunk(Chunk& chunk, const char* name) {  
    std::cout << "== " << name << " ==" << std::endl;

    for (int offset = 0; offset < chunk.size();) {
        // offset is incremented by disassembleInstruction() because instructions can have different sizes.
        offset = disassembleInstruction(chunk, offset);
    }
}
  
int disassembleInstruction(Chunk& chunk, int offset) {
    std::cout <<  std::setfill('0') << std::setw(4) << offset << " " ;
    OpCode instruction = chunk[offset];
    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
        std::cout << "Unknown opcode " << instruction << std::endl ; 
        return offset + 1;
    }
}

static int simpleInstruction(const char* name, int offset) {
  std::cout << name << std::endl ;
  return offset + 1;
}
