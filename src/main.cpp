#include "../include/tape_interface.hpp"
#include "../include/tapes_manager.hpp"

#include <iostream>
#include <string>

size_t get_token_value_from_config(std::string config, std::string token_name);

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cout << "not enough arguments" << std::endl;
        return 0;
    }

    std::string data_file_name = argv[1];
    std::string sorted_file_name = argv[2];

    size_t size_of_data_tape = get_token_value_from_config("config.txt", "size_of_data_tape");
    size_t memory_capacity = get_token_value_from_config("config.txt", "memory_capacity");

    Tape data_tape{data_file_name, size_of_data_tape};

    std::cout << "Data file path: " <<  data_file_name << std::endl;
    std::cout << "Sorted file path: " <<  sorted_file_name << std::endl;
    TapeInterface tape_interface("config.txt");
    std::cout << "Size of data_file = " << size_of_data_tape << std::endl;
    std::cout << "Memory capacity = " << memory_capacity << std::endl;

    TapesManager tapes_manager{tape_interface, memory_capacity};

    Tape sorted_tape = tapes_manager.sort_tape(sorted_file_name, data_tape);

    std::cout << "Result file path: " << sorted_tape.tape_name << std::endl;

    return 0;
} 