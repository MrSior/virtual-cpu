#include "Lexeme.h"
#include <map>
#include <string>

std::string LexemeDataToStr(const Lexeme &lexeme) {
    return (lexeme.type == ELexemeType::LiteralNum64
                ? std::to_string(lexeme.i64)
                : lexeme.str);
}

std::map<ELexemeType, std::string> g_LexemeTypeToStr = {
    {ELexemeType::Identifier, "Identifier"},
    {ELexemeType::Instruction, "Instruction"},
    {ELexemeType::Invalid, "Invalid"},
    {ELexemeType::LiteralNum64, "LiteralNum64"},
    {ELexemeType::Newline, "Newline"},
    {ELexemeType::Null, "Null"},
    {ELexemeType::Operator, "Operator"},
    {ELexemeType::Keyword, "Keyword"}};
