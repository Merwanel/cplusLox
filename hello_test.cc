// cmake --build build && ctest --test-dir ./build --output-on-failure

#include <gtest/gtest.h>

#include "./chunk.cpp"
#include "./debug.cpp"

TEST(Disassembler, op) {
    // std::stringstream buffer;
    // std::streambuf * coutbuf = capture_stdout(buffer) ;
    testing::internal::CaptureStdout();

    Chunk chunk;
    uint8_t constant = addConstant(chunk, 1.2) ;
    writeChunk(chunk, OP_CONSTANT);
    writeChunk(chunk, constant);

    constant = addConstant(chunk, 65.7) ;
    writeChunk(chunk, OP_CONSTANT);
    writeChunk(chunk, constant);
    
    writeChunk(chunk, OP_RETURN);
    disassembleChunk(chunk, "test chunk");
    
    std::string output = testing::internal::GetCapturedStdout();
    std::string expected = "== test chunk ==\n"
                            "0000 OP_CONSTANT         0 '1.2'\n"
                            "0002 OP_CONSTANT         1 '65.7'\n"
                            "0004 OP_RETURN\n";
    EXPECT_EQ(output, expected);
}