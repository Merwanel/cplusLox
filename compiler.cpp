#include <string>
#include "./scanner.cpp"
#include <iostream>
#include <iomanip>
#include "./chunk.hpp"

typedef struct {
    Token current;
    Token previous;
    bool hadError=false;
    bool panicMode=false;
} Parser;

typedef enum {
    // from lowest to highest precedence
    PREC_NONE,
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
} Precedence;


typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

class Compiler {
    public:
    Scanner scanner;
    Chunk& chunk ;
    Parser* parser; 


    Compiler(const std::string& source, Chunk& chunk) : scanner(source), chunk(chunk) {}

    bool compile() {
        advance() ;
        expression();
        consume(TOKEN_EOF, "Expect end of expression.");
        endCompiler();
        return !parser->hadError ;
    }
    
    void advance() {
        parser->previous = parser->current;

        for (;;) {
            parser->current = scanner.scanToken();
            if (parser->current.type != TOKEN_ERROR) break;

            errorAtCurrent(parser->current.str);
        }
    }
    // advance if the right type is found or print an error
    void consume(const TokenType& type, const std::string& message) {
        if (parser->current.type == type) {
            advance();
            return;
        }

        errorAtCurrent(message);
    }
    void emitByte(uint8_t byte) {writeChunk(currentChunk(), byte, parser->previous.line);}
    void emitBytes(uint8_t byte1, uint8_t byte2) {
        emitByte(byte1);
        emitByte(byte2);
    }
    void emitConstant(Value value) {
        writeConstant(currentChunk(), value, parser->previous.line);
    }
    Chunk& currentChunk() {return chunk;}
    void endCompiler() {emitReturn();}
    
    void emitReturn() {emitByte(OP_RETURN);}
    void expression() {parsePrecedence(PREC_ASSIGNMENT);}
    void number() {
        double value = strtod(scanner.source.c_str() + parser->previous.start, NULL);
        emitConstant(value);
    }
    void grouping() {
        expression();
        consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
    }
    void binary() {
        TokenType operatorType = parser->previous.type;
        ParseRule rule = getRule(operatorType);
        // one higher level of precedence for the right operand because the binary operators are left-associative.
        // Given a series of the same operator, like:
        // 1 + 2 + 3 + 4
        // is going to be parsed as 
        // ((1 + 2) + 3) + 4
        parsePrecedence((Precedence) (rule.precedence + 1));

        switch (operatorType) {
            case TOKEN_PLUS : emitByte(OP_ADD); break;
            case TOKEN_MINUS : emitByte(OP_SUBTRACT); break;
            case TOKEN_STAR : emitByte(OP_MULTIPLY); break;
            case TOKEN_SLASH : emitByte(OP_DIVIDE); break;
        }
    }
    void unary() {
        TokenType operatorType = parser->previous.type;

        // Compile the operand.
        parsePrecedence(PREC_UNARY);

        // Emit the operator instruction.
        switch (operatorType) {
            case TOKEN_MINUS: emitByte(OP_NEGATE); break;
            default: return; // Unreachable.
        }
    }
    void parsePrecedence(Precedence precedence) {
        // What goes here?
    }

    void errorAtCurrent(const std::string& message) {errorAt(parser->current, message);}
    void error(const std::string& message) {errorAt(parser->previous, message);}
    void errorAt(const Token& token, const std::string& message) {
        if (parser->panicMode) return;
        parser->panicMode = true;

        std::cerr << "[line " << token.line << "] Error" ;
        if (token.type == TOKEN_EOF) {
            std::cerr << " at end";
        } else if (token.type == TOKEN_ERROR) {
            // Nothing.
        } else {
            std::cerr << " at '" << scanner.source.substr(token.start, token.length) << "'" ;
        }
        std::cerr << ": " << message << std::endl;
        parser->hadError = true;
    }
};