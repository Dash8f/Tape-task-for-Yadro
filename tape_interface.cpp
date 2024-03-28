#include "tape_interface.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <chrono>
#include <thread>

#define size_of_number 32

#define symbol_of_start   '<'
#define symbol_of_end     '>'

#define read_delay_indentation   14
#define write_delay_indentation  18
#define move_delay_indentation   18
#define swap_delay_indentation   18



void delay(int milliseconds) 
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int binary_string_to_decimal(const std::string& binary_str)
{
    int decimal = 0;
    int binary_len = binary_str.length();
    
    for (int i = 1; i < binary_len; ++i) 
    {
        if (binary_str[i] == '1') 
        {
            decimal += static_cast<int>(pow(2, binary_len - i - 1));
        }
    }

    if(binary_str[0] == '0')
        return decimal;

    return ((-1) * decimal);
}

std::string decimal_to_binary_string(int number)
{
    std::string binary_str;

    char first_symbol = '0';

    if(number < 0)
    {
        number = -number;
        first_symbol = '1';
    }
    
    while (number > 0) 
    {
        binary_str = std::to_string(number % 2) + binary_str;
        number /= 2;
    }
    
    while (binary_str.length() < size_of_number - 1) 
    {
        binary_str = "0" + binary_str;
    }

    binary_str = first_symbol + binary_str;

    return binary_str;
}

TapeInterface::TapeInterface(std::string config_file_name)
{
    std::fstream config_file;

    config_file.open(config_file_name, std::ios::in);

    if(!config_file.is_open())
    {
        std::cout << "Tape interface was not created" << std::endl;
        return;
    }

    std::cout << "Config file was openen. Config settings:" << std::endl;

    config_file.seekg(read_delay_indentation, std::ios_base::cur); //TODO -=-
    config_file >> write_delay;
    std::cout << "Write delay is " << write_delay << " ms" << std::endl;

    config_file.seekg(write_delay_indentation, std::ios_base::cur);
    config_file >> read_delay;
    std::cout << "Read delay is " << read_delay << " ms" <<std::endl;

    config_file.seekg(move_delay_indentation, std::ios_base::cur);
    config_file >> move_delay;
    std::cout << "Move delay is " << move_delay << " ms" <<std::endl;

    config_file.seekg(swap_delay_indentation, std::ios_base::cur);
    config_file >> swap_delay;
    std::cout << "Swap delay is " << swap_delay << " ms" <<std::endl;
}

int32_t TapeInterface::read() 
{
    delay(read_delay);
    
    cur_tape.seekg(1, std::ios_base::cur);

    std::string binary_number;

    char symbol;
    for(int cnt = 0; cnt < size_of_number; ++cnt)
    {
        cur_tape.get(symbol);
        binary_number += symbol;
    }

    cur_tape.seekg(-(size_of_number + 1), std::ios_base::cur);

    return binary_string_to_decimal(binary_number);
}

void TapeInterface::write(int32_t number)
{
    delay(write_delay);

    std::string binary_number = decimal_to_binary_string(number);

    cur_tape.seekp(1, std::ios_base::cur);

    cur_tape << binary_number;

    cur_tape.seekp(-(size_of_number + 1), std::ios_base::cur);
}

void TapeInterface::move_next_right()
{
    delay(move_delay);

    cur_tape.seekg((size_of_number + 1), std::ios_base::cur);

    char symbol;
    cur_tape.get(symbol);
    cur_tape.seekg(-1, std::ios_base::cur);

    if(symbol == symbol_of_end)
    {
        cur_tape.seekg(-(size_of_number + 1), std::ios_base::cur);
    }
}

void TapeInterface::move_right(size_t cells_to_move)
{
    for(int cnt = 0; cnt < cells_to_move; ++cnt)
        move_next_right();
}

void TapeInterface::move_next_left()
{
    delay(move_delay);

    char symbol;
    cur_tape.get(symbol);
    cur_tape.seekg(-1, std::ios_base::cur);

    if(symbol == symbol_of_start)
    {
        return;
    }

    cur_tape.seekg(-(size_of_number + 1), std::ios_base::cur);
}

void TapeInterface::move_left(size_t cells_to_move)
{
    for(int cnt = 0; cnt < cells_to_move; ++cnt)
        move_next_left();
}

void TapeInterface::swap_tape(std::string new_tape_name)
{
    delay(swap_delay);

    if(cur_tape.is_open())
    {
        cur_tape.close();
        std::cout << "old file was closed" << std::endl;
    }

    cur_tape.open(new_tape_name, std::ios::in | std::ios::out);

    if(!cur_tape.is_open())
    {
        std::cout << "File " << new_tape_name << " didn't open" << std::endl;
    }
}
