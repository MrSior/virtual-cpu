#ifndef VIRTUAL_CPU_LEXEMEPARSER_H
#define VIRTUAL_CPU_LEXEMEPARSER_H

#include "Lexeme.h"
#include "Stream.h"
#include <filesystem>
#include <vector>

class LexemeParser {
  public:
    LexemeParser() = default;
    std::vector<Lexeme> GetLexemes(const std::string &input_file);
};

#endif
