#pragma once
#include "ErrorHandler.h"
#include "Instruction.h"
#include <vector>
#include <stack>
class VirtualMachine
{
      private:
            int ip;
            const std::vector<Instruction>& code;
            std::stack<int32_t> stack;
            std::vector<int32_t> memory;
            void runInstruction(const Instruction&);
      public:
            VirtualMachine(const std::vector<Instruction>&, int);
            void run();
};