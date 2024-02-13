#include "CPU.h"
#include "Log.h"

CPU::CPU(const sc_core::sc_module_name &name, sc_core::sc_time clock_period) : sc_module(name),
                                                                               m_clock_period(clock_period),
                                                                               m_program_counter(0),
                                                                               m_instruction_counter(0),
                                                                               m_instruction_counter_pre(0),
                                                                               m_registers{0},
                                                                               m_io_registers{0}
{
  SC_HAS_PROCESS(CPU); // Register with the SystemC kernel
  // Register the CPU thread with SystemC
  SC_THREAD(CPU_thread);

  m_instruction = new InstructionAVR;

  m_map_instructionHandle = {
      {InstructionAVR_en::ADD, &CPU::operator_ADD},
      {InstructionAVR_en::ADC, &CPU::operator_ADC},
      {InstructionAVR_en::ADIW, &CPU::operator_xxx},
      {InstructionAVR_en::SUB, &CPU::operator_xxx},
      {InstructionAVR_en::SUBI, &CPU::operator_xxx},
      {InstructionAVR_en::SBC, &CPU::operator_xxx},
      {InstructionAVR_en::SBCI, &CPU::operator_xxx},
      {InstructionAVR_en::MULS, &CPU::operator_xxx},
      {InstructionAVR_en::MULSU, &CPU::operator_xxx},
      {InstructionAVR_en::AND, &CPU::operator_xxx},
      {InstructionAVR_en::ANDI, &CPU::operator_xxx},
      {InstructionAVR_en::OR, &CPU::operator_xxx},
      {InstructionAVR_en::ORI, &CPU::operator_ORI},
      {InstructionAVR_en::EOR, &CPU::operator_EOR},
      {InstructionAVR_en::COM, &CPU::operator_xxx},
      {InstructionAVR_en::NEG, &CPU::operator_xxx},
      {InstructionAVR_en::SBR, &CPU::operator_xxx},
      {InstructionAVR_en::CBR, &CPU::operator_xxx},
      {InstructionAVR_en::INC, &CPU::operator_xxx},
      {InstructionAVR_en::DEC, &CPU::operator_xxx},
      {InstructionAVR_en::TST, &CPU::operator_xxx},
      {InstructionAVR_en::CLR, &CPU::operator_xxx},
      {InstructionAVR_en::SER, &CPU::operator_SER},
      {InstructionAVR_en::LDI, &CPU::operator_LDI},
      {InstructionAVR_en::LD_X1, &CPU::operator_xxx},
      {InstructionAVR_en::LD_X2, &CPU::operator_xxx},
      {InstructionAVR_en::LD_X3, &CPU::operator_xxx},
      {InstructionAVR_en::LD_Y1, &CPU::operator_xxx},
      {InstructionAVR_en::LD_Y2, &CPU::operator_xxx},
      {InstructionAVR_en::LD_Y3, &CPU::operator_xxx},
      {InstructionAVR_en::LD_Y4, &CPU::operator_xxx},
      {InstructionAVR_en::LD_Z1, &CPU::operator_xxx},
      {InstructionAVR_en::LD_Z2, &CPU::operator_xxx},
      {InstructionAVR_en::LD_Z3, &CPU::operator_xxx},
      {InstructionAVR_en::LD_Z4, &CPU::operator_xxx},
      {InstructionAVR_en::ST_X1, &CPU::operator_xxx},
      {InstructionAVR_en::ST_X2, &CPU::operator_xxx},
      {InstructionAVR_en::ST_X3, &CPU::operator_xxx},
      {InstructionAVR_en::ST_Y1, &CPU::operator_xxx},
      {InstructionAVR_en::ST_Y2, &CPU::operator_xxx},
      {InstructionAVR_en::ST_Y3, &CPU::operator_xxx},
      {InstructionAVR_en::ST_Y4, &CPU::operator_xxx},
      {InstructionAVR_en::ST_Z1, &CPU::operator_xxx},
      {InstructionAVR_en::ST_Z2, &CPU::operator_xxx},
      {InstructionAVR_en::ST_Z3, &CPU::operator_xxx},
      {InstructionAVR_en::ST_Z4, &CPU::operator_xxx},
      {InstructionAVR_en::LPM_1, &CPU::operator_xxx},
      {InstructionAVR_en::LPM_2, &CPU::operator_xxx},
      {InstructionAVR_en::LPM_3, &CPU::operator_xxx},
      {InstructionAVR_en::SBIW, &CPU::operator_xxx},
      {InstructionAVR_en::STS16bit, &CPU::operator_xxx},
      {InstructionAVR_en::MOV, &CPU::operator_MOV},
      {InstructionAVR_en::MOVW, &CPU::operator_xxx},
      {InstructionAVR_en::RJMP, &CPU::operator_RJMP},
      {InstructionAVR_en::RCALL, &CPU::operator_RCALL},
      {InstructionAVR_en::RET, &CPU::operator_RET},
      {InstructionAVR_en::RETI, &CPU::operator_xxx},
      {InstructionAVR_en::CPSE, &CPU::operator_xxx},
      {InstructionAVR_en::CP, &CPU::operator_xxx},
      {InstructionAVR_en::CPC, &CPU::operator_xxx},
      {InstructionAVR_en::CPI, &CPU::operator_xxx},
      {InstructionAVR_en::SBRC, &CPU::operator_xxx},
      {InstructionAVR_en::SBRS, &CPU::operator_xxx},
      {InstructionAVR_en::SBIC, &CPU::operator_xxx},
      {InstructionAVR_en::SBIS, &CPU::operator_SBIS},
      {InstructionAVR_en::BREQ, &CPU::operator_xxx},
      {InstructionAVR_en::BRNE, &CPU::operator_xxx},
      {InstructionAVR_en::BRCS, &CPU::operator_xxx},
      {InstructionAVR_en::BRCC, &CPU::operator_xxx},
      {InstructionAVR_en::BRSH, &CPU::operator_xxx},
      {InstructionAVR_en::BRLO, &CPU::operator_xxx},
      {InstructionAVR_en::BRMI, &CPU::operator_xxx},
      {InstructionAVR_en::BRPL, &CPU::operator_xxx},
      {InstructionAVR_en::BRGE, &CPU::operator_xxx},
      {InstructionAVR_en::BRLT, &CPU::operator_xxx},
      {InstructionAVR_en::BRHS, &CPU::operator_xxx},
      {InstructionAVR_en::BRHC, &CPU::operator_xxx},
      {InstructionAVR_en::BRTS, &CPU::operator_xxx},
      {InstructionAVR_en::BRTC, &CPU::operator_xxx},
      {InstructionAVR_en::BRVS, &CPU::operator_xxx},
      {InstructionAVR_en::BRVC, &CPU::operator_xxx},
      {InstructionAVR_en::BRIE, &CPU::operator_xxx},
      {InstructionAVR_en::BRID, &CPU::operator_xxx},
      {InstructionAVR_en::BRBS, &CPU::operator_xxx},
      {InstructionAVR_en::BRBC, &CPU::operator_xxx},
      {InstructionAVR_en::SBI, &CPU::operator_SBI},
      {InstructionAVR_en::CBI, &CPU::operator_xxx},
      {InstructionAVR_en::LSL, &CPU::operator_xxx},
      {InstructionAVR_en::LSR, &CPU::operator_xxx},
      {InstructionAVR_en::ROL, &CPU::operator_xxx},
      {InstructionAVR_en::ROR, &CPU::operator_xxx},
      {InstructionAVR_en::ASR, &CPU::operator_xxx},
      {InstructionAVR_en::SWAP, &CPU::operator_xxx},
      {InstructionAVR_en::SEI, &CPU::operator_xxx},
      {InstructionAVR_en::BSET, &CPU::operator_xxx},
      {InstructionAVR_en::CLI, &CPU::operator_xxx},
      {InstructionAVR_en::BCLR, &CPU::operator_xxx},
      {InstructionAVR_en::NOP, &CPU::operator_NOP},
      {InstructionAVR_en::SLEEP, &CPU::operator_xxx},
      {InstructionAVR_en::WDR, &CPU::operator_xxx},
      {InstructionAVR_en::BREAK, &CPU::operator_xxx},
      {InstructionAVR_en::POP, &CPU::operator_xxx},
      {InstructionAVR_en::PUSH, &CPU::operator_xxx},
      {InstructionAVR_en::IN, &CPU::operator_IN},
      {InstructionAVR_en::OUT, &CPU::operator_OUT},
      {InstructionAVR_en::CALL, &CPU::operator_CALL},
      {InstructionAVR_en::JMP, &CPU::operator_xxx},
      {InstructionAVR_en::LDS, &CPU::operator_xxx},
      {InstructionAVR_en::STS, &CPU::operator_xxx},
  };
}

