#pragma once
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

enum class TokenType {
    exit,
    int_lit,
    semi,
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    inline explicit Tokenizer(std::string src) : m_src(std::move(src)) {}

    inline std::vector<Token> tokenize()
    {
        std::vector<Token> tokens;
        std::string buf;
        while (peak().has_value()) {
            if (std::isalpha(peak().value()[0])) {
                buf.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value()[0])) {
                    buf.push_back(consume());
                }
                if (buf == "exit") {
                    tokens.push_back(Token{TokenType::exit, std::nullopt});
                    buf.clear();
                    continue;
                } else {
                    std::cerr << "You messed up!" << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else if (std::isdigit(peak().value()[0])) {
                buf.push_back(consume());
                while (peak().has_value() && std::isdigit(peak().value()[0])) {
                    buf.push_back(consume());
                }
                tokens.push_back(Token{TokenType::int_lit, buf});
                buf.clear();
                continue;
            } else if (peak().value()[0] == ';') {
                consume();
                tokens.push_back(Token{TokenType::semi, std::nullopt});
                continue;
            } else if (std::isspace(peak().value()[0])) {
                consume();
                continue;
            } else {
                std::cerr << "You messed up!" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return tokens;
    }
private:
    [[nodiscard]] inline std::optional<std::string> peak(int ahead = 1) const {
        if (m_index + ahead > m_src.length()) {
            return {};
        } else {
            return &m_src.at(m_index);
        }
    }
    inline char consume() {
       return m_src.at(m_index++);
    }
    int m_index = 0;
    const std::string m_src;
};