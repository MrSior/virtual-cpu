CXX=clang++
# CXX=/opt/homebrew/opt/gcc/bin/g++-13
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

LEXEME_P_DIR = lexemeparser
LEXEME_P_OBJ_DIR = $(LEXEME_P_DIR)/obj
LEXEME_P_SRCS = $(wildcard $(LEXEME_P_DIR)/*.cpp)
LEXEME_P_OBJS = $(patsubst $(LEXEME_P_DIR)/%.cpp, $(LEXEME_P_OBJ_DIR)/%.o, $(LEXEME_P_SRCS))

COMPILE_DIR = compile
COMPILE_OBJ_DIR = $(COMPILE_DIR)/obj
COMPILE_SRCS = $(wildcard $(COMPILE_DIR)/*.cpp)
COMPILE_OBJS = $(patsubst $(COMPILE_DIR)/%.cpp, $(COMPILE_OBJ_DIR)/%.o, $(COMPILE_SRCS))

RUNTIME_DIR = runtime
RUNTIME_OBJ_DIR = $(RUNTIME_DIR)/obj
RUNTIME_SRCS = $(wildcard $(RUNTIME_DIR)/*.cpp)
RUNTIME_OBJS = $(patsubst $(RUNTIME_DIR)/%.cpp, $(RUNTIME_OBJ_DIR)/%.o, $(RUNTIME_SRCS))

all: $(STACK) $(BUILD)/main 

$(BUILD)/main: main.cpp $(LEXEME_P_OBJS) $(COMPILE_OBJS)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $(BUILD)/main -L./$(STACKLIBDIR)/ -lStack -I./$(STACKSRCDIR)/ -I./$(LEXEME_P_DIR) -I./$(COMPILE_DIR)

$(BUILD)/runtime: run.cpp $(COMPILE_OBJS) $(RUNTIME_OBJS)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $(BUILD)/runtime -L./$(STACKLIBDIR)/ -lStack -I./$(STACKSRCDIR)/ -I./$(COMPILE_DIR) -I./$(RUNTIME_DIR)

$(STACK): $(STACKOBJS)
	@mkdir $(STACKLIBDIR)
	@ar cvrs $(STACK) $(STACKOBJS)

$(STACKOBJDIR)/%.o: $(STACKSRCDIR)/%.cpp $(STACKSRCDIR)/%.h
	@mkdir $(STACKOBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LEXEME_P_OBJ_DIR)/%.o: $(LEXEME_P_DIR)/%.cpp $(LEXEME_P_DIR)/%.h
	@mkdir -p $(LEXEME_P_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(COMPILE_OBJ_DIR)/%.o: $(COMPILE_DIR)/%.cpp $(COMPILE_DIR)/%.h
	@mkdir -p $(COMPILE_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RUNTIME_OBJ_DIR)/%.o: $(RUNTIME_DIR)/%.cpp $(RUNTIME_DIR)/%.h
	@mkdir -p $(RUNTIME_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(TEST)/bin:
	@mkdir $@

$(TEST)/bin/%: $(TEST)/%.cpp
	$(CXX) $(CXXFLAGS) $< -I./$(STACKSRCDIR)/ -o $@ -L./$(STACKLIBDIR)/ -lStack -I./$(TEST)/criterionlib/include/criterion -L./$(TEST)/criterionlib/lib/ -lcriterion

test: $(TEST)/bin $(TESTBINS)
	@for test in $(TESTBINS) ; do ./$$test ; done

testinfo: $(TEST)/bin $(TESTBINS)
	@for test in $(TESTBINS) ; do ./$$test --verbose ; done 

.PHONY: run clean test testinfo compile

compile: $(BUILD)/main
	@./$< $(filter-out $@,$(MAKECMDGOALS))

run: $(BUILD)/runtime
	@./$<

clean:
	@rm -rf $(BUILD) $(STACKLIBDIR) $(STACKOBJDIR) $(TEST)/bin $(LEXEME_P_OBJ_DIR) $(COMPILE_OBJ_DIR) $(RUNTIME_OBJ_DIR)

%:
	@:
