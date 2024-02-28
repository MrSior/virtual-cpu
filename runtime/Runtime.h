//
// Created by Семён on 28.02.2024.
//

#ifndef VIRTUAL_CPU_RUNTIME_H
#define VIRTUAL_CPU_RUNTIME_H

#include "../compile/Poliz.h"
#include "../mystack/src/mystack.h"
#include <fstream>
#include <string>
#include <vector>

class Runtime {
  private:
    std::string bin_path_;
    std::vector<PolizEntry> program_;

  public:
    Runtime(std::string bin_path);

    void run();
};

#endif // VIRTUAL_CPU_RUNTIME_H