CPU::~CPU()
{
  delete m_instruction;
}

[[noreturn]] void CPU::CPU_thread()
{
  Instruction_st decoded_instruction;
  while (true)
  {
    uint16_t address = m_program_counter*2;
    // Fetch instruction from the current program counter address
    uint32_t instruction = fetch_instruction(address);

    // std::cout << "[CPU] Fetched instruction: " << std::hex << instruction << std::dec << std::endl;

    if (decode_instruction(decoded_instruction, (uint16_t)(0x0000FFFF & instruction), (uint16_t)(0x0000FFFF & (instruction >> 16))))
    {
      // Execute the fetched instruction
      execute_instruction(decoded_instruction);
    }

    // Handle interrupts (if any)
    handle_interrupts();

    // Wait for the clock period to simulate instruction execution time
    wait(m_clock_period);
  }
}

// fetch 2 instructions 1 time
uint32_t CPU::fetch_instruction(uint16_t addr)
{
  // Create a TLM generic payload
  tlm::tlm_generic_payload trans;
  sc_time delay = sc_time(0, SC_NS); // You can adjust the delay as needed

  // Set the transaction parameters for read
  uint32_t instruction = 0; // This will store the fetched instruction
  trans.set_command(tlm::TLM_READ_COMMAND);
  trans.set_address(addr);
  trans.set_data_ptr(reinterpret_cast<unsigned char *>(&instruction));
  trans.set_data_length(4);     // ATMega328P has 2-byte instructions, request 2 of 2-byte instructions, due to some instruction composed with 4 bytes
  trans.set_streaming_width(4); // No streaming for fixed-size instructions
  trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

  // Execute the read transaction
  cpu_program_out_socket->b_transport(trans, delay);

  // Check for successful transaction
  if (trans.is_response_error())
  {
    SC_REPORT_ERROR("TLM-2", "Read transaction returned with error!");
    return 0x0000; // Return a null instruction if there was an error
  }

  // Return the fetched instruction
  return instruction;
}

