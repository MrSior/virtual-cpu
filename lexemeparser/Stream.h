#ifndef VIRTUAL_CPU_STREAM_H
#define VIRTUAL_CPU_STREAM_H

#include <cstddef>
#include <cstdint>
#include <string>

class Stream {
  public:
    char get();
    char peek();
    void seek(int diff);
    void setCur(int pos);
    int tell();
    bool isEnd();
    int getSize();
    Stream(std::string str = "");

    int64_t getNum(int st, bool is_read = true);

  private:
    std::string str_ = "";
    int off_ = 0;
};

#endif
