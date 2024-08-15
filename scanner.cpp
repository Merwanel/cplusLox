#ifndef SCANNER_CPP
#define SCANNER_CPP

#include <string>
#include "./scanner.hpp"

class Scanner {
    public:
    int start=0;  // start of the current lexeme
    int current=0;  // currrent character in the lexeme
    int line=1;  // for error reporting
    const std::string& source;

    Scanner( const std::string& s): source(s) {}
    
    bool isAtEnd() {
        return current == source.size();
    }
    char advance() {
        return source[current++];
    }
    char peek() {
        return source[current];
    }
    char peekNext() {
        if (isAtEnd()) return '\0';
        return source[current];
    }
    Token makeToken(TokenType type) {
        return Token {type, start, current - start, line};
    }
    Token errorToken(std::string message) {
        Token token;
        token.type = TOKEN_ERROR;
        token.str = message;
        return token;
    }
    bool match(const char type) {
        if( isAtEnd() || source[current] == type) return false;
        current++ ;
        return true;
    }
    void skipWhitespace() {
        for (;;) {
            char c = peek();
            switch (c) {
                case ' ':
                case '\r':
                case '\t':
                    advance();
                    break;
                case '\n':
                    line ++;
                    advance();
                    break;
                case '/':
                    if (peekNext() == '/') {
                        // A comment goes until the end of the line.
                        while (peek() != '\n' && !isAtEnd()) advance();
                    } else {
                        return;
                    }
                    break;
                default:
                    return;
            }
        }
    }
    Token str() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') line++;
            advance();
        }

        if (isAtEnd()) return errorToken("Unterminated string.");

        // The closing quote.
        advance();
        return makeToken(TOKEN_STRING);
    }
    Token number() {
        while (isdigit(peek())) advance();

        // Look for a fractional part.
        if (peek() == '.' && isdigit(peekNext())) {
            // Consume the ".".
            advance();

            while (isdigit(peek())) advance();
        }

        return makeToken(TOKEN_NUMBER);
    }
    TokenType checkKeyword(int startToken, int lengthToken, const std::string rest, TokenType type) {       
        if (current - start == startToken + lengthToken &&
            source.compare(start + startToken, lengthToken, rest) == 0) {
            return type;
        }
        return TOKEN_IDENTIFIER;
    }
    
    // implementation of a trie
    TokenType identifierType() {
        switch (source[start]) {
            case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
            case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
            case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
            case 'f':
                if (current - start > 1) {
                    switch (source[start+1]) {
                        case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                        case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                        case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                    }
                }
                break;
            case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
            case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
            case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
            case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
            case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
            case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
            case 't':
                if (current - start > 1) {
                    switch (source[start+1]) {
                        case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                        case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                    }
                }
                break;
            case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
            case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
        }
        return TOKEN_IDENTIFIER;
    }

    Token identifier() {
        while (isAlpha(peek()) || isdigit(peek())) advance();
        
        return makeToken(identifierType());
    }
    bool isAlpha(char c) {
        return c == '_' || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ;
    }
    Token scanToken() {
        skipWhitespace();
        start = current;

        if (isAtEnd()) return makeToken(TOKEN_EOF);
        char c = advance();
        if (isAlpha(c)) return identifier() ;
        if (isdigit(c)) return number() ;

        switch (c) {
            case '(': return makeToken(TOKEN_LEFT_PAREN);
            case ')': return makeToken(TOKEN_RIGHT_PAREN);
            case '{': return makeToken(TOKEN_LEFT_BRACE);
            case '}': return makeToken(TOKEN_RIGHT_BRACE);
            case ';': return makeToken(TOKEN_SEMICOLON);
            case ',': return makeToken(TOKEN_COMMA);
            case '.': return makeToken(TOKEN_DOT);
            case '-': return makeToken(TOKEN_MINUS);
            case '+': return makeToken(TOKEN_PLUS);
            case '/': return makeToken(TOKEN_SLASH);
            case '*': return makeToken(TOKEN_STAR);
            case '!':
                return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
            case '=':
                return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
            case '<':
                return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
            case '>':
                return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
            case '"':
                return str();
        }

        return errorToken("Unexpected character.");
    }
};

#endif