bool CPU::decode_instruction(Instruction_st &decoded_instruction, uint16_t instructionL, uint16_t instructionH)
{
  bool ret = false;
  uint8_t instruction_size = 0;
  ret = m_instruction->decode(decoded_instruction, instructionL, instructionH, instruction_size);
  m_instruction_counter_pre = m_instruction_counter;
  m_instruction_counter += instruction_size;
  return ret;
}

void CPU::execute_instruction(const Instruction_st &instruction)
{
  (this->*m_map_instructionHandle[instruction.name])(instruction);
}
void CPU::handle_interrupts()
{
  // Implementation for handling interrupts
  // ...

  // Placeholder interrupt handling logic
}

void CPU::operator_xxx(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", instruction.description);

  m_program_counter += instruction.instructionSize/2;
}

// (i) Rd ← Rd + Rr
void CPU::operator_ADD(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  uint8_t Rd = (instruction.instructionRaw & 0x01F0) >> 4;                                           // Extract bits 4-8 for Rd (dddd pattern)
  uint8_t Rr = (instruction.instructionRaw & 0x000F) | ((instruction.instructionRaw & 0x0200) >> 5); // Extract bits 0-3 and 9 for Rr (rrrr pattern and the r from rd)

  // Perform the logical ADD operation
  m_registers[Rd] = m_registers[Rd] + m_registers[Rr];

  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => ", instruction.description, " R", std::dec, static_cast<int>(Rd), " ← R", static_cast<int>(Rd), " + R", static_cast<int>(Rr));

  m_program_counter += 1;
}

// (i) Rd ← Rd + Rr + C
void CPU::operator_ADC(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  uint8_t Rd = (instruction.instructionRaw & 0x01F0) >> 4;                                           // Extract bits 4-8 for Rd (dddd pattern)
  uint8_t Rr = (instruction.instructionRaw & 0x000F) | ((instruction.instructionRaw & 0x0200) >> 5); // Extract bits 0-3 and 9 for Rr (rrrr pattern and the r from rd)

  // Perform the logical ADC operation
  m_registers[Rd] = m_registers[Rd] + m_registers[Rr] + m_register_SREG.C;

  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => ", instruction.description, " R", std::dec, static_cast<int>(Rd), " ← R", static_cast<int>(Rd), " + R", static_cast<int>(Rr), " + ", m_register_SREG.C);

  m_program_counter += 1;
}

