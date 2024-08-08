// chunk refer to sequences of bytecode

#ifndef CHUNK_H
#define CHUNK_H

#include <cstdint>
#include <vector>

#include "./value.hpp" 

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode; // operation codes


typedef struct {
    std::vector<uint8_t> code;
    ValueArray constants;
} Chunk;

#endif