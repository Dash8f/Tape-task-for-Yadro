#pragma once

#include "tape_interface.hpp"

#include <iostream>
#include <vector>
#include <fstream>

class TapesManager
{
private:
    size_t memory_capacity = 2;
    TapeInterface& tape_interface;
public:
    TapesManager(TapeInterface& tape_interface_, size_t memory_capacity_) : 
                                tape_interface(tape_interface_), memory_capacity(memory_capacity_) {};
    ~TapesManager() = default;

    Tape sort_tape(std::string sorted_file_name, Tape tape_to_sort);
private:
    Tape merge_sorted_tapes(std::string new_tape_name, Tape& tape_1, Tape& tape_2);
    std::vector<Tape> split_tape(Tape& tape_to_sort);
    Tape sort_tape_(std::string sorted_file_name, std::vector<Tape> vector_of_tapes);

};
