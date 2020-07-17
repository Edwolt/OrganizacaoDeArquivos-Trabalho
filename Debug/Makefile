all: printI arvoreB

RMFLAGS := -f -v
CFLAGS  := -g -fsanitize=address

printI:
	g++ printI.cpp -o ../printI.out $(CFLAGS)

arvoreB:
	g++ arvoreB.cpp -o ../arvoreB.out $(CFLAGS)

clean:
	rm ../printI.out $(RMFLAGS)
	rm ../arvoreB.out $(RMFLAGS)