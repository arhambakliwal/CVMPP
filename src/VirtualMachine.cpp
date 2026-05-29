#include "../include/VirtualMachine.h"
VirtualMachine::VirtualMachine(const std::vector<Instruction>& byteCode, int memorySize):ip(0), code(byteCode)
{
      memory.resize(memorySize);
}

void VirtualMachine::runInstruction(const Instruction& inst)
{
      int32_t a, b;
      switch(inst.opcode)
      {
            case Opcode::PUSH_INT:
                  stack.push(inst.operand);
                  ip++;
                  break;
            case Opcode::PUSH_BOOL:
                  stack.push(inst.operand);
                  ip++;
                  break;
            case Opcode::POP:
                  stack.pop();
                  ip++;
                  break;
            case Opcode::LOAD:
                  stack.push(memory.at(inst.operand));
                  ip++;
                  break;
            case Opcode::STORE:
                  memory[inst.operand]=stack.top();
                  stack.pop();
                  ip++;
                  break;
            case Opcode::OR:
                  b=stack.top();
                  stack.pop();
                  a=stack.top();
                  stack.pop();
                  stack.push(a||b);
                  ip++;
                  break;
            case Opcode::AND:
                  b=stack.top();
                  stack.pop();
                  a=stack.top();
                  stack.pop();
                  stack.push(a&&b);
                  ip++;
                  break;
            case Opcode::NOT:
                  a=stack.top();
                  stack.pop();
                  stack.push(!a);
                  ip++;
                  break;
            case Opcode::ADD:
                  b=stack.top();
                  stack.pop();
                  a=stack.top();
                  stack.pop();
                  stack.push(a+b);
                  ip++;
                  break;
            case Opcode::SUB:
                  b=stack.top();
                  stack.pop();
                  a=stack.top();
                  stack.pop();
                  stack.push(a-b);
                  ip++;
                  break;
            case Opcode::MUL:
                  b=stack.top();
                  stack.pop();
                  a=stack.top();
                  stack.pop();
                  stack.push(a*b);
                  ip++;
                  break;
            case Opcode::DIV:
                  b=stack.top();
                  stack.pop();
                  a=stack.top();
                  stack.pop();
                  if(b==0)
                  {
                        ErrorHandler::runtimeError("Division by zero");
                  }
                  stack.push(a/b);
                  ip++;
                  break;
            case Opcode::MOD:
                  b=stack.top();
                  stack.pop();
                  a=stack.top();
                  stack.pop();
                  if(b==0)
                  {
                        ErrorHandler::runtimeError("Division by zero");
                  }
                  stack.push(a%b);
                  ip++;
                  break;
            case Opcode::LT:
                  b=stack.top();
                  stack.pop();
                  a=stack.top();
                  stack.pop();
                  stack.push(a<b);
                  ip++;
                  break;
            case Opcode::GT:
                  b=stack.top();
                  stack.pop();
                  a=stack.top();
                  stack.pop();
                  stack.push(a>b);
                  ip++;
                  break;
            case Opcode::EQ:
                  b=stack.top();
                  stack.pop();
                  a=stack.top();
                  stack.pop();
                  stack.push(a==b);
                  ip++;
                  break;
            case Opcode::JMP:
                  ip=inst.operand;
                  break;
            case Opcode::JMP_IF:
                  if(stack.top())
                  {
                        ip=inst.operand;
                  }
                  else
                  {
                        ip++;
                  }
                  stack.pop();
                  break;
            case Opcode::JMP_IF_FALSE:
                  if(!stack.top())
                  {
                        ip=inst.operand;
                  }
                  else
                  {
                        ip++;
                  }
                  stack.pop();
                  break;
            case Opcode::PRINT:
                  std::cout<<stack.top()<<std::endl;
                  stack.pop();
                  ip++;
                  break;
            case Opcode::INPUT:
                  std::cin>>a;
                  stack.push(a);
                  ip++;
                  break;
            case Opcode::HALT:
                  ip=code.size();
                  break;
      }
}
void VirtualMachine::run()
{
      while(ip<(int)code.size())
      {
            runInstruction(code.at(ip));
      }
}