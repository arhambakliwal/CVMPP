#include "../include/Instruction.h"
std::ostream& operator<<(std::ostream& os, const Instruction& inst)
{
      switch(inst.opcode)
      {
            case Opcode::PUSH_INT:
                  os<<"PUSH_INT "<<inst.operand;
                  break;
            case Opcode::PUSH_BOOL:
                  os<<"PUSH_BOOL "<<inst.operand;
                  break;
            case Opcode::POP:
                  os<<"POP ";
                  break;
            case Opcode::LOAD:
                  os<<"LOAD "<<inst.operand;
                  break;
            case Opcode::STORE:
                  os<<"STORE "<<inst.operand;
                  break;
            case Opcode::OR:
                  os<<"OR ";
                  break;
            case Opcode::AND:
                  os<<"AND ";
                  break;
            case Opcode::NOT:
                  os<<"NOT ";
                  break;
            case Opcode::ADD:
                  os<<"ADD ";
                  break;
            case Opcode::SUB:
                  os<<"SUB ";
                  break;
            case Opcode::MUL:
                  os<<"MUL ";
                  break;
            case Opcode::DIV:
                  os<<"DIV ";
                  break;
            case Opcode::MOD:
                  os<<"MOD ";
                  break;
            case Opcode::LT:
                  os<<"LT ";
                  break;
            case Opcode::GT:
                  os<<"GT ";
                  break;
            case Opcode::EQ:
                  os<<"EQ ";
                  break;
            case Opcode::JMP:
                  os<<"JMP "<<inst.operand;
                  break;
            case Opcode::JMP_IF:
                  os<<"JMP_IF "<<inst.operand;
                  break;
            case Opcode::JMP_IF_FALSE:
                  os<<"JMP_IF_FALSE "<<inst.operand;
                  break;
            case Opcode::PRINT:
                  os<<"PRINT ";
                  break;
            case Opcode::INPUT:
                  os<<"INPUT ";
                  break;
            case Opcode::HALT:
                  os<<"HALT ";
                  break;     
      }
      return os;
}