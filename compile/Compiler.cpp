//
// Created by Семён on 25.02.2024.
//

#include "Compiler.h"

void Compiler::movePtr(int idx) {
    cur_lexeme_idx_ += idx;
    cur_lexeme_idx_ = std::max(0, cur_lexeme_idx_);
    cur_lexeme_idx_ = (int)std::min((size_t)cur_lexeme_idx_, input_.size() - 1);
    cur_lexeme_ = input_[cur_lexeme_idx_];
}

void Compiler::compile() {
    try {
        if (!input_.empty()) {
            movePtr(1);
            Program();
        }
    } catch (const CompileError &error) {
        std::cout << "[COMPILE ERROR]: " << error.what() << std::endl;
    }
}

void Compiler::Program() {
    if (cur_lexeme_.str != "begin") {
        throw CompileError("Program should start with BEGIN instruction",
                           cur_lexeme_.line, cur_lexeme_.pos);
    }
    movePtr(1);

    if (cur_lexeme_.type != ELexemeType::Newline) {
        throw CompileError("Each instruction should be placed on new line",
                           cur_lexeme_.line, cur_lexeme_.pos);
    }
    movePtr(1);

    Expressions();
}

void Compiler::Expressions() {
    Expression();

    if (cur_lexeme_.type != ELexemeType::Newline) {
        throw CompileError("Each instruction should be placed on new line",
                           cur_lexeme_.line, cur_lexeme_.pos);
    }
    movePtr(1);

    if (cur_lexeme_.str == "end") {
        return;
    }
    return Expressions();
}

void Compiler::Expression() {
    static std::vector<std::string> inst(
        {"pop", "add", "sub", "mul", "div", "out", "in", "ret"});
    static std::vector<std::string> inst_with_arg({"push", "pushr", "popr",
                                                   "jmp", "jeq", "jne", "ja",
                                                   "jae", "jb", "jbe", "call"});
    switch (cur_lexeme_.type) {
    case ELexemeType::Identifier:
        if (!poliz_.addLabel(cur_lexeme_.str, poliz_.getSize())) {
            throw CompileError("Declaration of same labels is not allowed",
                               cur_lexeme_.line, cur_lexeme_.pos);
        }
        movePtr(1);
        if (cur_lexeme_.str != ":") {
            throw CompileError("Expected colon after label", cur_lexeme_.line,
                               cur_lexeme_.pos);
        }
        movePtr(1);
        break;
    case ELexemeType::Operator:
        if (std::find(inst.begin(), inst.end(), cur_lexeme_.str) !=
            inst.end()) {
            poliz_.addEntry(PolizEntry(StrToPEntryType(cur_lexeme_.str)));
        } else if (std::find(inst_with_arg.begin(), inst_with_arg.end(),
                             cur_lexeme_.str) != inst_with_arg.end()) {
            if (cur_lexeme_.str == "call") {
                poliz_.addEntry(
                    PolizEntry(EPolizCmd::Push, poliz_.getSize() + 2));
                poliz_.addEntry(PolizEntry(EPolizCmd::Jmp));
            } else {
                poliz_.addEntry(PolizEntry(StrToPEntryType(cur_lexeme_.str)));
            }
            movePtr(1);
            Arg();
        } else {
            throw CompileError("Invalid instruction", cur_lexeme_.line,
                               cur_lexeme_.pos);
        }
        break;
    case ELexemeType::ProgramEnd:
        throw CompileError("Program should end with END instruction",
                           cur_lexeme_.line, cur_lexeme_.pos);
    default:
        throw CompileError("Expected instruction", cur_lexeme_.line,
                           cur_lexeme_.pos);
    }
}

void Compiler::Arg() {
    switch (cur_lexeme_.type) {
    case ELexemeType::LiteralNum64:
        poliz_.setEntryOp(poliz_.getSize() - 1, cur_lexeme_.i64);
        break;
    case ELexemeType::Keyword:
        poliz_.setEntryOp(poliz_.getSize() - 1, StrToRegType(cur_lexeme_.str));
        break;
    case ELexemeType::Identifier:
        if (!poliz_.checkIfLabelInRegist(cur_lexeme_.str)) {
            throw CompileError("Transition to undefined label",
                               cur_lexeme_.line, cur_lexeme_.pos);
        }
        poliz_.setEntryOp(poliz_.getSize() - 1,
                          poliz_.getLabelAddress(cur_lexeme_.str));
        break;
    default:
        throw CompileError("Invalid instruction argument", cur_lexeme_.line,
                           cur_lexeme_.pos);
    }
    movePtr(1);
}
Poliz Compiler::getPoliz() { return poliz_; }