### Author: Eduardo Souza Rocha

### Copilation
CC       := gcc

### Output
EXEC     := program
ZIP      := $(EXEC).zip

### Folder
F_UTIL := Debug
F_SRC  := src
F_BIN  := bin

### Files
INCLUDES := $(wildcard $(F_SRC)/opcoes/*.c) $(wildcard $(F_SRC)/opcoes/*.h)
SRC      := $(wildcard $(F_SRC)/*.c)
OBJ      := $(SRC:$(F_SRC)/%.c=$(F_BIN)/%.o)
HEADER   := $(wildcard $(F_SRC)/*.h)
MK       := Makefile


### Flags
LIBS     := -lm
CFLAGS   := $(LIBS)
FFLAGS   := $(LIBS) -O3 -march=native -w
DFLAGS   := $(LIBS) -Wall -Wextra -pedantic -Werror=implicit-function-declaration -fsanitize=address
RMFLAGS  := -f -v

### Actions
all:
	mkdir -p $(F_BIN)
	$(MAKE) compile

run: $(EXEC)
	./$(EXEC)


### Compile
compile: FLAGS := $(CFLAGS)
compile: clean $(EXEC)

final: FLAGS := $(FFLAGS)
final: clean $(EXEC)

debug: FLAGS := $(DFLAGS)
debug: clean $(EXEC)

zip: clean_zip
	zip $(ZIP) $(SRC) $(HEADER) $(INCLUDES) $(MK)

### Clean
clean_zip:
	rm $(ZIP) $(RMFLAGS)

clean:
	rm $(OBJ) $(RMFLAGS)
	rm $(DEBUG) $(RMFLAGS)
	rm $(EXEC) $(RMFLAGS)
	rm $(ZIP) $(RMFLAGS)

clean_util:
	cd $(F_DEBUG) && $(MAKE) clean

### Util
test:
	sh test.sh $(EXEC)

lines:
	sh lines.sh

util:
	cd $(F_DEBUG) && $(MAKE) all

valgrind:
	# Perdir qual era o codigo
	# Se eu for refazer tem que usar shell script


### Exec
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(FLAGS)

$(F_BIN)/%.o: $(F_SRC)/%.c
	$(CC) -c -o $@ $< $(FLAGS)
