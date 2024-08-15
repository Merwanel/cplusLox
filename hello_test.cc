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
        {TOKEN_PRINT, 0, 5, 1},
        {TOKEN_NUMBER, 6, 1, 1},
        {TOKEN_PLUS, 8, 1, 1},
        {TOKEN_NUMBER, 10, 1, 1},
        {TOKEN_SEMICOLON, 11, 1, 1},
        {TOKEN_EOF, 12, 0, 1},
    };
    std::string expected_str = TokensToStr(source, expectedTokens);
    EXPECT_EQ(output, expected_str);
}
TEST(Scanner, fun) {
    std::string source = R"x(var a = 9 ;)x""\r\n"
                        R"x(fun sayHi(first, last) {)x""\r\n"
                        R"x(    print "Hi, " + first + " " + last + "!";)x""\r\n"
                        R"x(    print 5 + 8 + a ;)x""\r\n"
                        "}\r\n"
                        "\r\n"
                        R"x(sayHi("Dear", "Reader");)x";
    Scanner scanner(source);
    std::vector<Token> tokens;
    for (;;) {
        tokens.push_back(scanner.scanToken());
        if (tokens.back().type == TOKEN_EOF) break;
    }
    std::string output = TokensToStr(source, tokens);

    std::vector<Token> expectedTokens{
        {TOKEN_VAR,0,3,1} ,
            {TOKEN_IDENTIFIER,4,1,1} ,
            {TOKEN_EQUAL_EQUAL,6,2,1} ,
            {TOKEN_NUMBER,8,1,1} ,
            {TOKEN_SEMICOLON,10,1,1} ,
            {TOKEN_FUN,13,3,2} ,
            {TOKEN_IDENTIFIER,17,5,2} ,
            {TOKEN_LEFT_PAREN,22,1,2} ,
            {TOKEN_IDENTIFIER,23,5,2} ,
            {TOKEN_COMMA,28,1,2} ,
            {TOKEN_IDENTIFIER,30,4,2} ,
            {TOKEN_RIGHT_PAREN,34,1,2} ,
            {TOKEN_LEFT_BRACE,36,1,2} ,
            {TOKEN_PRINT,43,5,3} ,
            {TOKEN_STRING,49,6,3} ,
            {TOKEN_PLUS,56,1,3} ,
            {TOKEN_IDENTIFIER,58,5,3} ,
            {TOKEN_PLUS,64,1,3} ,
            {TOKEN_STRING,66,3,3} ,
            {TOKEN_PLUS,70,1,3} ,
            {TOKEN_IDENTIFIER,72,4,3} ,
            {TOKEN_PLUS,77,1,3} ,
            {TOKEN_STRING,79,3,3} ,
            {TOKEN_SEMICOLON,82,1,3} ,
            {TOKEN_PRINT,89,5,4} ,
            {TOKEN_NUMBER,95,1,4} ,
            {TOKEN_PLUS,97,1,4} ,
            {TOKEN_NUMBER,99,1,4} ,
            {TOKEN_PLUS,101,1,4} ,
            {TOKEN_IDENTIFIER,103,1,4} ,
            {TOKEN_SEMICOLON,105,1,4} ,
            {TOKEN_RIGHT_BRACE,108,1,5} ,
            {TOKEN_IDENTIFIER,113,5,7} ,
            {TOKEN_LEFT_PAREN,118,1,7} ,
            {TOKEN_STRING,119,6,7} ,
            {TOKEN_COMMA,125,1,7} ,
            {TOKEN_STRING,127,8,7} ,
            {TOKEN_RIGHT_PAREN,135,1,7} ,
            {TOKEN_SEMICOLON,136,1,7} ,
            {TOKEN_EOF,137,0,7} ,
    };
    std::string expected_str = TokensToStr(source, expectedTokens);
    EXPECT_EQ(output, expected_str);
}
TEST(Scanner, reserved_keywords) {
    std::string source = "and class else false for fun if nil or\r\n"
                        "print return super this true var while";
    Scanner scanner(source);
    std::vector<Token> tokens;
    for (;;) {
        tokens.push_back(scanner.scanToken());
        if (tokens.back().type == TOKEN_EOF) break;
    }
    std::string output = TokensToStr(source, tokens);

    std::vector<Token> expectedTokens{
        {TOKEN_AND,0,3,1} ,
        {TOKEN_CLASS,4,5,1} ,
        {TOKEN_ELSE,10,4,1} ,
        {TOKEN_FALSE,15,5,1} ,
        {TOKEN_FOR,21,3,1} ,
        {TOKEN_FUN,25,3,1} ,
        {TOKEN_IF,29,2,1} ,
        {TOKEN_NIL,32,3,1} ,
        {TOKEN_OR,36,2,1} ,
        {TOKEN_PRINT,40,5,2} ,
        {TOKEN_RETURN,46,6,2} ,
        {TOKEN_SUPER,53,5,2} ,
        {TOKEN_THIS,59,4,2} ,
        {TOKEN_TRUE,64,4,2} ,
        {TOKEN_VAR,69,3,2} ,
        {TOKEN_WHILE,73,5,2} ,
        {TOKEN_EOF,78,0,2} ,
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