#include <iostream>
#include <sstream>
#include <cctype>
#include <stdexcept>

class Compiler {
private:
    std::string expression;
    size_t pos;

    char peek() {
        return pos < expression.size() ? expression[pos] : '\0';
    }

    char get() {
        return pos < expression.size() ? expression[pos++] : '\0';
    }

    // Parse a number (e.g., 42, 3.14)
    double parseNumber() {
        std::stringstream numberStream;
        while (isdigit(peek()) || peek() == '.') {
            numberStream << get();
        }
        double number;
        numberStream >> number;
        return number;
    }

    // Parse a factor (number or parenthesis)
    double parseFactor() {
        if (peek() == '(') {
            get();  // Consume '('
            double result = parseExpression();
            if (get() != ')') {
                throw std::runtime_error("Expected closing parenthesis");
            }
            return result;
        } else {
            return parseNumber();
        }
    }

    // Parse a term (handles * and /)
    double parseTerm() {
        double result = parseFactor();
        while (peek() == '*' || peek() == '/') {
            char op = get();
            if (op == '*') {
                result *= parseFactor();
            } else if (op == '/') {
                result /= parseFactor();
            }
        }
        return result;
    }

    // Parse an expression (handles + and -)
    double parseExpression() {
        double result = parseTerm();
        while (peek() == '+' || peek() == '-') {
            char op = get();
            if (op == '+') {
                result += parseTerm();
            } else if (op == '-') {
                result -= parseTerm();
            }
        }
        return result;
    }

public:
    double evaluate(const std::string &expr) {
        expression = expr;
        pos = 0;
        return parseExpression();
    }
};

int main() {
    Compiler compiler;
    std::string input;

    std::cout << "Enter arithmetic expressions (type 'exit' to quit):\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "exit") {
            break;
        }
        try {
            double result = compiler.evaluate(input);
            std::cout << "Result: " << result << "\n";
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
