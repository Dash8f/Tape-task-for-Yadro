#include "tapes_manager.hpp"
#include "tape_interface.hpp"

#include <iostream>
#include <fstream>
#include <string>

#define BIN_ZERO "00000000000000000000000000000000"

#define N 10
#define M 3

void TapesManager::delete_tape(Tape tape)
{
    int result = remove(tape.tape_name.c_str());

    delete &tape;
}

void TapesManager::sort_short_tape(Tape& tape)
{
    std::vector<int32_t> tmp;

    tape_interface.swap_tape(tape);
    for(int i = 0; i < tape.tape_size; ++i)
    {
        tmp.push_back(tape_interface.read());
        tape_interface.move_next_right();
    }
    std::sort(tmp.begin(), tmp.end());

    tape_interface.move_left(tape.tape_size);
    for(int i = 0; i < tape.tape_size; ++i)
    {
        tape_interface.write(tmp[i]);
        tape_interface.move_next_right();
    }
}

Tape TapesManager::merge_sorted_tapes(std::string new_tape_name,  Tape& tape_1, Tape& tape_2)
{ 
    Tape merged_tape{new_tape_name, (tape_1.tape_size + tape_2.tape_size)};

    size_t first_pointer  = 0;
    size_t second_pointer = 0;

    int32_t first_number, second_number;

    while((first_pointer  != tape_1.tape_size) && (second_pointer  != tape_2.tape_size) ) // read M/2 numbers to tmp vector
    {
        tape_interface.swap_tape(tape_1);
        tape_interface.move_right(first_pointer);
        first_number = tape_interface.read();

        tape_interface.swap_tape(tape_2);
        tape_interface.move_right(second_pointer);
        second_number = tape_interface.read();

        tape_interface.swap_tape(merged_tape);
        tape_interface.move_right(first_pointer + second_pointer);
        if(first_number < second_number)
        {
            tape_interface.write(first_number);
            ++first_pointer;
        }
        else
        {
            tape_interface.write(second_number);
            ++second_pointer;
        }
    }

    if(first_pointer  == tape_1.tape_size)
    {
        for(; second_pointer < tape_2.tape_size; ++second_pointer)
        {
            tape_interface.swap_tape(tape_2);
            tape_interface.move_right(second_pointer);
            int number = tape_interface.read();

            tape_interface.swap_tape(merged_tape);
            tape_interface.move_right(first_pointer + second_pointer);
            tape_interface.write(number);
        }
    }

    if(second_pointer == tape_2.tape_size)
    {
        for(; first_pointer < tape_1.tape_size; ++first_pointer)
        {
            tape_interface.swap_tape(tape_1);
            tape_interface.move_right(first_pointer);
            int number = tape_interface.read();

            tape_interface.swap_tape(merged_tape);
            tape_interface.move_right(first_pointer + second_pointer);
            tape_interface.write(number);
        }
    }
    return merged_tape;
}

Tape TapesManager::merge_sort_tape(Tape& tape_to_sort)
{
    size_t amount_of_files = N / M;

    size_t main_pointer = 0;

    for(size_t files_cnt = 0; files_cnt < amount_of_files; ++files_cnt)
    {
        tape_interface.swap_tape(tape_to_sort);
        tape_interface.move_right(main_pointer);
        std::vector<int32_t> numbers = tape_interface.read_to_vector(M);
        main_pointer += M;

        Tape tmp_tape{"/tmp/" + std::to_string(files_cnt) + ".txt", M};

        tape_interface.swap_tape(tmp_tape);
        tape_interface.write_vector(numbers);
    }

    if(N % M != 0)
    {
        ++amount_of_files;

        tape_interface.swap_tape(tape_to_sort);
        tape_interface.move_right(main_pointer);
        std::vector<int32_t> numbers = tape_interface.read_to_vector(N % M);
        main_pointer += (N % M);

        Tape tmp_tape{"/tmp/" + std::to_string(amount_of_files - 1) + ".txt", N % M};

        tape_interface.swap_tape(tmp_tape);
        tape_interface.write_vector(numbers);
    }

    Tape wtf_tape{"test", 10};

    return wtf_tape;
}