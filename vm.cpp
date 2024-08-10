#include "./vm.hpp"
#include "./debug.cpp"

InterpretResult interpret(VM& vm) {
    return run(vm) ;
}


/// @brief dispatching instructions
/// @param vm 
/// @return InterpretResult
static InterpretResult run(VM& vm) {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk.constants[READ_BYTE()])
#define READ_CONSTANT_LONG() (vm.chunk.constants[(uint32_t)(READ_BYTE() | (READ_BYTE() << 8) | (READ_BYTE() << 16))])

    while (vm.ip != vm.chunk.code.end()) {
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                printValue(constant);
                printf("\n");
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = READ_CONSTANT_LONG();
                printValue(constant);
                printf("\n");
                break;
            }
            case OP_RETURN: {
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
    return INTERPRET_RUNTIME_ERROR;
}