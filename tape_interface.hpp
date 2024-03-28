#pragma once

#include <iostream>
#include <fstream>

class TapeInterface
{
private:
    size_t write_delay;
    size_t read_delay;
    size_t move_delay;
    size_t swap_delay;
private:
    std::fstream cur_tape;
public:
    TapeInterface(std::string config_file_name);
    ~TapeInterface() = default;

    int32_t read();
    void write(int32_t number);

    void move_next_right();
    void move_right(size_t cells_to_move);

    void move_next_left();
    void move_left(size_t cells_to_move);


    void swap_tape(std::string new_tape_name);
};