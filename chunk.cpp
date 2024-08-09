
#ifndef CHUNK_CPP
#define CHUNK_CPP
#include "./chunk.hpp"
#include <iostream>

int addConstant(Chunk& chunk, Value value) {
    chunk.constants.push_back(value);
    return chunk.constants.size() - 1;
}


/// @param byte OpCode or operands
/// @param line coresponding line of the opcode in the code source
void writeChunk(Chunk& chunk, const uint8_t byte, const int line) {
    chunk.code.push_back(byte);
    if (chunk.lines.size() == 0 || chunk.lines.back().line != line) {
        int last_offset = chunk.lines.size() > 0 ? chunk.lines.back().offset : -1;
        chunk.lines.push_back(LineArray {last_offset, line});
    }
    chunk.lines.back().offset += 1;
}

int getline(const Chunk& chunk, const uint8_t instruction) {
    // std::cout << "getline:" << (int)instruction << std::endl ;
    int low = 0, high = chunk.lines.size()-1 ;
    int mid, best = low;
    while (low <= high) {
        mid = low + (high - low) / 2;
        // std::cout << "(" << low << "," << high << ")" << "," << mid << std::endl ;
        if (chunk.lines[mid].offset <= instruction) {
            best = mid;
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
        // std::cout << best << std::endl ;
    return chunk.lines[best].line;
}
#endif