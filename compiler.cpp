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




// Pratt parser operator precedence table
class Compiler {
    public:
    using ParseFn = void (Compiler::*)();
    
    typedef struct {
        ParseFn prefix;
        ParseFn infix;
        Precedence precedence;
    } ParseRule;
    Scanner scanner;
    Chunk& chunk ;
    Parser parser; 
    std::vector<ParseRule> rules;
    Compiler(const std::string& source, Chunk& chunk) : scanner(source), chunk(chunk)  {
        rules =  std::vector<ParseRule> {\
            /*[TOKEN_LEFT_PAREN]    =*/ {&Compiler::grouping, nullptr,   PREC_NONE},
            /*[TOKEN_RIGHT_PAREN]   =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_LEFT_BRACE]    =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_RIGHT_BRACE]   =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_COMMA]         =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_DOT]           =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_MINUS]         =*/ {&Compiler::unary,   &Compiler::binary, PREC_TERM},
            /*[TOKEN_PLUS]          =*/ {nullptr,     &Compiler::binary, PREC_TERM},
            /*[TOKEN_SEMICOLON]     =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_SLASH]         =*/ {nullptr,     &Compiler::binary, PREC_FACTOR},
            /*[TOKEN_STAR]          =*/ {nullptr,     &Compiler::binary, PREC_FACTOR},
            /*[TOKEN_BANG]          =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_BANG_EQUAL]    =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_EQUAL]         =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_EQUAL_EQUAL]   =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_GREATER]       =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_GREATER_EQUAL] =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_LESS]          =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_LESS_EQUAL]    =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_IDENTIFIER]    =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_STRING]        =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_NUMBER]        =*/ {&Compiler::number,   nullptr,   PREC_NONE},
            /*[TOKEN_AND]           =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_CLASS]         =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_ELSE]          =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_FALSE]         =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_FOR]           =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_FUN]           =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_IF]            =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_NIL]           =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_OR]            =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_PRINT]         =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_RETURN]        =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_SUPER]         =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_THIS]          =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_TRUE]          =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_VAR]           =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_WHILE]         =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_ERROR]         =*/ {nullptr,     nullptr,   PREC_NONE},
            /*[TOKEN_EOF]           =*/ {nullptr,     nullptr,   PREC_NONE},
        };
    }

    bool compile() {
        advance() ;
        expression();
        consume(TOKEN_EOF, "Expect end of expression.");
        endCompiler();
        return !parser.hadError ;
    }
    
    void advance() {
        parser.previous = parser.current;
        
        std::cout << (parser.current.start) << std::endl ;
        // skipping error tokens
        for (;;) {
            parser.current = scanner.scanToken();
            
            if (parser.current.type != TOKEN_ERROR) break;

            errorAtCurrent(parser.current.str);
        }
    }
    // advance if the right type is found or print an error
    void consume(const TokenType& type, const std::string& message) {
        if (parser.current.type == type) {
            advance();
            return;
        }

        errorAtCurrent(message);
    }
    void emitByte(uint8_t byte) {writeChunk(currentChunk(), byte, parser.previous.line);}
    void emitBytes(uint8_t byte1, uint8_t byte2) {
        emitByte(byte1);
        emitByte(byte2);
    }
    void emitConstant(Value value) {
        writeConstant(currentChunk(), value, parser.previous.line);
    }
    void emitReturn() {emitByte(OP_RETURN);}
    Chunk& currentChunk() {return chunk;}
    void endCompiler() {
        emitReturn();
        #ifdef DEBUG_PRINT_CODE
            disassembleChunk(currentChunk(), "code");
            std::cout << "hadError:" << parser.hadError << std::endl; 
        #endif
    }
    
    void expression() {parsePrecedence(PREC_ASSIGNMENT);}
    void number() {
        double value = strtod(scanner.source.c_str() + parser.previous.start, NULL);
        emitConstant(value);
    }
    void grouping() {
        expression();
        consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
    }
    void binary() {
        TokenType operatorType = parser.previous.type;
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
        TokenType operatorType = parser.previous.type;

        // Compile the operand.
        parsePrecedence(PREC_UNARY);

        // Emit the operator instruction.
        switch (operatorType) {
            case TOKEN_MINUS: emitByte(OP_NEGATE); break;
            default: return; // Unreachable.
        }
    }
    void parsePrecedence(Precedence precedence) {
        advance();
        ParseFn prefixRule = getRule(parser.previous.type).prefix;
        if (prefixRule == NULL) {
            error("Expect expression.");
            return;
        }

        (this->*prefixRule)();
        
        while (precedence <= getRule(parser.current.type).precedence) {
            advance();
            ParseFn infixRule = getRule(parser.previous.type).infix;
            (this->*infixRule)();
        }
    }
    ParseRule& getRule(TokenType& type) {
        return rules[type];
    }

    void errorAtCurrent(const std::string& message) {errorAt(parser.current, message);}
    void error(const std::string& message) {errorAt(parser.previous, message);}
    
    // print error and enter paanic mode .
    // panic mode means all subsequent errors get swallowed.
    // Panic mode ends when the parser reaches a synchronization point. 
    void errorAt(const Token& token, const std::string& message) {
        if (parser.panicMode) return;
        parser.panicMode = true;

        std::cerr << "[line " << token.line << "] Error" ;
        if (token.type == TOKEN_EOF) {
            std::cerr << " at end";
        } else if (token.type == TOKEN_ERROR) {
            // Nothing.
        } else {
            std::cerr << " at '" << scanner.source.substr(token.start, token.length) << "'" ;
        }
        std::cerr << ": " << message << std::endl;
        parser.hadError = true;
    }
};