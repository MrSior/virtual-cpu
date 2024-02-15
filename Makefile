CXX=g++
CXXFLAGS=-std=c++20

BUILD = build

STACKDIR = mystack
STACKSRCDIR = $(MYSTACKDIR)/src
STACK = $(MYSTACKDIR)/bin/libStack.a
STACKSRCS = $(wildcard $(STACKSRCDIR)/*.hpp)


TEST=tests
TESTS=$(wildcard $(TEST)/*.cpp)
TESTBINS=$(patsubst $(TEST)/%.cpp, $(TEST)/bin/%, $(TESTS))

$(BUILD)/main : main.cpp
    @mkdir $(BUILD)
    $(CXX) $(CXXFLAGS) $^ -o $(BUILD)/main -L./$(LIBDIR)/



build/mystack: mystack.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $^ -o $@

run: build/mystack
	@./build/mystack

test: mystack.h


clean:
	@rm -rf build

.PHONY: run clean
