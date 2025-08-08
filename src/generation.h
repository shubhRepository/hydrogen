#pragma once
#include "parser.h"

#include <sstream>

class Generator {
public:
    inline explicit Generator(NodeExit root) : m_root(std::move(root)) {}

    [[nodiscard]] std::string generate() const
    {
        std::stringstream output;
        output << ".global _start\n";
        output << ".section .text\n";
        output << "_start:\n";
        output << "    mov x0, #" << m_root.expr.int_lit.value.value() << "\n";
        output << "    mov x8, #93\n";
        output << "    svc #0\n";
        return output.str();
    }
private:
    const NodeExit m_root;
};