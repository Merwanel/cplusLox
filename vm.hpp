#ifndef VM_H
#define VM_H

#include <vector>

#include "./chunk.hpp"
#include <memory>

typedef struct {
    Chunk& chunk;
    std::vector<uint8_t>::const_iterator ip ;  //  instruction pointer, points to the next instruction
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();

void freeVM();

InterpretResult interpret(VM& vm, const Chunk& chunk);

static InterpretResult run(VM& vm) ;

#endif