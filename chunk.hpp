// chunk refer to sequences of bytecode

#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <cstdint>
#include <vector>

#include "./value.hpp" 

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode; // operation codes


typedef struct {
    int offset;
    int line;
} LineArray ;

typedef struct {
    std::vector<uint8_t> code;
    std::vector<LineArray> lines;
    ValueArray constants;
} Chunk;

int addConstant(Chunk& chunk, Value value)  ;

void writeChunk(Chunk& chunk, const uint8_t byte, const int line) ;

int getline(const Chunk& chunk, const uint8_t instruction) ;

#endif
