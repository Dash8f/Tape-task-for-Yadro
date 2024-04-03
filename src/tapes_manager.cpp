#include "../include/tapes_manager.hpp"
#include "../include/tape_interface.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

void delete_file(std::string file_name)
{
    std::filesystem::permissions(file_name, std::filesystem::perms::all);
    std::filesystem::remove(file_name);
}

std::string cut_tape_name(std::string tape_name)
{
    size_t lastSlashPos = tape_name.find_last_of('/');
    tape_name = tape_name.substr(lastSlashPos + 1);

    size_t dotPos = tape_name.find_last_of('.');
    tape_name = tape_name.substr(0, dotPos);

    return tape_name;
}

std::string make_merged_tape_name(Tape tape1, Tape tape2)
{
    std::string name1 = cut_tape_name(tape1.tape_name);
    std::string name2 = cut_tape_name(tape2.tape_name);

    return name1 + name2 + ".txt"; 
}

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

Tape TapesManager::merge_sorted_tapes(std::string new_tape_name,  Tape& tape_1, Tape& tape_2)
{ 
    Tape merged_tape{new_tape_name, (tape_1.tape_size + tape_2.tape_size)};

    size_t readen_from_tape1 = 0;
    size_t readen_from_tape2 = 0;

    while((readen_from_tape1 != tape_1.tape_size) && (readen_from_tape2 != tape_2.tape_size)) // read M/2 numbers to tmp vector
    {
        tape_interface.swap_tape(tape_1);
        tape_interface.move_right(readen_from_tape1);

        std::vector<int32_t> numbers_from_tape1 = tape_interface.read_to_vector(memory_capacity/2);

        tape_interface.swap_tape(tape_2);
        tape_interface.move_right(readen_from_tape2);

        std::vector<int32_t> numbers_from_tape2 = tape_interface.read_to_vector(memory_capacity/2);

        std::vector<int32_t> merged_numbers = merge_vectors(numbers_from_tape1, numbers_from_tape2);

        tape_interface.swap_tape(merged_tape);
        tape_interface.move_right(readen_from_tape1 + readen_from_tape2);
        tape_interface.write_vector(merged_numbers);

        readen_from_tape1 += numbers_from_tape1.size();
        readen_from_tape2 += numbers_from_tape2.size();
    }

    if(readen_from_tape1 == tape_1.tape_size)
    {
        while(readen_from_tape2 != tape_2.tape_size)
        {
            tape_interface.swap_tape(tape_2);
            tape_interface.move_right(readen_from_tape2);
            std::vector<int32_t> numbers = tape_interface.read_to_vector(memory_capacity);

            tape_interface.swap_tape(merged_tape);
            tape_interface.move_right(readen_from_tape1 + readen_from_tape2);
            tape_interface.write_vector(numbers);

            readen_from_tape2 += numbers.size();
        }
    }
    if(readen_from_tape2 == tape_2.tape_size)
    {
        while(readen_from_tape1 != tape_1.tape_size)
        {
            tape_interface.swap_tape(tape_1);
            tape_interface.move_right(readen_from_tape1);
            std::vector<int32_t> numbers = tape_interface.read_to_vector(memory_capacity);

            tape_interface.swap_tape(merged_tape);
            tape_interface.move_right(readen_from_tape1 + readen_from_tape2);
            tape_interface.write_vector(numbers);

            readen_from_tape1 += numbers.size();
        }
    }

    return merged_tape;
}

std::vector<Tape> TapesManager::split_tape(Tape& data_tape)
{
    size_t amount_of_files = data_tape.tape_size / memory_capacity;
    size_t rest = data_tape.tape_size % memory_capacity;

    std::vector<Tape> vector_of_tapes;

    size_t main_pointer = 0;

    for(size_t files_cnt = 0; files_cnt < amount_of_files; ++files_cnt)
    {
        tape_interface.swap_tape(data_tape);
        tape_interface.move_right(main_pointer);

        std::vector<int32_t> numbers = tape_interface.read_to_vector(memory_capacity);
        main_pointer += memory_capacity;
        std::sort(numbers.begin(), numbers.end());

        Tape tmp_tape{("/tmp/" + std::to_string(files_cnt) + ".txt"), memory_capacity};
        tape_interface.swap_tape(tmp_tape);
        tape_interface.write_vector(numbers);

        vector_of_tapes.push_back(tmp_tape);
    }

    if(rest != 0)
    {
        ++amount_of_files;

        tape_interface.swap_tape(data_tape);
        tape_interface.move_right(main_pointer);
        std::vector<int32_t> numbers = tape_interface.read_to_vector(rest);
        main_pointer += rest;
        std::sort(numbers.begin(), numbers.end());

        Tape tmp_tape{"/tmp/" + std::to_string(amount_of_files - 1) + ".txt", rest};
        tape_interface.swap_tape(tmp_tape);
        tape_interface.write_vector(numbers);

        vector_of_tapes.push_back(tmp_tape);
    }

    return vector_of_tapes;
}

Tape TapesManager::sort_tape_(std::string sorted_file_name, std::vector<Tape> vector_of_tapes)
{
    if(vector_of_tapes.size() == 2)
    {
        Tape sorted_tape = merge_sorted_tapes(sorted_file_name, vector_of_tapes[0], vector_of_tapes[1]);

        delete_file(vector_of_tapes[0].tape_name);
        delete_file(vector_of_tapes[1].tape_name);

        return sorted_tape;
    }
    
    std::vector<Tape> new_vector_of_tapes;

    for(size_t cnt = 0; cnt < vector_of_tapes.size(); cnt += 2)
    {
        if(cnt == vector_of_tapes.size() - 1)
        {
            new_vector_of_tapes.push_back(vector_of_tapes[cnt]);
            break;
        }

        std::string new_tape_name = make_merged_tape_name(vector_of_tapes[cnt], vector_of_tapes[cnt+1]);
        std::string tmp_name = "/tmp/" + new_tape_name;
        Tape merged_tape = merge_sorted_tapes(tmp_name, vector_of_tapes[cnt], vector_of_tapes[cnt+1]);

        delete_file(vector_of_tapes[cnt].tape_name);
        delete_file(vector_of_tapes[cnt+1].tape_name);

        new_vector_of_tapes.push_back(merged_tape);   
    }

    return sort_tape_(sorted_file_name, new_vector_of_tapes);
}

Tape TapesManager::sort_tape(std::string sorted_file_name, Tape tape_to_sort)
{
    std::vector<Tape> vector_of_tapes = split_tape(tape_to_sort);

    Tape sorted_tape = sort_tape_(sorted_file_name, vector_of_tapes);

    return sorted_tape;
}
