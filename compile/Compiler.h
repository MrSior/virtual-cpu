//
// Created by Семён on 25.02.2024.
//

#ifndef VIRTUAL_CPU_COMPILER_H
#define VIRTUAL_CPU_COMPILER_H

#include "../lexemeparser/Lexeme.h"
#include "Poliz.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class CompileError : public std::exception {
  private:
    std::string ctx_ = "";
    int line_ = -1;
    int pos_ = -1;

  public:
    CompileError() = default;
    CompileError(std::string ctx, int line, int pos)
        : ctx_(std::move(ctx)), line_(line), pos_(pos) {
        ctx_ += "  line " + std::to_string(line_) + ":" + std::to_string(pos_);
    };

    [[nodiscard]] const char *what() const noexcept override {
        return ctx_.c_str();
    }
};

class Compiler {
  private:
    std::vector<Lexeme> input_;
    Lexeme cur_lexeme_;
    Poliz poliz_;
    int cur_lexeme_idx_ = -1;

    void movePtr(int idx);

    void Program();
    void Expressions();
    void Expression();
    void Arg();

  public:
    Compiler(std::vector<Lexeme> input)
        : input_(std::move(input)), cur_lexeme_idx_(-1){};

    void compile();
    Poliz getPoliz();
};

#endif // VIRTUAL_CPU_COMPILER_H
