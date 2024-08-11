#include "./vm.hpp"
#include "./debug.cpp"

#include "./common.hpp"

InterpretResult interpret(VM& vm) {
    return run(vm) ;
}

VM initVM(const Chunk& chunk) {
    VM vm {&chunk, 0, std::vector<Value> (STACK_MAX), 0};
    return vm;
}

void resetStack(VM& vm) {
    // vm.stackTop = vm.stack.begin() ;
}


void push(VM& vm, Value value) {
    vm.stack[vm.stackTop++] = value;;
}

Value pop(VM& vm) {
    return vm.stack[--vm.stackTop] ;
}

/// @brief dispatching instructions
/// @param vm 
/// @return InterpretResult
static InterpretResult run(VM& vm) {
    // resetStack(vm); 
    std::array<int, 3> a{1,2,3};

std::array arr = { 1, 2, 3 };

    const Chunk& chunk = *vm.chunk;
    #define READ_BYTE() (chunk.code[vm.ip++])
    #define READ_CONSTANT() (chunk.constants[READ_BYTE()])
    #define READ_CONSTANT_LONG() (chunk.constants[(uint32_t)(READ_BYTE() | (READ_BYTE() << 8) | (READ_BYTE() << 16))])

    while (vm.ip != chunk.code.size()) {
        #ifdef DEBUG_TRACE_EXECUTION
            printf("          ");
            for (auto slot = 0; slot < vm.stackTop; slot++) {
                printf("[ ");
                printValue(vm.stack[slot]);
                printf(" ]");
            }
            printf("\n");
            disassembleInstruction(chunk, vm.ip) ;
        #endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(vm, constant);
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = READ_CONSTANT_LONG();
                push(vm, constant);
                break;
            }
            case OP_RETURN: {
                printValue(pop(vm));
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
    return INTERPRET_RUNTIME_ERROR;
}
