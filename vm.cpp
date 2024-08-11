#include "./vm.hpp"
#include "./debug.cpp"

#include "./common.hpp"

InterpretResult interpret(VM& vm, const Chunk& chunk) {
    vm = {&chunk, chunk.code.cbegin()};
    return run(vm) ;
}


/// @brief dispatching instructions
/// @param vm 
/// @return InterpretResult
static InterpretResult run(VM& vm) {
    
    const Chunk& chunk = *vm.chunk;
    #define READ_BYTE() (*vm.ip++)
    #define READ_CONSTANT() (chunk.constants[READ_BYTE()])
    #define READ_CONSTANT_LONG() (chunk.constants[(uint32_t)(READ_BYTE() | (READ_BYTE() << 8) | (READ_BYTE() << 16))])

    while (vm.ip != chunk.code.end()) {
        #ifdef DEBUG_TRACE_EXECUTION
            disassembleInstruction(chunk, std::distance(chunk.code.begin(), vm.ip)) ;
        #endif

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