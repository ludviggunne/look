CXX=g++
CFLAGS=-std=c++17 -Wall -Wextra -Wpedantic -MMD
INCLUDE=-Iinclude
SRC=$(wildcard src/*.cc)
OBJ=$(SRC:src/%.cc=build/%.o)
EXE=look

TEST_SRC=$(wildcard tests/*.cc)
TEST_OBJS_=$(TEST_SRC:tests/%.cc=build/%.o)
TEST_OBJS_+=$(OBJ)
# exclude main.o when building test executable
TEST_OBJS=$(filter-out build/main.o,$(TEST_OBJS_))

.PHONY: clean test run

#-include $(OBJ:.o=.d)
#-include $(TEST_OBJS:.o=.d)

$(EXE): build/$(EXE)
	@echo "Copy $< -> $@"
	@cp $< .

build/$(EXE): $(OBJ)
	@echo "Linking $@"
	@mkdir -p build
	@$(CXX) $(OBJ) -o $@

build/%.o: src/%.cc
	@echo "Compiling $@"
	@mkdir -p build
	@$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@

build/test: $(TEST_OBJS)
	@echo "Linking $@"
	@mkdir -p build
	@$(CXX) $(TEST_OBJS) -o $@

build/%.test.o: tests/%.test.cc
	@echo "Compiling $@"
	@mkdir -p build
	@$(CXX) $(CFLAGS) $(INCLUDE) -c $< -o $@

test: build/test
	@./build/test

# make run ARGS="..."
run: $(EXE)
	@./$<

clean:
	rm -rf build index.txt
