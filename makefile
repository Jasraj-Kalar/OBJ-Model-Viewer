CC=clang
CFLAGS=-g
CFLAGS+=-I$(INC)/glad/include
CFLAGS+=-I$(INC)/cglm/include
CFLAGS+=-I$(INC)/glfw/include

CXX=clang++
CXXFLAGS=-g -Wall -std=c++20

LDFLAGS=-g
LDLIBS+=-L$(INC)/glfw/lib -lglfw

CPPFLAGS=

BIN=bin/output
OBJ=obj
INC=ext
SRC=src

SRCDIRS=$(SRC)
SRCDIRS+=$(INC)/glad/src

SRCS=$(foreach SRCDIR, $(SRCDIRS), $(wildcard $(SRCDIR)/*.cpp))
CSRCS=$(foreach SRCDIR, $(SRCDIRS), $(wildcard $(SRCDIR)/*.c))

OBJS=$(patsubst %.cpp, $(OBJ)/%.o, $(SRCS))
OBJS+=$(patsubst %.c, $(OBJ)/%.o, $(CSRCS))

all:$(BIN)

release: CXXFLAGS=-std=c++20 -Wall -O2 -DNDEBUG
release: CFLAGS=
release: clean
release: $(BIN)

$(BIN): $(OBJS)
	@echo Linking...
	@$(CXX) $(LDFLAGS) $(LDLIBS) $(OBJS) -o $@

# GNU Make Manual Section 4.5.5 Implicit Directory Search
$(OBJ)/%.o: %.cpp
	@echo Compiling $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# GNU Make Manual Section 4.5.5 Implicit Directory Search
$(OBJ)/%.o: %.c
	@echo Compiling $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

print-%  : ; @echo $* = $($*)

clean:
	$(RM) -r $(BIN) $(OBJ)/*
