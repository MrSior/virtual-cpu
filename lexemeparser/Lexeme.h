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
    //    Instruction,
    Operator,
    Null,
    LiteralNum64,
    Keyword,
    Invalid,
    ProgramEnd
};

class Lexeme {
  public:
    Lexeme() = default;

    Lexeme(ELexemeType t, int l, int p) : type(t), line(l), pos(p){};

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
    Lexeme(ELexemeType type, const T &val, int l, int p) : Lexeme(type, l, p) {
        setValue<T>(val);
    };

    ELexemeType type = ELexemeType::Null;
    std::string str = "";
    int64_t i64 = 0;

    int line = -1;
    int pos = -1;
};

extern std::string LexemeDataToStr(const Lexeme &lexeme);
extern std::map<ELexemeType, std::string> g_LexemeTypeToStr;

#endif
