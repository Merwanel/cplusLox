#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <string>
#include <iomanip>
#include <iostream>

#include "./debug.hpp"
#include "./chunk.cpp"
#include "./scanner.cpp"


/** 
 * capture only std::cout, not printf
 */
std::streambuf * capture_stdout(std::stringstream& buffer) {
    return std::cout.rdbuf(buffer.rdbuf());
}

void reset_stdout(std::streambuf * coutbuf) {
    std::cout.rdbuf(coutbuf);
}

void disassembleChunk(const Chunk& chunk, const char* name) {  
    std::cout << "== " << name << " ==" << std::endl;

    for (int offset = 0; offset < chunk.code.size();) {
        // offset is incremented by disassembleInstruction() 
        // because instructions can have different sizes 
        // (1 opcode and between 0 and many operands ).
        offset = disassembleInstruction(chunk, offset);
    }
}
  
int disassembleInstruction(const Chunk& chunk, int offset) {
    printf("%04d ", offset);
    if (offset > 0 &&
        getline(chunk, offset) == getline(chunk, offset-1)) {
        printf("   | ");
    } else {
        printf("%4d ", getline(chunk, offset));
    }

    uint8_t instruction = chunk.code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return longConstantInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
        std::cout << "Unknown opcode " << instruction << std::endl ; 
        return offset + 1;
    }
}

void printValue(Value value) {
    std::cout << value ;
}

static int constantInstruction(const char* name, const Chunk& chunk, int offset) {
    uint8_t constant = chunk.code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk.constants[constant]);
    std::cout << "'" << std::endl ;
    return offset + 2;
}

static int longConstantInstruction(const char* name, const Chunk& chunk, int offset) {
    uint32_t constant = chunk.code[offset + 1] |
                        (chunk.code[offset + 2] << 8) |
                        (chunk.code[offset + 3] << 16);
    printf("%-16s %4d '", name, constant);
    printValue(chunk.constants[constant]);
    printf("'\n");
    return offset + 4;
}

static int simpleInstruction(const char* name, int offset) {
    std::cout << name << std::endl ;
    return offset + 1;
}

std::string get_token_type_str(TokenType type) {
    #define GET(p) case(p): return #p;
    switch (type) {
        GET(TOKEN_LEFT_PAREN);GET( TOKEN_RIGHT_PAREN); GET(TOKEN_LEFT_BRACE); GET( TOKEN_RIGHT_BRACE); GET(TOKEN_COMMA); GET( TOKEN_DOT); GET( TOKEN_MINUS); GET( TOKEN_PLUS); GET(TOKEN_SEMICOLON); GET( TOKEN_SLASH); GET( TOKEN_STAR); GET(TOKEN_BANG); GET( TOKEN_BANG_EQUAL); GET( TOKEN_EQUAL); GET( TOKEN_EQUAL_EQUAL); GET( TOKEN_GREATER); GET( TOKEN_GREATER_EQUAL); GET( TOKEN_LESS); GET( TOKEN_LESS_EQUAL); GET( TOKEN_IDENTIFIER); GET( TOKEN_STRING); GET( TOKEN_NUMBER); GET( TOKEN_AND); GET( TOKEN_CLASS); GET( TOKEN_ELSE); GET( TOKEN_FALSE); GET(TOKEN_FOR); GET( TOKEN_FUN); GET( TOKEN_IF); GET( TOKEN_NIL); GET( TOKEN_OR); GET( TOKEN_PRINT); GET( TOKEN_RETURN); GET( TOKEN_SUPER); GET( TOKEN_THIS); GET(TOKEN_TRUE); GET( TOKEN_VAR); GET( TOKEN_WHILE); GET( TOKEN_ERROR); GET( TOKEN_EOF);
    }
    #undef GET

    return "";
}

void print_tokens(const std::string source) {
    Scanner scanner(source);
    int line = -1;
    for (;;) {
        Token token = scanner.scanToken();
        std::string type = get_token_type_str(token.type);
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

#endif