#include "Runtime.h"
#include <cstdint>
#include <ostream>

Runtime::Runtime(std::string bin_path) {
    bin_path_ = std::move(bin_path);

    std::ifstream in(bin_path_, std::ios::in | std::ios::binary);
    uint16_t size = 0;
    in.read(reinterpret_cast<char *>(&size), sizeof(size));

    if (!in.good()) {
        throw std::runtime_error(
            "Failed read size of program from binary file");
    }

    program_.resize(size);

    in.read(reinterpret_cast<char *>(program_.data()),
            sizeof(PolizEntry) * size);

    if (!in.good()) {
        throw std::runtime_error("Failed read program from binary file");
    }

    in.close();
}

void Runtime::run() {
    container::Stack<int64_t> stack;

    int64_t reg_AX, reg_BX, reg_CX, reg_DX, reg_PC;
    reg_AX = reg_BX = reg_CX = reg_DX = reg_PC = 0;

    std::map<ERegister, int64_t> regs = {{ERegister::AX, 0},
                                         {ERegister::BX, 0},
                                         {ERegister::CX, 0},
                                         {ERegister::DX, 0},
                                         {ERegister::PC, 0}};

    for (int ind = 0; ind < program_.size();) {
        bool isMove = true;

        auto elem = program_[ind];
        if (elem.cmd == EPolizCmd::Push) {
            stack.push(elem.operand);
        } else if (elem.cmd == EPolizCmd::Pop) {
            stack.pop();
        } else if (elem.cmd == EPolizCmd::Pushr) {
            stack.push(regs[elem.reg]);
        } else if (elem.cmd == EPolizCmd::Popr) {
            regs[elem.reg] = stack.pop();
        } else if (elem.cmd == EPolizCmd::Add) {
            int64_t val1 = stack.pop();
            int64_t val2 = stack.top();
            int64_t res = val1 + val2;
            stack.push(val1);
            stack.push(res);
        } else if (elem.cmd == EPolizCmd::Sub) {
            int64_t val1 = stack.pop();
            int64_t val2 = stack.top();
            int64_t res = val1 - val2;
            stack.push(val1);
            stack.push(res);
        } else if (elem.cmd == EPolizCmd::Mul) {
            int64_t val1 = stack.pop();
            int64_t val2 = stack.top();
            int64_t res = val1 * val2;
            stack.push(val1);
            stack.push(res);
        } else if (elem.cmd == EPolizCmd::Div) {
            int64_t val1 = stack.pop();
            int64_t val2 = stack.top();
            int64_t res = val1 / val2;
            stack.push(val1);
            stack.push(res);
        } else if (elem.cmd == EPolizCmd::Out) {
            std::cout << stack.pop() << std::endl;
        } else if (elem.cmd == EPolizCmd::In) {
            int64_t val;
            std::cin >> val;
            stack.push(val);
        } else if (elem.cmd == EPolizCmd::Jmp) {
            ind = elem.operand;
            isMove = false;
        } else if (elem.cmd == EPolizCmd::Jeq) {
            auto val1 = stack.pop();
            auto val2 = stack.pop();
            if (val1 == val2) {
                ind = elem.operand;
                isMove = false;
            }
            stack.push(val2);
            stack.push(val1);
        } else if (elem.cmd == EPolizCmd::Jne) {
            auto val1 = stack.pop();
            auto val2 = stack.pop();
            if (val1 != val2) {
                ind = elem.operand;
                isMove = false;
            }
            stack.push(val2);
            stack.push(val1);
        } else if (elem.cmd == EPolizCmd::Ja) {
            auto val1 = stack.pop();
            auto val2 = stack.pop();
            if (val1 > val2) {
                ind = elem.operand;
                isMove = false;
            }
            stack.push(val2);
            stack.push(val1);
        } else if (elem.cmd == EPolizCmd::Jae) {
            auto val1 = stack.pop();
            auto val2 = stack.pop();
            if (val1 >= val2) {
                ind = elem.operand;
                isMove = false;
            }
            stack.push(val2);
            stack.push(val1);
        } else if (elem.cmd == EPolizCmd::Jb) {
            auto val1 = stack.pop();
            auto val2 = stack.pop();
            if (val1 < val2) {
                ind = elem.operand;
                isMove = false;
            }
            stack.push(val2);
            stack.push(val1);
        } else if (elem.cmd == EPolizCmd::Jbe) {
            auto val1 = stack.pop();
            auto val2 = stack.pop();
            if (val1 <= val2) {
                ind = elem.operand;
                isMove = false;
            }
            stack.push(val2);
            stack.push(val1);
        } else if (elem.cmd == EPolizCmd::Ret) {
            ind = stack.pop();
        }

        if (isMove) {
            ++ind;
        }
    }
}
