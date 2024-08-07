// cmake --build build && ctest --test-dir ./build

#include <gtest/gtest.h>

#include "./debug.cpp"

TEST(Disassembler, Basic) {
    std::stringstream buffer;
    std::streambuf * coutbuf = capture_stdout(buffer) ;

    Chunk chunk;
    chunk.push_back(OP_RETURN);
    disassembleChunk(chunk, "test chunk");
    
    reset_stdout(coutbuf);
    
    EXPECT_EQ(buffer.str(), "== test chunk ==\n0000 OP_RETURN\n");
}