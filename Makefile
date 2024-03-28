CXX_FLAGS = -std=c++17

TRGT = tapes

all: tape_interface.o main.o
	@g++ tape_interface.o main.o -o $(TRGT) $(CXX_FLAGS)
	@echo '$(TRGT) has been done'

tape_interface.o: tape_interface.cpp
	@g++ -c -o tape_interface.o tape_interface.cpp $(CXX_FLAGS)

main.o: main.cpp
	@g++ -c -o main.o main.cpp $(CXX_FLAGS)

run: $(TRGT)
	@./$(TRGT)

clean:
	@rm -f *.o
	@echo '.o files are cleaned'