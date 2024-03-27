CXX_FLAGS = -std=c++17

TRGT = tapes

all: tapes.o main.o
	@g++ tapes.o main.o -o $(TRGT) $(CXX_FLAGS)
	@echo '$(TRGT) has been done'

tapes.o: tapes.cpp
	@g++ -c -o tapes.o tapes.cpp $(CXX_FLAGS)

main.o: main.cpp
	@g++ -c -o main.o main.cpp $(CXX_FLAGS)

run: $(TRGT)
	@./$(TRGT)

clean:
	@rm -f *.o
	@echo '.o files are cleaned'