#include "tape_interface.hpp"

int main()
{
    std::fstream config_file;

    size_t write_delay = 0, read_delay  = 0, move_delay  = 0, swap_delay  = 0;

    config_file.open("config.txt", std::ios::in);

    if(config_file.is_open())
    {
        std::cout << "Config file is openen. Config settings:" << std::endl;

        config_file.seekg(47, std::ios_base::cur);
        config_file >> write_delay;
        std::cout << "Write delay is " << write_delay << " ms" << std::endl;

        config_file.seekg(18, std::ios_base::cur);
        config_file >> read_delay;
        std::cout << "Read delay is " << read_delay << " ms" <<std::endl;

        config_file.seekg(18, std::ios_base::cur);
        config_file >> move_delay;
        std::cout << "Move delay is " << move_delay << " ms" <<std::endl;

        config_file.seekg(18, std::ios_base::cur);
        config_file >> swap_delay;
        std::cout << "Swap delay is " << swap_delay << " ms" <<std::endl;
    }

    TapeInterface tape_interface{write_delay,read_delay,move_delay,swap_delay};

    tape_interface.swap_tape("data.txt");

    std::cout << tape_interface.read() << std::endl;

    tape_interface.move_next_right();

    tape_interface.write(99999);

    std::cout << tape_interface.read() << std::endl;

    tape_interface.move_next_left();

    std::cout << tape_interface.read() << std::endl;

    return 0;
}