// 1001 0110 KKdd KKKK
// Rd+1:Rd ← Rd+1:Rd + K
// d ∈ {24,26,28,30}, 0 ≤ K ≤ 63
void CPU::operator_ADIW(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract register pair number (d) and immediate value (K) from the instruction
  uint8_t d = (instruction.instructionRaw & 0x0030) >> 4; // d ∈ {0, 1, 2, 3} representing register pairs {24, 26, 28, 30}
  uint8_t K = (instruction.instructionRaw & 0x000F) | ((instruction.instructionRaw & 0x00C0) >> 2);

  // Calculate the effective address of the register pair
  uint16_t effective_address = m_registers[d + 24] | (m_registers[d + 25] << 8);

  // Perform the addition
  uint16_t result = effective_address + K;

  // Update the register pair with the result
  m_registers[d + 24] = static_cast<uint8_t>(result & 0xFF);
  m_registers[d + 25] = static_cast<uint8_t>((result >> 8) & 0xFF);

  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => ADIW Rd+1:Rd, K=", static_cast<int>(d + 24), ", ", static_cast<int>(K));

  m_program_counter += 1;
}

// 0010 00rd dddd rrrr
// Rd ← Rd • Rr
void CPU::operator_AND(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract register numbers from the instruction
  uint8_t Rd = (instruction.instructionRaw & 0x1F);
  uint8_t Rr = ((instruction.instructionRaw >> 5) & 0x1F);

  // Perform the logical AND operation
  m_registers[Rd] &= m_registers[Rr];

  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => ", "Rd=", static_cast<int>(Rd), ", Rr=", static_cast<int>(Rr));

  m_program_counter += 1;
}

void CPU::operator_ANDI(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract register number and immediate value from the instruction
  uint8_t Rd = (uint8_t)(instruction.instructionRaw & 0x0F) + 16;
  uint8_t K = ((instruction.instructionRaw >> 4) & 0xF0) | (instruction.instructionRaw & 0x0F);

  // Perform the logical AND operation with immediate value
  m_registers[Rd] &= K;

  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => ", "Rd=", static_cast<int>(Rd), ", K=", static_cast<int>(K));

  m_program_counter += 1;
}

void CPU::operator_BRCC(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract the relative offset (k) from the instruction
  int8_t k = static_cast<int8_t>((instruction.instructionRaw & 0x01F0) >> 4);
  if (k & 0x0010)
  {
    // Sign extend for negative values
    k |= 0xFFE0;
  }

  // Check the Carry Flag (C)
  if (!m_register_SREG.C)
  {
    // Branch if Carry is cleared
    m_program_counter += (k + 1);
  }

  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => BRCC k=", static_cast<int>(k));

  // Increment the program counter regardless of branching
  m_program_counter += 1;
}

void CPU::operator_BRCS(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract the branch offset (k) from the instruction
  int8_t k = static_cast<int8_t>((instruction.instructionRaw & 0x007E) >> 1); // Extract k and sign-extend

  // Check if the Carry Flag (C) is set in the status register (SREG)
  if (m_register_SREG.C)
  {
    // If C = 1, branch relatively to PC
    m_program_counter += k + 1;
  }
  else
  {
    // If C = 0, PC is incremented by 1
    m_program_counter += 1;
  }

  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => BRCS k=", static_cast<int>(k));

  m_program_counter += 1; // Increment PC for the next instruction
}

