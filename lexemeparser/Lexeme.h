#ifndef VIRTUAL_CPU_LEXEME_H
#define VIRTUAL_CPU_LEXEME_H

#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
enum class ELexemeType : int {
    Newline,
    Identifier,
    Instruction,
    Operator,
    Null,
    LiteralNum64,
    Keyword,
    Invalid
};

class Lexeme {
  public:
    Lexeme() = default;

    Lexeme(ELexemeType t) : type(t){};

    template <typename T> void setValue(const T &val) {
        if constexpr (std::is_same_v<T, int64_t>) {
            i64 = val;
        } else if (std::is_same_v<T, std::string>) {
            str = val;
        } else {
            throw std::logic_error("");
        }
    }

    template <typename T>
    Lexeme(ELexemeType type, const T &val) : Lexeme(type) {
        setValue<T>(val);
    };

    ELexemeType type = ELexemeType::Null;
    std::string str = "";
    int64_t i64 = 0;
};

extern std::string LexemeDataToStr(const Lexeme &lexeme);
extern std::map<ELexemeType, std::string> g_LexemeTypeToStr;

#endif
