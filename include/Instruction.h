#pragma once
#include <cstdint>
#include <iostream>
enum class Opcode:int8_t
{
      PUSH_INT, PUSH_BOOL, POP,

      LOAD, STORE,
      
      //Logical Operators
      OR, AND, NOT,
      //Arithmetic Operators
      ADD, SUB, MUL, DIV, MOD,
      //Comparision Operators
      LT, GT, EQ,
      //Jump
      JMP, JMP_IF, JMP_IF_FALSE,
      //I/O Instructions
      PRINT, INPUT,
      HALT
};
// std::ostream& operator<<(std::ostream& os, const Opcode& opcode);

struct Instruction
{
      Opcode opcode;
      int32_t operand;
      Instruction(Opcode op, int32_t operand=0):opcode(op), operand(operand){}
      friend std::ostream& operator<<(std::ostream&, const Instruction&);
};