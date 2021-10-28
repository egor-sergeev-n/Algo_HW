//1.3.Проверка битов
//Если в числе содержится только один бит со значением 1, записать в выходной поток OK.
//Иначе записать FAIL.
//Необходимо использование битовых операций.
//Использование арифметических операций запрещено.
#include <iostream>
#include <assert.h>
#include <sstream>

void check_one_bit(std::istream& input, std::ostream& output)
{
    unsigned int max = 2147483648; // 2^31
    unsigned int num = 0;
    input >> num;
    while (max > 0)
    {
        if (max == num)
        {
            output << "OK";
            return;
        }
        max >>= 1;
    }
    output << "FAIL";
}

void test_check_one_bit()
{
    {
        std::stringstream in;
        in << 0 << std::endl;
        std::stringstream out;
        check_one_bit(in, out);

        assert(out.str() == "FAIL");
    }

    {
        std::stringstream in;
        in << 1 << std::endl;
        std::stringstream out;
        check_one_bit(in, out);

        assert(out.str() == "OK");
    }

    {
        unsigned int num = 1;
        for (int i = 0; i < 31; ++i)
        {
            std::stringstream in;
            in << num << std::endl;
            std::stringstream out;
            check_one_bit(in, out);

            assert(out.str() == "OK");
            num <<= 1;
        }
    }

    {
        unsigned int num = 2;
        for (int i = 0; i < 31; ++i)
        {
            std::stringstream in;
            in << num + 1 << std::endl;
            std::stringstream out;
            check_one_bit(in, out);

            assert(out.str() == "FAIL");
            num <<= 1;
        }
    }
}

int main()
{
    test_check_one_bit();
    std::cout << "TESTS PASSED" << std::endl;
    //check_one_bit(std::cin, std::cout);
    return 0;
}