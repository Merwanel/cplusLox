#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <string>
#include <iomanip>
#include <iostream>

#include "./debug.hpp"
#include "./chunk.cpp"

/** 
 * capture only std::cout, not printf
 */
std::streambuf * capture_stdout(std::stringstream& buffer) {
    return std::cout.rdbuf(buffer.rdbuf());
}

void reset_stdout(std::streambuf * coutbuf) {
    std::cout.rdbuf(coutbuf);
}

void disassembleChunk(const Chunk& chunk, const char* name) {  
    std::cout << "== " << name << " ==" << std::endl;

    for (int offset = 0; offset < chunk.code.size();) {
        // offset is incremented by disassembleInstruction() 
        // because instructions can have different sizes 
        // (1 opcode and between 0 and many operands ).
        offset = disassembleInstruction(chunk, offset);
    }
}
  
int disassembleInstruction(const Chunk& chunk, int offset) {
    printf("%04d ", offset);
    if (offset > 0 &&
        getline(chunk, offset) == getline(chunk, offset-1)) {
        printf("   | ");
    } else {
        printf("%4d ", getline(chunk, offset));
    }

    uint8_t instruction = chunk.code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return longConstantInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
        std::cout << "Unknown opcode " << instruction << std::endl ; 
        return offset + 1;
    }
}

void printValue(Value value) {
    std::cout << value ;
}

static int constantInstruction(const char* name, const Chunk& chunk, int offset) {
    uint8_t constant = chunk.code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk.constants[constant]);
    std::cout << "'" << std::endl ;
    return offset + 2;
}

static int longConstantInstruction(const char* name, const Chunk& chunk, int offset) {
    uint32_t constant = chunk.code[offset + 1] |
                        (chunk.code[offset + 2] << 8) |
                        (chunk.code[offset + 3] << 16);
    printf("%-16s %4d '", name, constant);
    printValue(chunk.constants[constant]);
    printf("'\n");
    return offset + 4;
}

static int simpleInstruction(const char* name, int offset) {
    std::cout << name << std::endl ;
    return offset + 1;
}

#endif