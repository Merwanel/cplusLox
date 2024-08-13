#include <string>
#include "./scanner.cpp"
#include <iostream>

void compile(const std::string source) {
    Scanner scanner(source);
    int line = -1;
    for (;;) {
        Token token = scanner.scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        printf("%2d ", token.type);
        std::cout << "'" << scanner.source.substr(token.length, token.start)  <<  "'" <<  std::endl ;
        if (token.type == TOKEN_EOF) break;
    }
}