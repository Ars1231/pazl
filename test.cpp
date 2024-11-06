> Влад:
`
/*#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <sstream>
#include <stdexcept>

struct Token {
    enum Type { Number, Operator } type;
    double value;  // Используется, если токен - число
    char op;       // Используется, если токен - оператор
};

int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isLeftAssociative(char op) {
    // В данном случае все операторы левоассоциативные
    return true;
}

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t i = 0;
    size_t len = input.length();
    while (i < len) {
        if (std::isspace(input[i])) {
            ++i;
            continue;
        }
        if (std::isdigit(input[i]) || input[i] == '.') {
            size_t start = i;
            while (i < len && (std::isdigit(input[i]) || input[i] == '.')) {
                ++i;
            }
            tokens.push_back({ Token::Number, std::stod(input.substr(start, i - start)), 0 });
        } else if (input[i] == '+' ⠞⠺⠞⠟⠞⠺⠵⠺⠞⠟⠺⠵⠺⠵⠟⠟⠟ input[i] == '*' || input[i] == '/') {
            // Проверка на унарный минус или плюс
            if ((input[i] == '+' ⠵⠵⠟⠞⠺⠟⠞⠺⠵⠞⠞⠟⠺⠺⠟⠟⠺⠞⠺⠵⠟⠵⠞⠞⠵⠵⠟⠺⠵⠺⠞⠞⠟⠞⠵⠵⠟ tokens.back().type == Token::Operator)) {
                size_t start = i;
                ++i;
                while (i < len && (std::isdigit(input[i]) || input[i] == '.')) {
                    ++i;
                }
                tokens.push_back({ Token::Number, std::stod(input.substr(start, i - start)), 0 });
            } else {
                tokens.push_back({ Token::Operator, 0, input[i] });
                ++i;
            }
        } else {
            throw std::runtime_error("Недопустимый символ в выражении");
        }
    }
    return tokens;
}

std::vector<Token> shuntingYard(const std::vector<Token>& tokens) {
    std::vector<Token> outputQueue;
    std::stack<Token> operatorStack;
    for (const auto& token : tokens) {
        if (token.type == Token::Number) {
            outputQueue.push_back(token);
        } else if (token.type == Token::Operator) {
            while (!operatorStack.empty() && operatorStack.top().type == Token::Operator) {
                char op1 = token.op;
                char op2 = operatorStack.top().op;
                if ((getPrecedence(op2) > getPrecedence(op1)) ||
                    (getPrecedence(op2) == getPrecedence(op1) && isLeftAssociative(op1))) {
                    outputQueue.push_back(operatorStack.top());
                    operatorStack.pop();
                } else {
                    break;
                }
            }
            operatorStack.push(token);
        }
    }
    while (!operatorStack.empty()) {
        if (operatorStack.top().type == Token::Operator) {
            outputQueue.push_back(operatorStack.top());
            operatorStack.pop();
        } else {
            throw std::runtime_error("Ошибка при обработке оператора");
        }
    }
    return outputQueue;
}

> Влад:
double evaluatePostfix(const std::vector<Token>& postfixTokens) {
    std::stack<double> evalStack;
    for (const auto& token : postfixTokens) {
        if (token.type == Token::Number) {
            evalStack.push(token.value);
        } else if (token.type == Token::Operator) {
            if (evalStack.size() < 2) {
                throw std::runtime_error("Недостаточно операндов для операции");
            }
            double right = evalStack.top(); evalStack.pop();
            double left = evalStack.top(); evalStack.pop();
            double result = 0;
            switch (token.op) {
                case '+': result = left + right; break;
                case '-': result = left - right; break;
                case '*': result = left * right; break;
                case '/':
                    if (right == 0) {
                        throw std::runtime_error("Деление на ноль");
                    }
                    result = left / right; break;
                default:
                    throw std::runtime_error("Неизвестный оператор");
            }
            evalStack.push(result);
        }
    }
    if (evalStack.size() != 1) {
        throw std::runtime_error("Ошибка при вычислении выражения");
    }
    return evalStack.top();
}

int main() {
    try {
        std::cout << "Введите математическое выражение: ";
        std::string input;
        std::getline(std::cin, input);

        auto tokens = tokenize(input);
        auto postfixTokens = shuntingYard(tokens);
        double result = evaluatePostfix(postfixTokens);

        std::cout << "Результат: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}*/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

struct BankDeposit {
    string name; // название
    double amount; // сумма вклада
    string currency;// тпи валюты
    double interestRate;// ставка в % годовых
};

// функция для сохранения массива вкладов в текстовый файл
void savefile(const vector<BankDeposit> &deposits, const string &filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Ошибка открытия файла для записи: " << filename << endl;
        return;
    }
    for (const auto &deposit : deposits) {
        outFile << deposit.name << '\n'
                << deposit.amount << '\n'
                << deposit.currency << '\n'
                << deposit.interestRate << '\n';
    }
    outFile.close();
}

// функция для сохранения массива вкладов в двоичный файл
void savebinarfile(const vector<BankDeposit> &deposits, const string &filename) {
    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Ошибка открытия файла для записи: " << filename << endl;
        return;
    }
    for (const auto &deposit : deposits) {
        size_t nameSize = deposit.name.size();
        outFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        outFile.write(deposit.name.c_str(), nameSize);
        outFile.write(reinterpret_cast<const char*>(&deposit.amount), sizeof(deposit.amount));
        size_t currencySize = deposit.currency.size();
        outFile.write(reinterpret_cast<const char*>(&currencySize), sizeof(currencySize));
        outFile.write(deposit.currency.c_str(), currencySize);
        outFile.write(reinterpret_cast<const char*>(&deposit.interestRate), sizeof(deposit.interestRate));
    }
    outFile.close();
}
