CXX=g++
CXXFLAGS=-std=c++20

BUILD = build

STACKDIR = mystack
STACKSRCDIR = $(STACKDIR)/src
STACKOBJDIR = $(STACKDIR)/obj
STACKLIBDIR = $(STACKDIR)/lib
STACK = $(STACKLIBDIR)/libStack.a
STACKSRCS = $(wildcard $(STACKSRCDIR)/*.cpp)
STACKOBJS = $(patsubst $(STACKSRCDIR)/%.cpp, $(STACKOBJDIR)/%.o, $(STACKSRCS))


TEST=tests
TESTS=$(wildcard $(TEST)/*.cpp)
TESTBINS=$(patsubst $(TEST)/%.cpp, $(TEST)/bin/%, $(TESTS))

all: $(STACK) $(BUILD)/main 

$(BUILD)/main: main.cpp 
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $(BUILD)/main -L./$(STACKLIBDIR)/ -lStack -I./$(STACKSRCDIR)/

$(STACK): $(STACKOBJS)
	@mkdir $(STACKLIBDIR)
	ar cvrs $(STACK) $(STACKOBJS)

$(STACKOBJDIR)/%.o: $(STACKSRCDIR)/%.cpp $(STACKSRCDIR)/%.h
	@mkdir $(STACKOBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(TEST)/bin:
	@mkdir $@

$(TEST)/bin/%: $(TEST)/%.cpp
	$(CXX) $(CXXFLAGS) $< -I./$(STACKSRCDIR)/ -o $@ -L./$(STACKLIBDIR)/ -lStack -I./$(TEST)/criterionlib/include/criterion -L./$(TEST)/criterionlib/lib/ -lcriterion

test: $(TEST)/bin $(TESTBINS)
	for test in $(TESTBINS) ; do ./$$test --verbose ; done

run: $(BUILD)/main
	@./$<

clean:
	@rm -rf $(BUILD) $(STACKLIBDIR) $(STACKOBJDIR) $(TEST)/bin

.PHONY: run clean
