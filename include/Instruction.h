#pragma once

#include <string>
#include "Mytype.h"

class CPU;

// Define a struct to represent an AVR instruction
struct Instruction_st
{
  uint32_t instructionRaw; // raw instruction
  uint16_t pattern;        // Opcode pattern to match
  uint16_t mask;           // Opcode mask for the instruction
  uint16_t name;           // Name of the instruction
  uint8_t instructionSize; // Size of the instruction in byte
  uint8_t clock_count;     // number of clock for execute this instruction
  std::string description; // Description of the instruction
  // Copy assignment operator
  Instruction_st &
  operator=(const Instruction_st &other)
  {
    if (this != &other)
    {
      pattern = other.pattern;
      mask = other.mask;
      name = other.name;
      instructionSize = other.instructionSize;
      clock_count = other.clock_count;
      description = other.description;
    }
    return *this;
  }
};

// struct InstructionHandle_st
// {
//   Instruction_st instruction;
//   void (CPU::*parseFunction)(Instruction_st instruction, uint16_t instructionL, uint16_t instructionH); // Function pointer for instruction parsing
// };

// Interface class for Instruction
class Instruction
{
public:
  virtual bool decode(Instruction_st &decoded_instruction, uint16_t instructionL, uint16_t instructionH, uint8_t &instruction_size) = 0;
  virtual ~Instruction() = default; // Destructor to clean up allocated memory
};
