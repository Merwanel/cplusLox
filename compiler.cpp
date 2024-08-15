#include <string>
#include "./scanner.cpp"
#include <iostream>
#include <iomanip>

std::string get_token_type_str2(TokenType type) {
    #define GET(p) case(p): return #p;
    switch (type) {
        GET(TOKEN_LEFT_PAREN);GET( TOKEN_RIGHT_PAREN); GET(TOKEN_LEFT_BRACE); GET( TOKEN_RIGHT_BRACE); GET(TOKEN_COMMA); GET( TOKEN_DOT); GET( TOKEN_MINUS); GET( TOKEN_PLUS); GET(TOKEN_SEMICOLON); GET( TOKEN_SLASH); GET( TOKEN_STAR); GET(TOKEN_BANG); GET( TOKEN_BANG_EQUAL); GET( TOKEN_EQUAL); GET( TOKEN_EQUAL_EQUAL); GET( TOKEN_GREATER); GET( TOKEN_GREATER_EQUAL); GET( TOKEN_LESS); GET( TOKEN_LESS_EQUAL); GET( TOKEN_IDENTIFIER); GET( TOKEN_STRING); GET( TOKEN_NUMBER); GET( TOKEN_AND); GET( TOKEN_CLASS); GET( TOKEN_ELSE); GET( TOKEN_FALSE); GET(TOKEN_FOR); GET( TOKEN_FUN); GET( TOKEN_IF); GET( TOKEN_NIL); GET( TOKEN_OR); GET( TOKEN_PRINT); GET( TOKEN_RETURN); GET( TOKEN_SUPER); GET( TOKEN_THIS); GET(TOKEN_TRUE); GET( TOKEN_VAR); GET( TOKEN_WHILE); GET( TOKEN_ERROR); GET( TOKEN_EOF);
    }
    #undef GET

    return "";
}
void compile(const std::string& source) {
    Scanner scanner(source);
    int line = -1;
    for (;;) {
        Token token = scanner.scanToken();
        std::string type = get_token_type_str2(token.type);
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        std::cout << std::setw(25) << std::left << type ;
        std::cout << "'" << scanner.source.substr(token.start, token.length)  <<  "'\t" << token.start << " "<< token.length <<  std::endl ;
        // std::cout << "{" << type << "," << token.start << "," << token.length << "," << token.line <<"} ," << std::endl ;
        if (token.type == TOKEN_EOF) break;
    }
    
}