void CPU::operator_BREQ(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract the offset from the instruction (in two's complement form)
  int8_t offset = static_cast<int8_t>((instruction.instructionRaw >> 3) & 0x7F); // Assuming the immediate field contains the offset

  // Check if the Zero Flag (Z) is set
  if (m_register_SREG.Z)
  {
    // Branch relatively if Z flag is set
    m_program_counter += offset + 1;
  }
  else
  {
    // Otherwise, move to the next instruction
    m_program_counter += 1;
  }

  // Print debug information
  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => BREQ k=", static_cast<int>(offset));

  // Update program counter
  m_program_counter += 1;
}

void CPU::operator_BRNE(const Instruction_st &instruction)
{
}

void CPU::operator_CALL(const Instruction_st &instruction)
{
}

void CPU::operator_CLI(const Instruction_st &instruction)
{
}

void CPU::operator_COM(const Instruction_st &instruction)
{
}

void CPU::operator_CP(const Instruction_st &instruction)
{
}

// Rd ← Rd ⊕ Rr
void CPU::operator_EOR(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract the destination register (Rd) and source register (Rr)
  uint8_t Rd = (instruction.instructionRaw >> 4) & 0x1F;
  uint8_t Rr = instruction.instructionRaw & 0x0F;

  // Perform the logical XOR operation between the contents of Rd and Rr
  m_registers[Rd] ^= m_registers[Rr];

  // Print debug information
  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => EOR Rd,Rr=", static_cast<int>(Rd), ",", static_cast<int>(Rr));

  // Update program counter
  m_program_counter += 1;
}

//Rd ← Rd + 1
void CPU::operator_IN(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract the destination register (Rd) and I/O address (A)
  uint8_t Rd = (instruction.instructionRaw >> 4) & 0x1F;
  uint8_t A = instruction.instructionRaw & 0x3F;

  // Load data from the I/O Space into register Rd
  // m_registers[Rd] = read_io_register(A); // Assuming read_io_register is a function to read from I/O space

  // Print debug information
  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => IN Rd,A=", static_cast<int>(Rd), ",", static_cast<int>(A));

  // Update program counter
  m_program_counter += 1;
}

//Rd ← K
void CPU::operator_LDI(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract the destination register (Rd) and immediate value (K)
  uint8_t Rd = (instruction.instructionRaw >> 4) & 0x0F;
  uint8_t K = ((instruction.instructionRaw >> 4) & 0xF0) | (instruction.instructionRaw & 0x0F);

  // Load the immediate value into the destination register
  m_registers[Rd + 16] = K;

  // Print debug information
  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => LDI Rd,K=", static_cast<int>(Rd + 16), ",", static_cast<int>(K));

  // Update program counter
  m_program_counter += 1;
}

//Rd ← Rr
void CPU::operator_MOV(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract the source register (Rr) and destination register (Rd)
  uint8_t Rd = (instruction.instructionRaw >> 4) & 0x1F;
  uint8_t Rr = ((instruction.instructionRaw >> 5) & 0x10) | (instruction.instructionRaw & 0x0F);

  // Copy the value from the source register to the destination register
  m_registers[Rd] = m_registers[Rr];

  // Print debug information
  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => MOV Rd,Rr=", static_cast<int>(Rd), ",", static_cast<int>(Rr));

  // Update program counter
  m_program_counter += 1;
}

void CPU::operator_NOP(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Print debug information
  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => NOP");

  // Update program counter
  m_program_counter += 1;
}

// Rd ← Rd v K
void CPU::operator_ORI(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract the destination register (Rd) and the immediate value (K) from the instruction
  uint8_t d = (instruction.instructionRaw >> 4) & 0x1F; // Rd (destination register)
  uint8_t K = ((instruction.instructionRaw >> 4) & 0xF0) | (instruction.instructionRaw & 0x0F);        // K (immediate value)

  // Perform the logical OR operation
  m_registers[d] |= K;

  // Print debug information
  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => ORI Rd,K=", static_cast<int>(d), ",", static_cast<int>(K));

  // Update program counter
  m_program_counter += 1;
}

//I/O(A) ← Rr
void CPU::operator_OUT(const Instruction_st &instruction)
{
  std::stringstream stream;

  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;

  std::string instruction_counter_str = stream.str(); // Convert to string

  // Extract the source register (Rr) and the I/O location (A) from the instruction
  uint8_t r = (instruction.instructionRaw >> 4) & 0x1F; // Rr (source register)
  uint8_t A = (((instruction.instructionRaw >> 5) & 0x30) | (instruction.instructionRaw & 0x0F));        // A (I/O location)

  // Store the contents of register Rr in the I/O space
  // Example code to store Rr to I/O(A)
  // io_space[A] = m_registers[r];

  // Print debug information
  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => OUT A,Rr= ", static_cast<int>(A), ",", static_cast<int>(r));

  // Update program counter
  m_program_counter += 1;
}

//PC ← PC + k + 1
void CPU::operator_RCALL(const Instruction_st &instruction)
{
  std::stringstream stream;
  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;
  std::string instruction_counter_str = stream.str(); // Convert to string
  // Retrieve the offset from the instruction (in two's complement form)
  int16_t offset = static_cast<int16_t>(instruction.instructionRaw & 0x0FFF);
  // Convert the offset to a signed value
  if (offset & 0x0800)
  {
    offset |= 0xF000; // Sign-extend the offset if negative
  }

  // Calculate the return address
  uint16_t returnAddress = m_program_counter + 1;

  // Push the return address onto the stack
  m_stack.set(returnAddress);

  // Update the program counter
  m_program_counter += offset + 1;

  // Print debug information
  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => RCALL k=", static_cast<int>(offset));
}

//PC(15:0) ← STACK
void CPU::operator_RET(const Instruction_st &instruction)
{
  std::stringstream stream;
  // Set the stream to display the value in hexadecimal format
  stream << std::hex << m_program_counter;
  std::string instruction_counter_str = stream.str(); // Convert to string

  // Retrieve the return address from the stack
  uint16_t returnAddress = m_stack.get();

  // Update the program counter
  m_program_counter = returnAddress;

  // Print debug information
  AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => RET None");
}

//PC ← PC + k + 1
void CPU::operator_RJMP(const Instruction_st &instruction) {
    std::stringstream stream;
    // Set the stream to display the value in hexadecimal format
    stream << std::hex << m_program_counter;
    std::string instruction_counter_str = stream.str(); // Convert to string

    // Retrieve the offset from the instruction (in two's complement form)
    int16_t offset = static_cast<int16_t>(instruction.instructionRaw & 0x0FFF);
    // Convert the offset to a signed value
    if (offset & 0x0800) {
        offset |= 0xF000; // Sign-extend the offset if negative
    }

    // Print debug information
    AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => RJMP k=", static_cast<int>(offset));

    // Update the program counter with the relative offset
    m_program_counter += offset + 1;
}

//I/O(A,b) ← 1
void CPU::operator_SBI(const Instruction_st &instruction) {
    std::stringstream stream;
    // Set the stream to display the value in hexadecimal format
    stream << std::hex << m_program_counter;
    std::string instruction_counter_str = stream.str(); // Convert to string

    // Extract the I/O register address (A) and bit position (b) from the instruction
    uint8_t A = (instruction.instructionRaw >> 3) & 0x1F; // Extract bits 4-8
    uint8_t b = instruction.instructionRaw & 0x07;        // Extract bits 0-2

    // Set the specified bit in the I/O register
    m_io_registers[A] |= (1 << b);

    // Print debug information
    AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => SBI ", static_cast<int>(A), ",", static_cast<int>(b));

    // Update the program counter
    m_program_counter += 1;
}

//If I/O(A,b) = 1 then PC ← PC + 2 (or 3) else PC ← PC + 1
void CPU::operator_SBIS(const Instruction_st &instruction) {
    std::stringstream stream;
    // Set the stream to display the value in hexadecimal format
    stream << std::hex << m_program_counter;
    std::string instruction_counter_str = stream.str(); // Convert to string

    // Extract the I/O register address (A) and bit position (b) from the instruction
    uint8_t A = (instruction.instructionRaw >> 3) & 0x1F; // Extract bits 4-8
    uint8_t b = instruction.instructionRaw & 0x07;        // Extract bits 0-2

    // Test the specified bit in the I/O register
    if ((m_io_registers[A] >> b) & 0x01) {
        // If the bit is set, skip the next instruction
        m_program_counter += 2;
    } else {
        // If the bit is not set, move to the next instruction
        m_program_counter += 1;
    }

    // Print debug information
    AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => SBIS ", static_cast<int>(A), ",", static_cast<int>(b));

    // Update the program counter
    m_program_counter += 1;
}

//Rd ← $FF
//16 ≤ d ≤ 31
void CPU::operator_SER(const Instruction_st &instruction) {
    std::stringstream stream;
    // Set the stream to display the value in hexadecimal format
    stream << std::hex << m_program_counter;
    std::string instruction_counter_str = stream.str(); // Convert to string

    // Extract the destination register (Rd) from the instruction
    uint8_t d = (uint8_t)((instruction.instructionRaw >> 4) & 0x0F) + 16; // Extract bits 4-7

    // Load the value $FF directly into the destination register
    m_registers[d] = 0xFF;

    // Print debug information
    AVR_tlm_sim::log::printInfo(instruction_counter_str, ": ", std::setfill('0'), std::setw(4), std::hex, instruction.instructionRaw, " => SER R", static_cast<int>(d));

    // Update the program counter
    m_program_counter += 1;
}
