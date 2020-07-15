#Author: Eduardo Souza Rocha

#Copilation
CC       := gcc

#Output
EXEC     := enduser
DEBUG    := debug
ZIP      := $(EXEC).zip

#Files
INCLUDES := $(wildcard src/opcoes/*.c) $(wildcard src/opcoes/*.h)
SRC      := $(wildcard src/*.c)
OBJ      := $(SRC:src/%.c=bin/%.o)
HEADER   := $(wildcard src/*.h)
MK       := Makefile

#Flags
CFLAGS   := -Wall -lm -Wextra -pedantic -Werror=implicit-function-declaration 
# CFLAGS   := CFLAGS -fsanitize=address
RMFLAGS  := -f -v

#Actions
all: 
	mkdir -p bin
	make $(EXEC)

run: $(EXEC)
	./$(EXEC)

compile: clean $(EXEC)

zip: clean_zip
	zip $(ZIP) $(SRC) $(HEADER) $(INCLUDES) $(MK)

#Clean
clean_zip:
	rm $(ZIP) $(RMFLAGS)

clean:
	rm $(OBJ) $(RMFLAGS)
	rm $(DEBUG) $(RMFLAGS)
	rm $(EXEC) $(RMFLAGS)
	rm $(ZIP) $(RMFLAGS)


#Debug
test:
	sh test.sh $(EXEC)

lines:
	sh lines.sh

debug: $(OBJ)
	$(CC) -g -o $(DEBUG) $(SRC)

mencheck: debug


#Compile
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(CFLAGS)

bin/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)
