#include "tape_interface.hpp"
#include "tapes_manager.hpp"

#define N 10

int main()
{
    Tape first_tape{"data.txt", N};
    Tape tape_1{"data1.txt", 10};
    Tape tape_2{"data2.txt", 10};

    TapeInterface tape_interface("config.txt", first_tape);

    TapesManager tapes_manager{tape_interface};

    tape_interface.swap_tape(tape_1);
    std::vector<int> nums1 = tape_interface.read_to_vector(10);

    std::cout << "first tape: ";
    for(int i = 0; i < tape_1.tape_size; ++i)
    {
        std::cout << nums1[i] << ' ';
    }
    std::cout << std::endl;

    tapes_manager.sort_short_tape(tape_1);
    tape_interface.swap_tape(tape_1);
    nums1 = tape_interface.read_to_vector(10);

    std::cout << "Sorted first tape: ";
    for(int i = 0; i < tape_1.tape_size; ++i)
    {
        std::cout << nums1[i] << ' ';
    }
    std::cout << std::endl;





    tape_interface.swap_tape(tape_2);
    nums1 = tape_interface.read_to_vector(10);

    std::cout << "second tape: ";
    for(int i = 0; i < tape_1.tape_size; ++i)
    {
        std::cout << nums1[i] << ' ';
    }
    std::cout << std::endl;

    tapes_manager.sort_short_tape(tape_2);
    tape_interface.swap_tape(tape_2);
    nums1 = tape_interface.read_to_vector(10);

    std::cout << "Sorted second tape: ";
    for(int i = 0; i < tape_2.tape_size; ++i)
    {
        std::cout << nums1[i] << ' ';
    }
    std::cout << std::endl;


    Tape tape_3 = tapes_manager.merge_sorted_tapes("sorted.txt", tape_1, tape_2);

    tape_interface.swap_tape(tape_3);
    nums1 = tape_interface.read_to_vector(tape_3.tape_size);
    std::cout << "Merged tape: ";
    for(int i = 0; i < tape_3.tape_size; ++i)
    {
        std::cout << nums1[i] << ' ';
    }
    std::cout << std::endl;

    return 0;
} 