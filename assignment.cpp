#include <iostream>
#include <cctype>


using namespace std;

// Token types
enum TokenType {
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    EOF_TOKEN
};

// Token structure
struct Token {
    TokenType type;
    int value;
};

class Lexer {
public:
    Lexer(const string& text) : text(text), pos(0), current_char(text[0]) {}

    void error() {
        throw runtime_error("Invalid character");
    }

    void advance() {
        pos++;
        if (pos < text.size()) {
            current_char = text[pos];
        } else {
            current_char = '\0';
        }
    }

    void skip_whitespace() {
        while (current_char != '\0' && isspace(current_char)) {
            advance();
        }
    }

    int integer() {
        string result;
        while (current_char != '\0' && isdigit(current_char)) {
            result += current_char;
            advance();
        }
        return stoi(result);
    }

    Token get_next_token() {
        while (current_char != '\0') {
            if (isspace(current_char)) {
                skip_whitespace();
                continue;
            }

            if (isdigit(current_char)) {
                return Token{INTEGER, integer()};
            }

            if (current_char == '+') {
                advance();
                return Token{PLUS, '+'};
            }

            if (current_char == '-') {
                advance();
                return Token{MINUS, '-'};
            }

            if (current_char == '*') {
                advance();
                return Token{MULTIPLY, '*'};
            }

            if (current_char == '/') {
                advance();
                return Token{DIVIDE, '/'};
            }

            if (current_char == '(') {
                advance();
                return Token{LPAREN, '('};
            }

            if (current_char == ')') {
                advance();
                return Token{RPAREN, ')'};
            }

            error();
        }

        return Token{EOF_TOKEN, '\0'};
    }

private:
    string text;
    size_t pos;
    char current_char;
};

class Parser {
public:
    Parser(Lexer& lexer) : lexer(lexer), current_token(lexer.get_next_token()) {}

    void error() {
        throw runtime_error("Invalid syntax");
    }

    void eat(TokenType expected_type) {
        if (current_token.type == expected_type) {
            current_token = lexer.get_next_token();
        } else {
            error();
        }
    }

    int factor() {
        int result;
        Token token = current_token;
        if (token.type == INTEGER) {
            result = token.value;
            eat(INTEGER);
        } else if (token.type == LPAREN) {
            eat(LPAREN);
            result = expr();
            eat(RPAREN);
        } else {
            error();
        }
        return result;
    }

    int term() {
        int result = factor();

        while (current_token.type == MULTIPLY || current_token.type == DIVIDE) {
            Token token = current_token;
            if (token.type == MULTIPLY) {
                eat(MULTIPLY);
                result *= factor();
            } else if (token.type == DIVIDE) {
                eat(DIVIDE);
                int divisor = factor();
                if (divisor != 0) {
                    result /= divisor;
                } else {
                    throw runtime_error("Division by zero");
                }
            }
        }

        return result;
    }

    int expr() {
        int result = term();

        while (current_token.type == PLUS || current_token.type == MINUS) {
            Token token = current_token;
            if (token.type == PLUS) {
                eat(PLUS);
                result += term();
            } else if (token.type == MINUS) {
                eat(MINUS);
                result -= term();
            }
        }

        return result;
    }

    int parse() {
        return expr();
    }

private:
    Lexer& lexer;
    Token current_token;
};

int main() {
    string input;
    cout << "Python-like Compiler/Interpreter Built in C++ for Arithmetic Operations" << endl;
    cout << ">>> ";  
    getline(cin, input);

    Lexer lexer(input);
    Parser parser(lexer);

    try {
        int result = parser.parse();
        cout << result << endl;  
    } catch (const exception& e) {
        cerr << "Error during parsing: " << e.what() << endl;
        return 1;
    }

    return 0;
}
