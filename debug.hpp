#include "./chunk.hpp"

std::streambuf * capture_stdout(std::stringstream& buffer) ;

void reset_stdout(std::streambuf * coutbuf) ;

void disassembleChunk(Chunk& chunk, const char* name) ;
  
int disassembleInstruction(Chunk& chunk, int offset) ;

static int simpleInstruction(const char* name, int offset) ;