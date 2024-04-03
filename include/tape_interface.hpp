#pragma once

#include <iostream>
#include <vector>
#include <fstream>

class Tape
{
public:
    std::string tape_name;
    size_t tape_size;
    std::fstream* tape_file;
public:
    Tape(std::string tape_name_, size_t tape_size_);
    Tape(std::string tape_name_, Tape& tape_for_copying);
    ~Tape() = default;
};

class TapeInterface
{
public:
private:
    size_t write_delay;
    size_t read_delay;
    size_t move_delay;
    size_t swap_delay;
private:
    Tape* tape;
    size_t cursor_pos = 0;
public:
    TapeInterface(std::string config_file_name) ;
    ~TapeInterface() = default;

    int32_t read();
    std::vector<int32_t> read_to_vector(size_t vector_size);

    void write(int32_t number);
    void write_vector(std::vector<int32_t> numbers);

    void move_next_right();
    void move_right(size_t cells_to_move);

    void move_next_left();
    void move_left(size_t cells_to_move);

    void swap_tape(Tape& new_tape);

    void print_interface_settings();
};