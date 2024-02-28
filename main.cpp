#include "compile/Compiler.h"
#include "lexemeparser/Lexeme.h"
#include "lexemeparser/Parser.h"
#include "mystack/src/mystack.h"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <type_traits>
#include <vector>

class C {
    int cl;

  public:
    C() { cl = 3; }
    C(int x) {
        cl = x;
        return;
    }
};

class A {
  public:
    int a;
    int b;

    A() {
        a = b = 0;
        std::cout << "LOG: default cstr call" << std::endl;
    };

    A(int a, int b) {
        this->a = a;
        this->b = b;
        std::cout << "LOG: cstr from data call" << std::endl;
    }

    A(const A &t) {
        a = t.a;
        b = t.b;
        std::cout << "LOG: copy cstr call" << std::endl;
    }

    A(A &&t) {
        a = t.a;
        b = t.b;
        std::cout << "LOG: move cstr call" << std::endl;
    }

    A &operator=(A &&t) {
        a = t.a;
        b = t.b;
        std::cout << "LOG: move assignment operator call" << std::endl;
        return *this;
    }

    A &operator=(const A &t) {
        a = t.a;
        b = t.b;
        std::cout << "LOG: assignment operator call" << std::endl;
        return *this;
    }

    ~A() { std::cout << "LOG: destuctor call\n"; }
};

int main(int argc, char *argv[]) {

    // container::Stack<A> a;

    // for (int i = 0; i < 3; ++i) {
    //     a.push(A({i, i}));
    // }

    // std::cout << "_________pop()_________\n";
    // A elem = a.pop();
    // std::cout << elem.a << " " << elem.b << '\n';
    // std::cout << "_______________________\n";

    //     container::Stack<int> a;

    if (argc < 2) {
        throw std::runtime_error("no file was passed in arguments");
    }
    std::string input_file = argv[argc - 1];

    LexemeParser parser;

    auto res = parser.GetLexemes(input_file);

    for (auto &elem : res) {
        std::cout << "Line: " << elem.line << " pos: " << elem.pos << " "
                  << g_LexemeTypeToStr[elem.type] << " "
                  << LexemeDataToStr(elem) << std::endl;
    }

    for (auto it = res.begin(); it < res.end();) {
        if (it->type == ELexemeType::Null) {
            it = res.erase(it);
        } else if (it->type == ELexemeType::Invalid) {
            std::stringstream error;
            error << "[ERROR] Invalid symbol at line " << it->line << " pos "
                  << it->pos << std::endl;
            throw std::runtime_error(error.str());
        } else {
            ++it;
        }
    }

    for (auto it = res.begin() + 1; it < res.end();) {
        if (it->type == ELexemeType::Newline &&
            (it - 1)->type == ELexemeType::Newline) {
            it = res.erase(it);
        } else {
            ++it;
        }
    }

    Compiler compiler(res);

    compiler.compile();
    auto poliz = compiler.getPoliz();

    for (auto &elem : poliz.getEntries()) {
        std::cout << g_PolizCmdToStr[elem.cmd] << " ";
        if (elem.reg != ERegister::None) {
            std::cout << (int32_t)elem.reg << "\n";
        } else {
            std::cout << elem.operand << "\n";
        }
    }

    std::ofstream output("progfiles/output.out", std::ios::binary);
    //    for (auto &elem : poliz.getEntries()) {
    //        //        auto st = elem.toStruct();
    //        //        output.write(reinterpret_cast<const char *>(&st),
    //        sizeof(st)); output.write(reinterpret_cast<const char *>(&elem),
    //        sizeof(elem));
    //    }

    auto entries = poliz.getEntries();
    uint16_t entries_size = entries.size();
    output.write(reinterpret_cast<const char *>(&entries_size),
                 sizeof(entries_size));
    output.write(reinterpret_cast<const char *>(&entries[0]),
                 sizeof(PolizEntry) * entries_size);

    if (!output.good()) {
        throw std::runtime_error("Failed to write file");
    }
    output.close();

    return 0;
}
