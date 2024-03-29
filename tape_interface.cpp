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

    bool is_negative = (binary_str[0] == '1');

    if (is_negative) 
    {
        std::string inverted_binary = binary_str;
        for (char& c : inverted_binary) 
            c = (c == '0') ? '1' : '0';

        for (size_t i = 0; i < inverted_binary.length(); ++i) 
        {
            if (inverted_binary[i] == '1') 
                decimal += static_cast<int>(pow(2, inverted_binary.length() - i - 1));
        }

        decimal = -decimal - 1;
    }
    else
    {
        for (size_t i = 0; i < binary_str.length(); ++i) 
        {
            if (binary_str[i] == '1') 
                decimal += static_cast<int>(pow(2, binary_str.length() - i - 1));
        }
    }

    return decimal;
}

std::string decimal_to_binary_string(int num) 
{
    std::string binary_str = "";

    bool is_negative = num < 0;

    if (is_negative) 
        num = -num - 1;

    while (num > 0)
    {
        binary_str = (num % 2 == 0 ? "0" : "1") + binary_str;
        num /= 2;
    }

    if (is_negative) 
    {
        for (char& c : binary_str) 
            c = (c == '0') ? '1' : '0';

        while (binary_str.length() < 32)
            binary_str = "1" + binary_str;
    }
    else
    {
        while (binary_str.length() < 32)
            binary_str = "0" + binary_str;
    }

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

    config_file.close();
}

int32_t TapeInterface::read() 
{
    delay(read_delay);
    
    cur_tape->tape_file.seekg(1, std::ios_base::cur);

    std::string binary_number;

    char symbol;
    for(int cnt = 0; cnt < size_of_number; ++cnt)
    {
        cur_tape->tape_file.get(symbol);
        binary_number += symbol;
    }

    cur_tape->tape_file.seekg(-(size_of_number + 1), std::ios_base::cur);

    return binary_string_to_decimal(binary_number);
}

std::vector<int32_t> TapeInterface::read_to_vector(size_t vector_size)
{
    std::vector<int32_t> numbers;

    for(size_t cnt = 0; cnt < vector_size; ++cnt)
    {   
        numbers.push_back(read());
        move_next_right();
    }

    return numbers;
}

void TapeInterface::write(int32_t number)
{
    delay(write_delay);

    std::string binary_number = decimal_to_binary_string(number);

    cur_tape->tape_file.seekp(1, std::ios_base::cur);

    cur_tape->tape_file << binary_number;

    cur_tape->tape_file.seekp(-(size_of_number + 1), std::ios_base::cur);
}

void TapeInterface::write_vector(std::vector<int32_t> numbers)
{
    for(size_t cnt = 0; cnt < numbers.size(); ++cnt)
    {   
        write(numbers[cnt]);
        move_next_right();
    }
}

void TapeInterface::move_next_right()
{
    delay(move_delay);

    cur_tape->tape_file.seekg((size_of_number + 1), std::ios_base::cur);

    char symbol;
    cur_tape->tape_file.get(symbol);
    cur_tape->tape_file.seekg(-1, std::ios_base::cur);

    if(symbol == symbol_of_end)
    {
        cur_tape->tape_file.seekg(-(size_of_number + 1), std::ios_base::cur);
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
    cur_tape->tape_file.get(symbol);
    cur_tape->tape_file.seekg(-1, std::ios_base::cur);

    if(symbol == symbol_of_start)
    {
        return;
    }

    cur_tape->tape_file.seekg(-(size_of_number + 1), std::ios_base::cur);
}

void TapeInterface::move_left(size_t cells_to_move)
{
    for(int cnt = 0; cnt < cells_to_move; ++cnt)
        move_next_left();
}

void TapeInterface::swap_tape(std::string new_tape_name)
{
    delay(swap_delay);

    if(cur_tape->tape_file.is_open())
    {
        cur_tape->tape_file.close();
        std::cout << "old file was closed" << std::endl;
    }

    cur_tape->tape_file.open(new_tape_name, std::ios::in | std::ios::out);

    if(!cur_tape->tape_file.is_open())
    {
        //std::cout << "File " << new_tape_name << " didn't open" << std::endl;
    }
}

void TapeInterface::swap_tape(Tape* new_tape)
{
    cur_tape = new_tape;

    if(!cur_tape->tape_file.is_open())
        cur_tape->tape_file.open(cur_tape->tape_name, std::ios::in | std::ios::out);

    cur_tape->tape_file.seekp(0, std::ios_base::beg);
    cur_tape->tape_file.seekg(0, std::ios_base::beg);

    //std::cout << cur_tape->tape_name << std::endl;
}

