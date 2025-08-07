#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

enum class TokenType {
    _return,
    int_lit,
    semi,
};

struct Token
{
    TokenType type;
    std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string& text)
{
    std::vector<Token> tokens;
    std::string buf;
    int text_size = text.size();
    for (int i = 0; i < text_size; i++)
    {
        char c = text.at(i);
        if (std::isalpha(c))
        {
            buf.push_back(c);
            i++;
            while (std::isalpha(text.at(i)))
            {
                buf.push_back(text.at(i));
                i++;
            }
            i--;
            if (buf == "return")
            {
                tokens.push_back(Token{TokenType::_return, std::nullopt});
                buf.clear();
            } else
            {
                std::cerr << "you messed up" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else if (std::isdigit(c))
        {
            buf.push_back(c);
            i++;
            while (std::isdigit(text.at(i)))
            {
                buf.push_back(text.at(i));
                i++;
            }
            i--;
            tokens.push_back(Token{TokenType::int_lit, buf});
            buf.clear();
        } else if (c == ';')
        {
            tokens.push_back(Token{TokenType::semi, std::nullopt});
        } else if (std::isspace(c))
        {
            continue;
        } else
        {
            std::cerr << "you messed up" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return tokens;
}

std::string tokens_to_asm(const std::vector<Token>& tokens)
{
    std::stringstream output;
    output << ".global _start\n";
    output << ".section .text\n_start:\n";
    for (int i = 0; i < tokens.size(); i++)
    {
        const Token& token = tokens[i];
        if (token.type == TokenType::_return)
        {
            if (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::int_lit)
            {
                if (i + 2 < tokens.size() && tokens[i + 2].type == TokenType::semi)
                {
                    output << "    mov x0, #" << tokens[i + 1].value.value() << "\n";
                    output << "    mov x8, #93\n" ;
                    output << "    svc #0\n";
                }
            }
        }
    }
    return output.str();
}

int main(const int argc, char *argv[]) {
    if (argc != 2)
    {
        std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
        std::cerr << "hydro <input.hy>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in | std::ios::binary);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }
    // std::cout << path << std::endl;
    std::vector<Token> tokens = tokenize(contents);
    {
        std::fstream file("out.s", std::ios::out | std::ios::binary);;
        file << tokens_to_asm(tokens);
        std::cout << tokens_to_asm(tokens) << std::endl;
    }

    system("as -o out.o out.s");
    system("ld -o out out.o");
    return EXIT_SUCCESS;
}