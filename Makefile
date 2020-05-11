#Author: Eduardo Souza Rocha

#Copilation
CC       := gcc

#Output
EXEC     := enduser
DEBUG    := debug
ZIP      := $(EXEC).zip

#Files
SRC      := $(wildcard *.c)
OBJ      := $(SRC:.c=.o)
HEADER   := $(wildcard *.h)
MK       := Makefile
TEST_IN  := TesteIn/
TEST_OUT := TesteOut/

#Flags
CFLAGS   := -Wall -lm -Wextra -pedantic -Werror=implicit-function-declaration
RMFLAGS  := -f -v
IN       := 1..7


#Actions
all: $(EXEC)

run: $(EXEC)
	./$(EXEC)

compile: clean $(EXEC)

zip: clean_zip
	zip $(ZIP) $(SRC) $(HEADER) $(MK)


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
	cp $(TEST_IN)* $(TEST_OUT)
	cp $(EXEC) $(TEST_OUT)
	cd $(TEST_OUT)
	echo "for i in {$(IN)}; do ./$(EXEC) < \$$i.in > \$$i.res || break; done" | bash
	cd ..

debug: $(OBJ)
	$(CC) -g -o $(DEBUG) $(SRC)

mencheck: debug


#Compile
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

