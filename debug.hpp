#include "./chunk.hpp"

std::streambuf * capture_stdout(std::stringstream& buffer) ;

void reset_stdout(std::streambuf * coutbuf) ;

void disassembleChunk(const Chunk& chunk, const char* name) ;
  
int disassembleInstruction(const Chunk& chunk, int offset) ;

void printValue(Value value);

static int simpleInstruction(const char* name, int offset) ;

static int longConstantInstruction(const char* name, const Chunk& chunk, int offset) ;

static int constantInstruction(const char* name, const Chunk& chunk, int offset);
