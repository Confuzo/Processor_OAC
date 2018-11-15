#include "Instructions.hpp"

int main(int argc, char const *argv[])
{   
    std::string teste = "add       $s3,      $s0,      $s1";
    Instructions instruction(teste);
    return 0;
}
