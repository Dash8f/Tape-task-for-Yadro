#include "tape_interface.hpp"

int main()
{
    TapeInterface tape_interface("config.txt");

    tape_interface.swap_tape("data.txt");

    std::cout << tape_interface.read() << std::endl;
    tape_interface.write(-999);
    tape_interface.move_next_right();
    tape_interface.write(999);
    std::cout << tape_interface.read() << std::endl;
    tape_interface.move_next_left();
    std::cout << tape_interface.read() << std::endl;

    return 0;
}