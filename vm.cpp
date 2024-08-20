#include "./vm.hpp"
#include "./debug.cpp"
#include "./compiler.cpp"


InterpretResult interpret(VM& vm, const std::string& source) {
    Chunk chunk;
    Compiler compiler(source, chunk) ;

    if (!compiler.compile()) {
        return INTERPRET_COMPILE_ERROR;
    }
    vm.chunk = &chunk;
    InterpretResult result = run(vm);

    return result;
}

VM initVM(const Chunk& chunk) {
    VM vm {&chunk, 0, std::vector<Value>(STACK_MAX), 0};
    return vm;
}

void resetStack(VM& vm) {
    // vm.stackTop = vm.stack.begin() ;
}


void push(VM& vm, Value value) {
    if (vm.stackTop == vm.stack.size()) {
        vm.stack.resize(vm.stack.size() * 2);
    }
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

    const Chunk& chunk = *vm.chunk;
    #define READ_BYTE() (chunk.code[vm.ip++])
    #define READ_CONSTANT() (chunk.constants[READ_BYTE()])
    #define READ_CONSTANT_LONG() (chunk.constants[(uint32_t)(READ_BYTE() | (READ_BYTE() << 8) | (READ_BYTE() << 16))])
    #define BINARY_OP(op) \
        do { \
        double b = pop(vm); \
        double a = pop(vm); \
        push(vm, a op b); \
        } while (false) // trick to put several statement in a macro

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

        switch (uint8_t instruction = READ_BYTE()) {
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
            case OP_NEGATE:   {
                push(vm, -pop(vm));  // TODO negate direclty number => measure performance
                break;
            }
            case OP_ADD:      BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE:   BINARY_OP(/); break;
            case OP_RETURN: {
                printValue(pop(vm));
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef READ_CONSTANT_LONG
    #undef BINARY_OP
    return INTERPRET_RUNTIME_ERROR;
}
