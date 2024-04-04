#include "tape_interface.hpp"

#include "colors.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <chrono>
#include <thread>

#define size_of_number 32

#define BIN_ZERO "00000000000000000000000000000000"

#define symbol_of_start   '<'
#define symbol_of_end     '>'
#define split_symbol ' '

void delay(int milliseconds) 
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int string_to_int(std::string number_str) {
    int number;
    std::stringstream ss(number_str);

    ss >> number;

    return number;
}

size_t get_token_value_from_config(std::string config, std::string token_name)
{
    std::ifstream file(config);
    std::string line;
    std::string size_value;

    while (std::getline(file, line)) 
    {
        size_t pos = line.find(token_name);
        if (pos != std::string::npos) 
        {
            size_t equal_sign_pos = line.find("=", pos);
            if (equal_sign_pos != std::string::npos) 
            {
                file.close();
                return  string_to_int(line.substr(equal_sign_pos + 1));
            }
        }
    }

    return 0;
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
        for(size_t i = 0; i < tape_size_;  ++i)
        {
            tmp <<  BIN_ZERO << split_symbol;
        }

        tmp.seekp(-1, std::ios_base::cur);
        tmp << symbol_of_end;

        tmp.close();
    }
    else
    {
        tape_file->close();
    }
}

TapeInterface::TapeInterface(std::string config_file_name)
{
    write_delay = get_token_value_from_config(config_file_name, "write_delay");
    read_delay = get_token_value_from_config(config_file_name, "read_delay");
    move_delay = get_token_value_from_config(config_file_name, "move_delay");
    swap_delay = get_token_value_from_config(config_file_name, "swap_delay");

    tape = NULL;
}

int32_t TapeInterface::read() 
{
    delay(read_delay);

    tape->tape_file->open(tape->tape_name, std::ios::in);
    
    tape->tape_file->seekg(1 + cursor_pos * (size_of_number + 1));

    std::string binary_number = "";

    char symbol = ' ';
    for(int cnt = 0; cnt < size_of_number; ++cnt)
    {
        tape->tape_file->get(symbol);
        binary_number += symbol;
    }

    tape->tape_file->close();
    return binary_string_to_decimal(binary_number);
}

std::vector<int32_t> TapeInterface::read_to_vector(size_t vector_size)
{
    size_t prev_currsor_pos = cursor_pos;

    std::vector<int32_t> numbers;

    if(vector_size > (tape->tape_size - cursor_pos))
        vector_size = tape->tape_size - cursor_pos;

    for(size_t cnt = 0; cnt < vector_size; ++cnt)
    {   
        numbers.push_back(read());
        move_next_right();
    }

    cursor_pos = prev_currsor_pos;

    return numbers;
}

void TapeInterface::write(int32_t number)
{
    delay(write_delay);

    tape->tape_file->open(tape->tape_name, std::ios::in | std::ios::out | std::ios::ate);

    std::string binary_number = decimal_to_binary_string(number);

    tape->tape_file->seekp(1 + cursor_pos * (size_of_number + 1));

    *tape->tape_file << binary_number;

    tape->tape_file->close();
}

void TapeInterface::write_vector(std::vector<int32_t> numbers)
{
    size_t prev_currsor_pos = cursor_pos;

    for(size_t cnt = 0; cnt < numbers.size(); ++cnt)
    {   
        write(numbers[cnt]);
        move_next_right();
    }

    cursor_pos = prev_currsor_pos;
}

void TapeInterface::move_next_right()
{
    delay(move_delay);

    if(cursor_pos + 1 < tape->tape_size)
        ++cursor_pos;
}

void TapeInterface::move_right(size_t cells_to_move)
{
    for(size_t cnt = 0; cnt < cells_to_move; ++cnt)
        move_next_right();
}

void TapeInterface::move_next_left()
{
    delay(move_delay);
    if(cursor_pos > 0)
        --cursor_pos;
}

void TapeInterface::move_left(size_t cells_to_move)
{
    for(size_t cnt = 0; cnt < cells_to_move; ++cnt)
        move_next_left();
}

void TapeInterface::swap_tape(Tape& new_tape)
{
    delay(swap_delay);

    tape = &new_tape;

    cursor_pos = 0;
}

void TapeInterface::print_interface_settings()
{
    std::cout << "TapeInterface settings:" << std::endl;

    std::cout << "Write delay is " << MAGENTA << write_delay << " ms" << RESET << std::endl;
    std::cout << "Read delay is "  << MAGENTA << read_delay  << " ms" << RESET <<std::endl;
    std::cout << "Move delay is "  << MAGENTA << move_delay  << " ms" << RESET <<std::endl;
    std::cout << "Swap delay is "  << MAGENTA << swap_delay  << " ms" << RESET <<std::endl;
}

Tape::Tape(std::string tape_name_, Tape& tape_for_copying)
{
    tape_size = tape_for_copying.tape_size;
    tape_name = tape_name_;

    tape_file = new std::fstream;
    tape_file->open(tape_name_, std::ios::out | std::ios::trunc);

    tape_for_copying.tape_file->open(tape_for_copying.tape_name, std::ios::in);

    std::string text_from_tape;
    std::getline(*tape_for_copying.tape_file, text_from_tape);

    *tape_file >>  text_from_tape;

    tape_file->close();
}