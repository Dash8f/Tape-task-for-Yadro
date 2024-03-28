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
    size_t currsor_pos = 0;

public:
    TapeInterface( size_t write_delay_, 
                   size_t read_delay_, 
                   size_t move_delay_,
                   size_t swap_delay_) : 
                   write_delay(write_delay_),
                   read_delay(read_delay_),
                   move_delay(move_delay_),
                   swap_delay(swap_delay_) {};

    ~TapeInterface() = default;

    int32_t read();
    void write(int32_t number);

    void move_next_right();
    void move_right(size_t cells_to_move);

    void move_next_left();
    void move_left(size_t cells_to_move);


    void swap_tape(std::string new_tape);
};

//std::ios::in | std::ios::out | std::ios::app - для чтения и записи