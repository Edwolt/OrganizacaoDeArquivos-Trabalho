### Author: Eduardo Souza Rocha

### Copilation
CC       := gcc

### Output
EXEC     := enduser
DEBUG    := debug
ZIP      := $(EXEC).zip

### Folder
F_DEBUG := Debug
F_SRC  := src
F_BIN  := bin

### Files
INCLUDES := $(wildcard $(F_SRC)/opcoes/*.c) $(wildcard $(F_SRC)/opcoes/*.h)
SRC      := $(wildcard $(F_SRC)/*.c)
OBJ      := $(SRC:$(F_SRC)/%.c=$(F_BIN)/%.o)
HEADER   := $(wildcard $(F_SRC)/*.h)
MK       := Makefile


### Flags
CFLAGS   := -Wall -lm -Wextra -pedantic -Werror=implicit-function-declaration 
DFLAGS   := -fsanitize=address
RMFLAGS  := -f -v

### Actions
all: 
	mkdir -p $(F_BIN)
	make $(EXEC)

run: $(EXEC)
	./$(EXEC)

compile: clean $(EXEC)

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

### Debug
test:
	sh test.sh $(EXEC)

lines:
	sh lines.sh

debug: $(OBJ)
	$(CC) -g -o $(DEBUG) $(SRC) $(CFLAGS) $(DFLAGS)

util:
	cd $(F_DEBUG) && $(MAKE) all

mencheck: debug


### Compile
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(CFLAGS)

$(F_BIN)/%.o: $(F_SRC)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)
