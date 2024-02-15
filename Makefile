CXX=g++
CXXFLAGS=-std=c++20

build/mystack: mystack.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $^ -o $@

run: build/mystack
	@./build/mystack

clean:
	@rm -rf build

.PHONY: run clean
