#pragma once

#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <systemc.h>
#include "Mytype.h"
#include "InstructionAvr.h"
#include "SRam.h"

#define STACK_SIZE 256
#define IO_REGISTER_BASE_ADDRESS 0x0020

struct SREG
{
  uint8_t I : 1; // Interrupt Enable
  uint8_t T : 1; // Bit Copy Storage
  uint8_t H : 1; // Half Carry
  uint8_t S : 1; // Sign Bit (I ⊕ V)
  uint8_t V : 1; // Two's Complement Overflow
  uint8_t N : 1; // Negative
  uint8_t Z : 1; // Zero
  uint8_t C : 1; // Carry
};

union StackPointer
{
  uint16_t SPL[STACK_SIZE];
  uint8_t SPS[STACK_SIZE*2];
};

class Stack
{
private:
  StackPointer m_sp;
  int sp;

public:
  Stack()
  {
    sp = STACK_SIZE - 1;
  }

  void push(uint8_t data)
  {
    if (sp < 0)
    {
      // Handle stack overflow
      return;
    }
    m_sp.SPS[sp--] = data;
  }

  uint8_t pop()
  {
    if (sp > STACK_SIZE - 1)
    {
      // Handle stack underflow
      return 0;
    }
    return m_sp.SPS[++sp];
  }

  uint16_t get()
  {
    sp += sizeof(uint16_t); // Assuming each stack entry is 2 bytes
    if (sp > STACK_SIZE - 1)
    {
      // Handle stack underflow
      return 0;
    }
    return m_sp.SPL[sp];
  }

  void set(uint16_t data)
  {
    if (sp < 0)
    {
      // Handle stack overflow
      return;
    }
    m_sp.SPL[sp] = data;
    sp -= sizeof(uint16_t); // Assuming each stack entry is 2 bytes
  }
};

class CPU : public sc_module
{
public:
  // TLM initiator socket for program memory accessing
  tlm_utils::simple_initiator_socket<CPU> cpu_program_out_socket;

  // TLM initiator socket for data memory accessing
  tlm_utils::simple_initiator_socket<CPU> cpu_data_out_socket;

  // Constructor with a parameter for the clock period
  CPU(const sc_core::sc_module_name &name, sc_core::sc_time clock_period);

  // Destructor
  virtual ~CPU();

  // SystemC thread for simulating CPU activity
  [[noreturn]] void CPU_thread();

  // read all general registers and io registers
  void readRegisters(uint8_t* generalRegister, uint8_t* ioRegister){
    memcpy(generalRegister, m_registers, sizeof(m_registers));
    memcpy(ioRegister, m_io_registers, sizeof(m_io_registers));
  }

private:
  // Clock period for the CPU
  sc_time m_clock_period;

  // Instrucion counter for debug disasembly code from binary
  uint16_t m_instruction_counter;

  // Instrucion counter for debug disasembly code from binary
  uint16_t m_instruction_counter_pre;

  // Instrucion counter for debug disasembly code from binary
  uint16_t m_program_counter;

  // Instruction decoder
  InstructionAVR *m_instruction;

  // AVR status register
  SREG m_register_SREG;

  // AVR stack pointer
  Stack m_stack;

  // General purpose registers
  uint8_t m_registers[32];

  // io registers
  uint8_t m_io_registers[64];

  // A method to fetch instructions from memory
  uint32_t
  fetch_instruction(uint16_t addr);

  // Map of the instruction processing function
  std::map<uint16_t, void (CPU::*)(const Instruction_st &)> m_map_instructionHandle;

  // Decode the instruction
  bool decode_instruction(Instruction_st &decoded_instruction, uint16_t instructionL, uint16_t instructionH);

  // A method to execute the fetched instruction
  void execute_instruction(const Instruction_st &instruction);

  // method to read the sram
  void sram_read(uint16_t address, uint8_t* des, uint16_t length);

  // method to write the sram
  void sram_write(uint16_t address, uint8_t* src, uint16_t length);

  // Method to handle interrupts (if required)
  void handle_interrupts();

  void operator_xxx(const Instruction_st &instruction);

  //'adc', 'add', 'adiw', 'and', 'andi', 'brcc', 'brcs', 'breq', 'brne', 'call', 'cli', 'com', 'cp'

  void operator_ADD(const Instruction_st &instruction);

  void operator_ADC(const Instruction_st &instruction);

  void operator_ADIW(const Instruction_st &instruction);

  void operator_AND(const Instruction_st &instruction);

  void operator_ANDI(const Instruction_st &instruction);

  void operator_BRCC(const Instruction_st &instruction);

  void operator_BRCS(const Instruction_st &instruction);

  void operator_BREQ(const Instruction_st &instruction);

  void operator_BRNE(const Instruction_st &instruction);

  void operator_CALL(const Instruction_st &instruction);

  void operator_CLI(const Instruction_st &instruction);

  void operator_COM(const Instruction_st &instruction);

  void operator_CP(const Instruction_st &instruction);

  //'cpc', 'cpi', 'cpse', 'dec'
  // ...

  //'EOR', 'IN', 'LDI', 'MOV', 'NOP', 'ORI', 'OUT', 'RCALL', 'RET', 'RJMP', 'SBI', 'SBIS', 'SER'
  void operator_EOR(const Instruction_st &instruction);

  void operator_IN(const Instruction_st &instruction);

  void operator_LDI(const Instruction_st &instruction);

  void operator_MOV(const Instruction_st &instruction);

  void operator_NOP(const Instruction_st &instruction);

  void operator_ORI(const Instruction_st &instruction);

  void operator_OUT(const Instruction_st &instruction);

  void operator_RCALL(const Instruction_st &instruction);

  void operator_RET(const Instruction_st &instruction);

  void operator_RJMP(const Instruction_st &instruction);

  void operator_SBI(const Instruction_st &instruction);

  void operator_SBIS(const Instruction_st &instruction);

  void operator_SER(const Instruction_st &instruction);

  // 'jmp', 'ld_x1', 'ld_z1', 'ldi', 'lds', 'lpm_2', 'lpm_3', 'movw', 'muls', 'mulsu', 'or', 'pop', 'push', 'reti', 'sbc', 'sbci', 'sbiw', 'sei', 'st_x1', 'st_x2', 'sts', 'sub', 'subi'
};
