#include "tape_interface.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <chrono>
#include <thread>

#define size_of_number 32

#define BIN_ZERO "00000000000000000000000000000000"

#define symbol_of_start   '<'
#define symbol_of_end     '>'
#define split_symbol ' '

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

Tape::Tape(std::string tape_name_, size_t tape_size_)
{
    tape_size = tape_size_;
    tape_name = tape_name_;

    tape_file = new std::fstream;

    tape_file->open(tape_name_);

    if (!tape_file->is_open()) 
    {
        std::fstream tmp(tape_name_);

        tmp.open(tape_name_, std::ios::out);

        tmp << symbol_of_start;
        for(int i = 0; i < tape_size_;  ++i)
        {
            tmp <<  BIN_ZERO << split_symbol;
        }

        tmp.seekp(-1, std::ios_base::cur);
        tmp << symbol_of_end;

        tmp.close();
    }
    else
    {
        tape_file->seekp(0, std::ios_base::beg);
        tape_file->seekg(0, std::ios_base::beg);
        tape_file->close();
    }
}

TapeInterface::TapeInterface(std::string config_file_name, Tape& first_tape) : tape(first_tape)
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

    tape.tape_file->open(tape.tape_name, std::ios::in);
    
    tape.tape_file->seekg(1 + curros_pos * (size_of_number + 1));

    std::string binary_number = "";

    char symbol = ' ';
    for(int cnt = 0; cnt < size_of_number; ++cnt)
    {
        tape.tape_file->get(symbol);
        binary_number += symbol;
    }

    tape.tape_file->close();
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

    tape.tape_file->open(tape.tape_name, std::ios::in | std::ios::out | std::ios::ate);

    std::string binary_number = decimal_to_binary_string(number);

    tape.tape_file->seekp(1 + curros_pos * (size_of_number + 1));

    *tape.tape_file << binary_number;

    tape.tape_file->close();
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

    if(curros_pos + 1 < tape.tape_size)
        ++curros_pos;
}

void TapeInterface::move_right(size_t cells_to_move)
{
    for(int cnt = 0; cnt < cells_to_move; ++cnt)
        move_next_right();
}

void TapeInterface::move_next_left()
{
    delay(move_delay);
    if(curros_pos > 0)
        --curros_pos;
}

void TapeInterface::move_left(size_t cells_to_move)
{
    for(int cnt = 0; cnt < cells_to_move; ++cnt)
        move_next_left();
}

void TapeInterface::swap_tape(Tape& new_tape)
{
    delay(swap_delay);
    tape = new_tape;
    curros_pos = 0;
}

