// cmake --build build && ctest --test-dir ./build --output-on-failure

#include <gtest/gtest.h>

#include "./chunk.cpp"
#include "./debug.cpp"

TEST(Disassembler, op) {
    testing::internal::CaptureStdout();

    Chunk chunk;

    writeConstant(chunk, 1.2, 123);
    writeConstant(chunk, 65.7, 1234);
    
    writeChunk(chunk, OP_RETURN, 1234);

    disassembleChunk(chunk, "test chunk");
    
    std::string output = testing::internal::GetCapturedStdout();
    std::string expected = "== test chunk ==\n"
                            "0000  123 OP_CONSTANT         0 '1.2'\n"
                            "0002    | OP_CONSTANT         1 '65.7'\n"
                            "0004 1234 OP_RETURN\n";
    EXPECT_EQ(output, expected);
}
TEST(Disassembler, op_constant_long) {
    testing::internal::CaptureStdout();

    Chunk chunk;
    for(int i = 0 ; i < 263 ; i++) {
        writeConstant(chunk, i, i);
    }
    disassembleChunk(chunk, "test chunk");
    
    std::string output = testing::internal::GetCapturedStdout();
    int start_last_line = output.size()-1 ;
    while (output[--start_last_line] != '\n') ;
    std::string last_line = output.substr(start_last_line+1);

    std::string expected = "0536    | OP_CONSTANT_LONG  262 '262'\n";
    EXPECT_EQ(last_line, expected);
}