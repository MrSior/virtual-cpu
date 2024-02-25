#include "Parser.h"
#include "Lexeme.h"
#include "Stream.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

std::vector<Lexeme> LexemeParser::GetLexemes(const std::string &input_file) {
    std::vector<Lexeme> result;
    std::vector<std::string> resvOperators(
        {"begin", "end", "pop",  "add",   "sub",  "mul", "div", "out",
         "in",    "ret", "push", "pushr", "popr", "jmp", "jeq", "jne",
         "ja",    "jae", "jb",   "jbe",   "call", ":"});
    std::vector<std::string> resvKeywords({"AX", "BX", "CX", "DX", "PC"});

    std::ifstream input_f(input_file);
    if (!input_f.good()) {
        std::stringstream ss;
        ss << "Current path is " << std::filesystem::current_path() << " "
           << input_file << '\n';
        throw std::runtime_error("Failed to read input file: " + ss.str());
    }

    std::stringstream buff;
    buff << input_f.rdbuf();
    auto input = buff.str();

    int cur_line = 1;
    int line_start_pos = 0;

    Stream stream(input);
    while (!stream.isEnd()) {
        char cur_rd = stream.peek();
        if (cur_rd == ' ' || cur_rd == '\t') {
            stream.seek(1);
            result.push_back(Lexeme(ELexemeType::Null, cur_line,
                                    stream.tell() - line_start_pos));
        } else if (cur_rd == '\n') {
            stream.seek(1);
            result.push_back(Lexeme(ELexemeType::Newline, cur_line,
                                    stream.tell() - line_start_pos));
            cur_line += 1;
            line_start_pos = stream.tell();
        } else if (cur_rd >= '0' && cur_rd <= '9') {
            auto num = stream.getNum(stream.tell());
            result.push_back(Lexeme(ELexemeType::LiteralNum64, num, cur_line,
                                    stream.tell() - line_start_pos));
        } else {
            auto checkIfInList = [&stream](std::vector<std::string> list) {
                auto st_pos = stream.tell();
                std::tuple<bool, std::string> res = {false, ""};
                for (int i = 0; !stream.isEnd() && !list.empty(); ++i) {
                    char chr = stream.get();
                    for (auto it = list.begin(); it < list.end();) {
                        if (it->at(i) != chr) {
                            it = list.erase(it);
                        } else if (it->size() == i + 1) {
                            bool flag = false;
                            if (stream.isEnd()) {
                                flag = true;
                            } else {
                                char c = std::tolower(stream.peek());
                                if (!(c >= 'a' && c <= 'z') &&
                                    !(c >= '0' && c <= '9') && c != '_') {
                                    flag = true;
                                }
                            }
                            if (flag) {
                                res = {true, *it};
                            }
                            it = list.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }
                stream.setCur(st_pos);
                return res;
            };

            auto [validKeyword, str] = checkIfInList(resvKeywords);
            if (validKeyword) {
                stream.seek(str.size());
                result.push_back(Lexeme(ELexemeType::Keyword, str, cur_line,
                                        stream.tell() - line_start_pos));
                continue;
            }

            auto [validOperator, str2] = checkIfInList(resvOperators);
            if (validOperator) {
                stream.seek(str2.size());
                result.push_back(Lexeme(ELexemeType::Operator, str2, cur_line,
                                        stream.tell() - line_start_pos));
                continue;
            }

            std::string identifier_name;
            for (int i = 0; !stream.isEnd(); ++i) {
                char c = std::tolower(stream.peek());
                if (!(c >= 'a' && c <= 'z') && !(c >= '0' && c <= '9') &&
                    c != '_') {
                    break;
                }
                identifier_name += stream.get();
            }

            if (identifier_name.empty()) {
                result.push_back(Lexeme(ELexemeType::Invalid, cur_line,
                                        stream.tell() - line_start_pos));
                stream.seek(1);
            } else {
                result.push_back(Lexeme(ELexemeType::Identifier,
                                        identifier_name, cur_line,
                                        stream.tell() - line_start_pos));
            }
        }
    }

    result.push_back(Lexeme(ELexemeType::ProgramEnd, cur_line,
                            stream.tell() - line_start_pos));
    return result;
};
