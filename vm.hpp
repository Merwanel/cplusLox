#ifndef VM_H
#define VM_H

#include <vector>

#include "./chunk.hpp"
#include <memory>

typedef struct {
    const Chunk* chunk;
    std::vector<uint8_t>::const_iterator ip ;  //  instruction pointer, points to the next instruction
    std::vector<Value> stack;
    std::vector<Value>::iterator stackTop;

} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();

void freeVM();

InterpretResult interpret(VM& vm);

static InterpretResult run(VM& vm) ;

#endif