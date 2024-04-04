CXX_FLAGS = -std=c++17 -fsanitize=address -g -ggdb3 -W -Iinclude

OBJECTS = obj/tape_interface.o obj/tapes_manager.o obj/main.o

TRGT = tapes

all: $(TRGT) data_generator

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(CXX_FLAGS) -c -o $@ $<

$(TRGT): $(OBJECTS)
	g++ $(OBJECTS) -o $(TRGT) $(CXX_FLAGS)

run: $(TRGT)
	@./$(TRGT) data/data.txt data/sorted.txt

clean:
	rm -rf obj
	@echo '.o files are cleaned'

new_data:
	@./data_generator

data_generator: data/data_generator.cpp
	g++  -std=c++17 -W -Iinclude -o $@ $<