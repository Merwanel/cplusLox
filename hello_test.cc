// cmake --build build && ctest --test-dir ./build --output-on-failure

#include <gtest/gtest.h>

#include "./chunk.cpp"
#include "./debug.cpp"
#include "./vm.cpp"

std::string TokensToStr(std::string& source , std::vector<Token>& tokens) {
    testing::internal::CaptureStdout();
    std::string str = ""; 
    int line = -1;
    for (Token token : tokens) {
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        std::string type = get_token_type_str(token.type);
        std::cout << std::setw(25) << std::left << type ;
        std::cout << "'" << source.substr(token.start, token.length)  <<  "'" << std::setw(10) << std::right << token.start << " "<< token.length <<  std::endl ;
        if (token.type == TOKEN_EOF) break;
    }
    return testing::internal::GetCapturedStdout();

}
// 1 + 2 * 3 - 4 / -5    
// TEST(Disassembler, op) {
//     testing::internal::CaptureStdout();

//     Chunk chunk;
    
//     writeConstant(chunk, 1, 1);
//     writeConstant(chunk, 2, 1);
//     writeChunk(chunk, OP_ADD, 1);
//     writeConstant(chunk, 3, 2);
//     writeChunk(chunk, OP_MULTIPLY, 2);
//     writeConstant(chunk, 4, 3);
//     writeChunk(chunk, OP_SUBTRACT, 3);
//     writeConstant(chunk, 5, 3);
//     writeChunk(chunk, OP_DIVIDE, 4);
    
//     writeChunk(chunk, OP_RETURN, 1234);

//     disassembleChunk(chunk, "test chunk");
    
//     std::string output = testing::internal::GetCapturedStdout();
//     std::string expected = "== test chunk ==\n"
//                         "0000    1 OP_CONSTANT         0 '1'\n"
//                         "0002    | OP_CONSTANT         1 '2'\n"
//                         "0004    | OP_ADD\n"
//                         "0005    | OP_CONSTANT         2 '3'\n"
//                         "0007    2 OP_MULTIPLY\n"
//                         "0008    | OP_CONSTANT         3 '4'\n"
//                         "0010    | OP_SUBTRACT\n"
//                         "0011    | OP_CONSTANT         4 '5'\n"
//                         "0013    4 OP_DIVIDE\n"
//                         "0014 1234 OP_RETURN\n";
//     EXPECT_EQ(output, expected);
// }
// TEST(Disassembler, op_constant_long) {
//     testing::internal::CaptureStdout();

//     Chunk chunk;
//     for(int i = 0 ; i < 263 ; i++) {
//         writeConstant(chunk, i, i);
//     }
//     disassembleChunk(chunk, "test chunk");
    
//     std::string output = testing::internal::GetCapturedStdout();
//     int start_last_line = output.size()-1 ;
//     while (output[--start_last_line] != '\n') ;
//     std::string last_line = output.substr(start_last_line+1);

//     std::string expected = "0536    | OP_CONSTANT_LONG  262 '262'\n";
//     EXPECT_EQ(last_line, expected);
// }
TEST(Scanner, print) {
    std::string source = "print 1 + 2;";
    Scanner scanner(source);
    std::vector<Token> tokens;
    for (;;) {
        tokens.push_back(scanner.scanToken());
        if (tokens.back().type == TOKEN_EOF) break;
    }
    std::string output = TokensToStr(source, tokens);
    
    std::vector<Token> expectedTokens{
        {TOKEN_IDENTIFIER, 0, 5, 1},
        {TOKEN_NUMBER, 6, 1, 1},
        {TOKEN_PLUS, 8, 1, 1},
        {TOKEN_NUMBER, 10, 1, 1},
        {TOKEN_SEMICOLON, 11, 1, 1},
        {TOKEN_EOF, 12, 0, 1},
    };
    std::string expected_str = TokensToStr(source, expectedTokens);
    EXPECT_EQ(output, expected_str);
}

// // 1 + 2 * 3 - 4 / -5    
// TEST(VM, binary_op) {
//     testing::internal::CaptureStdout();

//     Chunk chunk;
    
//     writeConstant(chunk, 1, 1);
//     writeConstant(chunk, 2, 1);
//     writeChunk(chunk, OP_ADD, 1);
//     writeConstant(chunk, 3, 2);
//     writeChunk(chunk, OP_MULTIPLY, 2);
//     writeConstant(chunk, 4, 3);
//     writeChunk(chunk, OP_SUBTRACT, 3);
//     writeConstant(chunk, 5, 3);
//     writeChunk(chunk, OP_DIVIDE, 4);
    
//     writeChunk(chunk, OP_RETURN, 1234);

//     VM vm = initVM(chunk);
//     interpret(vm);
//     std::string output = testing::internal::GetCapturedStdout();
//     std::string expected = "1\n";
//     EXPECT_EQ(output, expected);
// }