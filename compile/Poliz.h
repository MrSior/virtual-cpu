#ifndef VIRTUAL_CPU_POLIZ_H
#define VIRTUAL_CPU_POLIZ_H

#include <map>
#include <string>
#include <utility>
#include <vector>
enum class EPolizCmd : int32_t {
    Invalid,

    Push,
    Pop,
    Pushr,
    Popr,
    Add,
    Sub,
    Mul,
    Div,
    Out,
    In,
    Jmp,
    Jeq,
    Jne,
    Ja,
    Jae,
    Jb,
    Jbe,
    Ret
};

enum class ERegister : int32_t { AX, BX, CX, DX, PC, None, Invalid };

class PolizEntry {
  public:
    PolizEntry() = default;

    explicit PolizEntry(EPolizCmd pCmd) : cmd(pCmd){};
    explicit PolizEntry(EPolizCmd pCmd, int64_t pOp)
        : cmd(pCmd), operand(pOp){};

    explicit PolizEntry(EPolizCmd pCmd, ERegister r) : cmd(pCmd), reg(r){};

    EPolizCmd cmd = EPolizCmd::Invalid;
    ERegister reg = ERegister::None;
    int64_t operand = 0;

    struct PolizStruct {
        int32_t cmd;
        int32_t reg;
        int64_t operand;
    };

    PolizStruct toStruct();
};

class Poliz {
  private:
    std::vector<PolizEntry> poliz_;
    std::map<std::string, int> labelsRegistry_;

  public:
    void addEntry(const PolizEntry &entry);
    bool addLabel(const std::string &label, int idx);
    bool checkIfLabelInRegist(const std::string &label);
    int getLabelAddress(const std::string &label);
    std::vector<PolizEntry> getEntries() const;

    template <typename T> void setEntryOp(int idx, const T &val) {
        if constexpr (std::is_same_v<T, int64_t>) {
            poliz_[idx].operand = val;
        }
        if constexpr (std::is_same_v<T, ERegister>) {
            poliz_[idx].reg = val;
        }
    }

    int getSize();

    Poliz &operator=(const Poliz &other) {
        if (this != &other) {
            poliz_ = other.poliz_;
            labelsRegistry_ = other.labelsRegistry_;
        }
        return *this;
    }

    Poliz &operator+=(const Poliz &other) {
        for (const auto &elem : other.poliz_) {
            addEntry(elem);
        }
        return *this;
    }

    Poliz operator+(const Poliz &other) {
        Poliz res = *this;
        res += other;
        return res;
    }
};

EPolizCmd StrToPEntryType(const std::string &str);

ERegister StrToRegType(const std::string &str);

extern std::map<EPolizCmd, std::string> g_PolizCmdToStr;

#endif