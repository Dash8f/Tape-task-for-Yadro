#include "tape_interface.hpp"
#include "tapes_manager.hpp"

int main()
{
    TapeInterface tape_interface("config.txt");

    TapesManager tapes_manager{tape_interface};

    //tapes_manager.merge_sort_tape("data.txt");

    Tape tape_1;
    tape_1.tape_name = "data1.txt";
    tape_1.tape_size = 10;

    tape_interface.swap_tape(&tape_1);
    std::cout << "first tape: ";
    for(int i = 0; i < tape_1.tape_size; ++i)
    {
        std::cout << tape_interface.read() << ' ';
        tape_interface.move_next_right();
    }
    std::cout << std::endl;

    tapes_manager.sort_short_tape(&tape_1);

    tape_interface.swap_tape(&tape_1);
    std::cout << "sorted fisrt tape: ";
    for(int i = 0; i < tape_1.tape_size; ++i)
    {
        std::cout << tape_interface.read() << ' ';
        tape_interface.move_next_right();
    }
    std::cout << std::endl;

    Tape tape_2;
    tape_2.tape_name = "data2.txt";
    tape_2.tape_size = 10;

    tape_interface.swap_tape(&tape_2);
    std::cout << "second tape: ";
    for(int i = 0; i < tape_2.tape_size; ++i)
    {
        std::cout << tape_interface.read() << ' ';
        tape_interface.move_next_right();
    }
    std::cout << std::endl;

    tapes_manager.sort_short_tape(&tape_2);

    tape_interface.swap_tape(&tape_2);
    std::cout << "sorted second tape: ";
    for(int i = 0; i < tape_2.tape_size; ++i)
    {
        std::cout << tape_interface.read() << ' ';
        tape_interface.move_next_right();
    }
    std::cout << std::endl;




    Tape tape_3 = tapes_manager.merge_sorted_tapes("merged.txt", &tape_1, &tape_2);

    tape_interface.swap_tape(&tape_3);
    std::cout << "merged tape: ";
    for(int i = 0; i < tape_3.tape_size; ++i)
    {
        std::cout << tape_interface.read() << ' ';
        tape_interface.move_next_right();
    }
    std::cout << std::endl;


    Tape test = tapes_manager.merge_sort_tape(&tape_1);



    return 0;
} 