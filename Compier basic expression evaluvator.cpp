#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <stdexcept>

// Function to determine the precedence of an operator
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to apply an operator to two operands
double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw std::runtime_error("Division by zero!");
            return a / b;
    }
    return 0;
}

// Main function to evaluate the expression
double evaluate(const std::string& expression) {
    std::stack<double> values;
    std::stack<char> ops;

    for (int i = 0; i < expression.length(); i++) {
        // Skip whitespace
        if (expression[i] == ' ') {
            continue;
        }
        
        // If current token is a number, push it to stack for numbers
        else if (isdigit(expression[i])) {
            double val = 0;
            // There may be more than one digit in the number
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                 if (expression[i] == '.') {
                    i++;
                    double dec = 0.1;
                    while(i < expression.length() && isdigit(expression[i])) {
                        val = val + (expression[i] - '0') * dec;
                        dec /= 10;
                        i++;
                    }
                 } else {
                    val = (val * 10) + (expression[i] - '0');
                    i++;
                 }
            }
            values.push(val);
            i--; // Correct for the loop's i++
        }
        
        // If current token is an operator
        else {
            // While top of 'ops' has same or greater precedence to current token,
            // apply operator on top of 'ops' to top two elements in values stack.
            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            // Push current token to 'ops'.
            ops.push(expression[i]);
        }
    }

    // Entire expression has been parsed, apply remaining ops to remaining values.
    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();
        double val1 = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    // Top of 'values' contains the result, return it.
    return values.top();
}

int main() {
    std::string expression;
    std::cout << "Enter a basic arithmetic expression (e.g., 10 + 2 * 6): ";
    getline(std::cin, expression);

    try {
        double result = evaluate(expression);
        std::cout << "Result: " << result << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
