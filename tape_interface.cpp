#include "tape_interface.hpp"

#include <iostream>
#include <string>

#include <chrono>
#include <thread>

void delay(int milliseconds) 
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int binaryToDecimal(const std::string& binary_str) 
{
    int decimal = 0;
    int binary_len = binary_str.length();
    
    for (int i = 0; i < binary_len; ++i) 
    {
        if (binary_str[i] == '1') 
        {
            decimal += static_cast<int>(pow(2, binary_len - i - 1));
        }
    }
    
    return decimal;
}

std::string intToBinaryString(int number) 
{
    std::string binaryString;
    
    while (number > 0) 
    {
        binaryString = std::to_string(number % 2) + binaryString;
        number /= 2;
    }
    
    while (binaryString.length() < 32) 
    {
        binaryString = "0" + binaryString;
    }
    
    return binaryString;
}

int32_t TapeInterface::read() 
{
    delay(read_delay);
    
    cur_tape.seekg(1, std::ios_base::cur);

    std::string binary_number;

    char symbol;
    for(int cnt = 0; cnt < 32; ++cnt)
    {
        cur_tape.get(symbol);
        binary_number += symbol;
    }

    cur_tape.seekg(-33, std::ios_base::cur);

    return binaryToDecimal(binary_number);
}

void TapeInterface::write(int32_t number)
{
    delay(write_delay);

    std::string binary_number = intToBinaryString(number);

    cur_tape.seekp(1, std::ios_base::cur);

    cur_tape << binary_number;

    cur_tape.seekp(-33, std::ios_base::cur);
}

void TapeInterface::move_next_right()
{
    delay(move_delay);

    cur_tape.seekg(33, std::ios_base::cur);

    char symbol;
    cur_tape.get(symbol);
    cur_tape.seekg(-1, std::ios_base::cur);

    if(symbol == '>')
    {
        cur_tape.seekg(-33, std::ios_base::cur);
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

    if(symbol == '<')
    {
        return;
    }

    cur_tape.seekg(-33, std::ios_base::cur);
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
        std::cout << "old file closed" << std::endl;
    }

    cur_tape.open(new_tape_name, std::ios::in | std::ios::out);

    if(!cur_tape.is_open())
    {
        std::cout << "File" << new_tape_name << "didn`t open" << std::endl;
    }

    // std::cout << "File " << new_tape_name << " opened" << std::endl;

    currsor_pos = 0;
}
