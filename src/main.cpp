#include <iostream>
#include <string>

#include "tokenizer.hpp"

static void evaluate(const std::string& input) {
    sk::tokenizer tokenizer(input);

    sk::token next_token = tokenizer.next_token();
    while (next_token.type != sk::token_type::END_OF_FILE) {
        std::cout << "Type: " << sk::token_type_to_string(next_token.type) << ", Lexeme: " << next_token.lexeme << "\n";

        next_token = tokenizer.next_token();
    }
}

int main() {
    std::string input;
    std::cout << "Snark REPL. Type 'exit' to quit.\n";

    while (true) {
        std::cout << "> ";

        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        evaluate(input);
    }

    return 0;
}