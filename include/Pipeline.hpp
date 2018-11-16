#include <systemc.h>
#include <iostream>
#include <fstream>
#include "Instructions.hpp"

SC_MODULE(pipeline)
{

    sc_out<sc_uint<32>> reg_a, reg_b;
    sc_out<sc_uint<4>> op_code;
    sc_in<sc_uint<32>> address;
    sc_out<bool> enable;
    sc_in<bool> clk;

    std::vector<std::string> registers;
    std::vector<Instructions> instructions_v;

    void l_registers(){
        std::ifstream arquivo("files/regs.txt", std::fstream::in);
        std::string line;
        while (std::getline(arquivo, line))
        {
            if (!line.empty())
            {
                Tokenizer tokenizer(line);
                std::vector<std::string> token = tokenizer.getToken();
                for (auto i(0u); i < token.size(); i++)
                {
                    registers.push_back(token.at(i));
                }
            }
        }

        arquivo.close();
    }

    void l_instructions(){
        std::ifstream arquivo("files/data.txt", std::fstream::in);
        std::string line;

        while (std::getline(arquivo, line))
        {
            if (!line.empty())
            {
                Instructions instruction(line);
                instructions_v.push_back(instruction);
            }
        }

        arquivo.close();
    }

    void w_registers(){
        if (address.read() >= instructions_v.size()){
            enable.write(false);
            return;
        }else{
            enable.write(true);
        }

        Instructions  instruction = instructions_v.at(address.read());
        if (instruction.getOp1() != "" and instruction.getOp2() != "" and instruction.getName() != "lw")
        {
            std::string r1 = instruction.getOp1();
            std::string r2 = instruction.getOp2();
            int index1 = stoi(r1.substr(r1.length() - 1));
            int index2 = stoi(r2.substr(r2.length() - 1));
            int op1 = stoi(registers.at(index1));
            int op2 = stoi(registers.at(index2));
            reg_a.write(op1);
            reg_b.write(op2);
        }

        if (instruction.getName() == "add")
        {
            op_code.write(1);
        }
        else if (instruction.getName() == "sub")
        {
            op_code.write(2);
        }
        else if (instruction.getName() == "and")
        {
            op_code.write(3);
        }
        else if (instruction.getName() == "or")
        {
            op_code.write(4);
        }
        else if (instruction.getName() == "xor")
        {
            op_code.write(5);
        }
        else if (instruction.getName() == "cmp")
        {
            op_code.write(6);
        }
        else if (instruction.getName() == "not")
        {
            op_code.write(7);
        }
        else if (instruction.getName() == "j")
        {
            std::string r1 = instruction.getDest();
            int index1 = stoi(r1);
            reg_a.write(index1);
            op_code.write(8);
        }
        else if (instruction.getName() == "jn")
        {
            std::string r1 = instruction.getDest();
            int index1 = stoi(r1);
            reg_a.write(index1);
            op_code.write(9);
        }
        else if (instruction.getName() == "jz")
        {
            std::string r1 = instruction.getDest();
            int index1 = stoi(r1);
            reg_a.write(index1);
            op_code.write(10);
        }
        else if (instruction.getName() == "lw")
        {
            std::string r1 = instruction.getOp1();
            reg_a.write(stoi(r1));

            std::string r2 = instruction.getDest();
            int index2 = stoi(r2.substr(r2.length() - 1));
            reg_b.write(index2);
            op_code.write(11);
        }
        else if (instruction.getName() == "sw")
        {
            std::string r1 = instruction.getDest();
            reg_b.write(stoi(r1));
            op_code.write(12);
        }
    }

    SC_CTOR(pipeline)
    {
        SC_METHOD(l_registers);
        SC_METHOD(l_instructions);
        SC_METHOD(w_registers);

        sensitive << address << clk;
    }
};
