#include "Poliz.h"

void Poliz::addEntry(const PolizEntry &entry) { poliz_.push_back(entry); }

bool Poliz::addLabel(const std::string &label, int idx) {
    if (labelsRegistry_.contains(label)) {
        return false;
    }
    labelsRegistry_.insert({label, idx});
    return true;
}

int Poliz::getSize() { return (int)poliz_.size(); }

bool Poliz::checkIfLabelInRegist(const std::string &label) {
    return labelsRegistry_.contains(label);
}

int Poliz::getLabelAddress(const std::string &label) {
    return labelsRegistry_[label];
}

EPolizCmd StrToPEntryType(const std::string &str) {
    if (str == "pop")
        return EPolizCmd::Pop;
    if (str == "popr")
        return EPolizCmd::Popr;
    if (str == "add")
        return EPolizCmd::Add;
    if (str == "sub")
        return EPolizCmd::Sub;
    if (str == "mul")
        return EPolizCmd::Mul;
    if (str == "div")
        return EPolizCmd::Div;
    if (str == "out")
        return EPolizCmd::Out;
    if (str == "in")
        return EPolizCmd::In;
    if (str == "ret")
        return EPolizCmd::Ret;
    if (str == "push")
        return EPolizCmd::Push;
    if (str == "pushr")
        return EPolizCmd::Pushr;
    if (str == "jmp")
        return EPolizCmd::Jmp;
    if (str == "ja")
        return EPolizCmd::Ja;
    if (str == "jeq")
        return EPolizCmd::Jeq;
    if (str == "jne")
        return EPolizCmd::Jne;
    if (str == "jae")
        return EPolizCmd::Jae;
    if (str == "jb")
        return EPolizCmd::Jb;
    if (str == "jbe")
        return EPolizCmd::Jbe;
    return EPolizCmd::Invalid;
}

ERegister StrToRegType(const std::string &str) {
    if (str == "AX") {
        return ERegister::AX;
    }
    if (str == "BX") {
        return ERegister::BX;
    }
    if (str == "CX") {
        return ERegister::CX;
    }
    if (str == "DX") {
        return ERegister::DX;
    }
    if (str == "PC") {
        return ERegister::PC;
    }
    return ERegister::Invalid;
}