#ifndef VM_H
#define VM_H

#include <vector>
#include <array>

#include "./chunk.hpp"
#include <memory>

#define STACK_MAX 256

typedef struct {
    const Chunk* chunk;
    int ip ;  //  instruction pointer, points to the next instruction
    std::vector<Value> stack;
    int stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

VM initVM(const Chunk& chunk);

void freeVM(VM& vm);

InterpretResult interpret(VM& vm);

static InterpretResult run(VM& vm) ;

void push(VM& vm, Value value) ;

Value pop(VM& vm) ;

#endif