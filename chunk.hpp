// chunk refer to sequences of bytecode

#ifndef CHUNK_H
#define CHUNK_H

#include <cstdint>
#include <vector>

typedef enum {
    OP_RETURN,
} OpCode; // operation codes


typedef std::vector<OpCode> Chunk;

#endif