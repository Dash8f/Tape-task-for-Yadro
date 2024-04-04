#include <iostream>
#include <fstream>
#include <string>

void delete_file(std::string file_name)
{
    std::fstream file;
    file.open(file_name);
    if(file.is_open())
    {
        file.close();
        std::filesystem::permissions(file_name, std::filesystem::perms::all);
        std::filesystem::remove(file_name);
    }
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

int main()
{
    delete_file("data/data.txt");

    unsigned rand_value = 1;
    srand(rand_value);

    std::fstream data1, data2, data;

    data.open("data/data.txt", std::ios::out | std::ios_base::trunc);

    data  << '<';

    int size_of_data_files = 0;

    std::cout << "Enter size of data file" << std::endl;
    std::cin >> size_of_data_files;

    for(int i = 0; i < size_of_data_files;  ++i)
    {
        std::string random_number = decimal_to_binary_string(rand() - (RAND_MAX / 2));

        data << random_number << ' ';
    }
    data.seekp(-1, std::ios_base::cur);
    data  << '>';
}