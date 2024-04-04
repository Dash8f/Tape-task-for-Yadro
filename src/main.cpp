#include "tape_interface.hpp"
#include "tapes_manager.hpp"

#include "colors.hpp"

#include <iostream>
#include <string>

#include <chrono>

void print_info(std::string data_file_name, std::string sorted_file_name, 
                size_t size_of_data_tape, size_t memory_capacity)
{
    std::cout << "Data file path: " <<  GREEN << data_file_name << RESET << std::endl;
    std::cout << "Sorted file path: " <<  GREEN << sorted_file_name << RESET << std::endl;
    std::cout << "Size of data_file = " << YELLOW << size_of_data_tape << RESET <<std::endl;
    std::cout << "Memory capacity = " << YELLOW << memory_capacity << RESET << std::endl;
}

size_t get_token_value_from_config(std::string config, std::string token_name);

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cout << RED << "not enough arguments" << RESET << std::endl;
        return 0;
    }
    auto begin = std::chrono::steady_clock::now();

    std::string data_file_name = argv[1];
    std::string sorted_file_name = argv[2];

    size_t size_of_data_tape = get_token_value_from_config("config.txt", "size_of_data_tape");
    size_t memory_capacity = get_token_value_from_config("config.txt", "memory_capacity");

    if(memory_capacity < 2)
    {
        std::cout << RED <<"Not enough memory" << RESET << std::endl;
        return 0;
    }
    
    print_info(data_file_name, sorted_file_name, size_of_data_tape, memory_capacity);

    Tape data_tape{data_file_name, size_of_data_tape};
    TapeInterface tape_interface("config.txt");
    tape_interface.print_interface_settings();
    TapesManager tapes_manager{tape_interface, memory_capacity};

    Tape sorted_tape = tapes_manager.sort_tape(sorted_file_name, data_tape);
    std::cout << "Result file path: " << GREEN << sorted_tape.tape_name << RESET << std::endl;

    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout <<  "Execution time: " << BLUE << elapsed_ms.count() << " ms" << RESET << std::endl;

    return 0;
} 