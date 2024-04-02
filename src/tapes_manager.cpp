#include "../include/tapes_manager.hpp"
#include "../include/tape_interface.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#define BIN_ZERO "00000000000000000000000000000000"

#define N 10
#define M 6

 std::vector<int32_t> merge_vectors(std::vector<int32_t> vector1, std::vector<int32_t> vector2)
 {
    std::vector<int32_t> result;

    size_t len1 = vector1.size();
    size_t len2 = vector2.size();

    size_t i = 0;
    size_t j = 0;

    while((i != len1) && (j != len2))
    {
        if(vector1[i] > vector2[j])
        {
            result.push_back(vector2[j]);
            ++j;
        }
        else
        {
            result.push_back(vector1[i]);
            ++i;
        }
    }

    if(i == len1)
        for(; j != len2; ++j)
            result.push_back(vector2[j]);

    if(j == len2)
        for(; i != len1; ++i)
            result.push_back(vector1[i]);

    return result;
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

    size_t readen_from_tape1 = 0;
    size_t readen_from_tape2 = 0;

    while((readen_from_tape1 == tape_1.tape_size) && (readen_from_tape2 == tape_2.tape_size)) // read M/2 numbers to tmp vector
    {
        #ifdef DEBUG
        std::cout << "readen_from_tape1 = " << readen_from_tape1 << std::endl;
        std::cout << "readen_from_tape2 = " << readen_from_tape2 << std::endl;
        #endif

        tape_interface.swap_tape(tape_1);
        tape_interface.move_right(readen_from_tape1);

        #ifdef DEBUG
        std::cout << "curssor1 = " << tape_interface.cursor_pos << std::endl;
        #endif

        std::vector<int32_t> numbers_from_tape1 = tape_interface.read_to_vector(M/2);

        #ifdef DEBUG
        std::vector<int32_t> nums1 = tape_interface.read_to_vector(len1);
        std::cout << "vector1:";
        for(int i = 0; i < len1; ++i)
        {
            std::cout << nums1[i] << ' ';
        }
        std::cout << std::endl;
        #endif

        tape_interface.swap_tape(tape_2);
        tape_interface.move_right(readen_from_tape2);

        #ifdef DEBUG
        std::cout << "curssor2 = " << tape_interface.cursor_pos << std::endl;
        #endif

        std::vector<int32_t> numbers_from_tape2 = tape_interface.read_to_vector(M/2);

        #ifdef DEBUG
        std::vector<int32_t> nums2 = tape_interface.read_to_vector(len2);
        std::cout << "vector2: ";
        for(int i = 0; i < len1; ++i)
        {
            std::cout << nums2[i] << ' ';
        }
        std::cout << std::endl;
        #endif

        std::vector<int32_t> merged_numbers = merge_vectors(numbers_from_tape1, numbers_from_tape2);

        #ifdef DEBUG
        std::cout << "merged vector: ";
        for(int i = 0; i < merged_numbers.size(); ++i)
            std::cout << merged_numbers[i] << ' ';
        std::cout << std::endl;
        #endif

        tape_interface.swap_tape(merged_tape);
        tape_interface.move_right(readen_from_tape1 + readen_from_tape2);
        tape_interface.write_vector(merged_numbers);

        readen_from_tape1 += numbers_from_tape1.size();
        readen_from_tape2 += numbers_from_tape2.size();
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