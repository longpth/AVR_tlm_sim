#include "InstructionAvr.h"
#include "CPU.h"
#include "Log.h"

InstructionAVR::InstructionAVR() : m_instructionArray{}
{
  uint16_t instr_index = 0;
  // Opcode ,mask, name, instructionSize, clock_count, description
  //Arithmetic Instructions:
  m_instructionArray[instr_index++] = {0x0000, 0b0000110000000000, 0b1111110000000000, InstructionAVR_en::ADD, 2, 1, "ADD : Add without Carry"};
  m_instructionArray[instr_index++] = {0x0000, 0b0001110000000000, 0b1111110000000000, InstructionAVR_en::ADC, 2, 1, "ADC : Add with Carry"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001011000000000, 0b1111111100000000, InstructionAVR_en::ADIW, 2, 1, "ADIW : Add Immediate to Word"};
  m_instructionArray[instr_index++] = {0x0000, 0b0001100000000000, 0b1111110000000000, InstructionAVR_en::SUB, 2, 1, "SUB : Subtract without Carry"};
  m_instructionArray[instr_index++] = {0x0000, 0b0101000000000000, 0b1111000000000000, InstructionAVR_en::SUBI, 2, 1, "SUBI : Subtract Immediate"};
  m_instructionArray[instr_index++] = {0x0000, 0b0000100000000000, 0b1111110000000000, InstructionAVR_en::SBC, 2, 1, "SBC : Subtract with Carry"};
  m_instructionArray[instr_index++] = {0x0000, 0b0100000000000000, 0b1111000000000000, InstructionAVR_en::SBCI, 2, 1, "SBCI : Subtract Immediate with Carry"};
  m_instructionArray[instr_index++] = {0x0000, 0b0000001000000000, 0b1111111100000000, InstructionAVR_en::MULS, 2, 1, "MULS : Multiply Signed"};
  m_instructionArray[instr_index++] = {0x0000, 0b0000001100000000, 0b1111111110001000, InstructionAVR_en::MULSU, 2, 1, "MULSU : Multiply Signed with Unsigned"};
  m_instructionArray[instr_index++] = {0x0000, 0b0010000000000000, 0b1111110000000000, InstructionAVR_en::AND, 2, 1, "AND : Logical AND"};
  m_instructionArray[instr_index++] = {0x0000, 0b0111000000000000, 0b1111000000000000, InstructionAVR_en::ANDI, 2, 1, "ANDI : Logical AND with Immediate"};
  m_instructionArray[instr_index++] = {0x0000, 0b0010100000000000, 0b1111110000000000, InstructionAVR_en::OR, 2, 1, "OR : Logical OR"};
  m_instructionArray[instr_index++] = {0x0000, 0b0110000000000000, 0b1111000000000000, InstructionAVR_en::ORI, 2, 1, "ORI : Logical OR with Immediate"};
  m_instructionArray[instr_index++] = {0x0000, 0b0010010000000000, 0b1111110000000000, InstructionAVR_en::EOR, 2, 1, "EOR : Exclusive OR"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000000000, 0b1111111000001111, InstructionAVR_en::COM, 2, 1, "COM : One's Complement"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000000001, 0b1111111000001111, InstructionAVR_en::NEG, 2, 1, "NEG : Two's Complement"};
  m_instructionArray[instr_index++] = {0x0000, 0b0110000000000000, 0b1111000000000000, InstructionAVR_en::SBR, 2, 1, "SBR : Set Bit(s) in Register (Same as ORI)"};
  m_instructionArray[instr_index++] = {0x0000, 0b0111000000000000, 0b1111000000000000, InstructionAVR_en::CBR, 2, 1, "CBR : Clear Bit(s) in Register (ANDI with K complemented)"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000000011, 0b1111111000001111, InstructionAVR_en::INC, 2, 1, "INC : Increment"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000001010, 0b1111111000001111, InstructionAVR_en::DEC, 2, 1, "DEC : Decrement"};
  m_instructionArray[instr_index++] = {0x0000, 0b0010000000000000, 0b1111110000000000, InstructionAVR_en::TST, 2, 1, "TST : Test for Zero or Minus (Same as AND)"};
  m_instructionArray[instr_index++] = {0x0000, 0b0010010000000000, 0b1111110000000000, InstructionAVR_en::CLR, 2, 1, "CLR : Clear Register (Same as EOR with the same register)"};
  m_instructionArray[instr_index++] = {0x0000, 0b1110111100001111, 0b1111111100001111, InstructionAVR_en::SER, 2, 1, "SER : Set Register"};
  m_instructionArray[instr_index++] = {0x0000, 0b1110000000000000, 0b1111000000000000, InstructionAVR_en::LDI, 2, 1, "LDI : Load Immediate"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000001100, 0b1111111000001111, InstructionAVR_en::LD_X1, 2, 1, "LD_X1 : Load Indirect from Data Space to Register using Index X"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000001101, 0b1111111000001111, InstructionAVR_en::LD_X2, 2, 1, "LD_X2 : Load Indirect from Data Space to Register using Index X"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000001110, 0b1111111000001111, InstructionAVR_en::LD_X3, 2, 1, "LD_X3 : Load Indirect from Data Space to Register using Index X"};
  m_instructionArray[instr_index++] = {0x0000, 0b1000000000001000, 0b1111111000001111, InstructionAVR_en::LD_Y1, 2, 1, "LD_Y1 : Load Indirect from Data Space to Register using Index Y"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000001001, 0b1111111000001111, InstructionAVR_en::LD_Y2, 2, 1, "LD_Y2 : Load Indirect from Data Space to Register using Index Y"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000001010, 0b1111111000001111, InstructionAVR_en::LD_Y3, 2, 1, "LD_Y3 : Load Indirect from Data Space to Register using Index Y"};
  m_instructionArray[instr_index++] = {0x0000, 0b1000000000001000, 0b1101001000001000, InstructionAVR_en::LD_Y4, 2, 1, "LD_Y4 : Load Indirect from Data Space to Register using Index Y"};
  m_instructionArray[instr_index++] = {0x0000, 0b1000000000000000, 0b1111111000001111, InstructionAVR_en::LD_Z1, 2, 1, "LD_Z1 : Load Indirect from Data Space to Register using Index Z"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000000001, 0b1111111000001111, InstructionAVR_en::LD_Z2, 2, 1, "LD_Z2 : Load Indirect from Data Space to Register using Index Z"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000000010, 0b1111111000001111, InstructionAVR_en::LD_Z3, 2, 1, "LD_Z3 : Load Indirect from Data Space to Register using Index Z"};
  m_instructionArray[instr_index++] = {0x0000, 0b1000000000000000, 0b1101001000001000, InstructionAVR_en::LD_Z4, 2, 1, "LD_Z4 : Load Indirect from Data Space to Register using Index Z"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001001000001100, 0b1111111000001111, InstructionAVR_en::ST_X1, 2, 1, "ST_X1 : Store Indirect From Register to Data Space using Index X"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001001000001101, 0b1111111000001111, InstructionAVR_en::ST_X2, 2, 1, "ST_X2 : Store Indirect From Register to Data Space using Index X"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001001000001110, 0b1111111000001111, InstructionAVR_en::ST_X3, 2, 1, "ST_X3 : Store Indirect From Register to Data Space using Index X"};
  m_instructionArray[instr_index++] = {0x0000, 0b1000001000001000, 0b1111111000001111, InstructionAVR_en::ST_Y1, 2, 1, "ST_Y1 : Store Indirect From Register to Data Space using Index Y"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001001000001001, 0b1111111000001111, InstructionAVR_en::ST_Y2, 2, 1, "ST_Y2 : Store Indirect From Register to Data Space using Index Y"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001001000001010, 0b1111111000001111, InstructionAVR_en::ST_Y3, 2, 1, "ST_Y3 : Store Indirect From Register to Data Space using Index Y"};
  m_instructionArray[instr_index++] = {0x0000, 0b1000001000001000, 0b1101001000001000, InstructionAVR_en::ST_Y4, 2, 1, "ST_Y4 : Store Indirect From Register to Data Space using Index Y"};
  m_instructionArray[instr_index++] = {0x0000, 0b1000001000000000, 0b1111111000001111, InstructionAVR_en::ST_Z1, 2, 1, "ST_Z1 : Store Indirect From Register to Data Space using Index Z"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001001000000001, 0b1111111000001111, InstructionAVR_en::ST_Z2, 2, 1, "ST_Z2 : Store Indirect From Register to Data Space using Index Z"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001001000000010, 0b1111111000001111, InstructionAVR_en::ST_Z3, 2, 1, "ST_Z3 : Store Indirect From Register to Data Space using Index Z"};
  m_instructionArray[instr_index++] = {0x0000, 0b1000001000000000, 0b1101001000001000, InstructionAVR_en::ST_Z4, 2, 1, "ST_Z4 : Store Indirect From Register to Data Space using Index Z"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010111001000, 0b1111111111111111, InstructionAVR_en::LPM_1, 2, 1, "LPM_1 : Load Program Memory"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000000100, 0b1111111000001111, InstructionAVR_en::LPM_2, 2, 1, "LPM_2 : Load Program Memory"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000000101, 0b1111111000001111, InstructionAVR_en::LPM_3, 2, 1, "LPM_3 : Load Program Memory"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001011100000000, 0b1111111100000000, InstructionAVR_en::SBIW, 2, 1, "SBIW : Subtract Immediate from Word"};
  m_instructionArray[instr_index++] = {0x0000, 0b1010100000000000, 0b1111100000000000, InstructionAVR_en::STS16bit, 2, 1, "STS16bit : 16bit Store Direct to Data Space"};
  m_instructionArray[instr_index++] = {0x0000, 0b0010110000000000, 0b1111110000000000, InstructionAVR_en::MOV, 2, 1, "MOV : Copy Register"};
  m_instructionArray[instr_index++] = {0x0000, 0b0000000100000000, 0b1111111100000000, InstructionAVR_en::MOVW, 2, 1, "MOVW : Copy Register Word"};

  //Branch Instructions:
  m_instructionArray[instr_index++] = {0x0000, 0b1100000000000000, 0b1111000000000000, InstructionAVR_en::RJMP, 2, 1, "RJMP : Relative Jump"};
  m_instructionArray[instr_index++] = {0x0000, 0b1101000000000000, 0b1111000000000000, InstructionAVR_en::RCALL, 2, 1, "RCALL : Relative Call to Subroutine"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010100001000, 0b1111111111111111, InstructionAVR_en::RET, 2, 1, "RET : Return from Subroutine"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010100011000, 0b1111111111111111, InstructionAVR_en::RETI, 2, 1, "RETI : Return from Interrupt"};
  m_instructionArray[instr_index++] = {0x0000, 0b0001000000000000, 0b1111110000000000, InstructionAVR_en::CPSE, 2, 1, "CPSE : Compare, Skip if Equal"};
  m_instructionArray[instr_index++] = {0x0000, 0b0001010000000000, 0b1111110000000000, InstructionAVR_en::CP, 2, 1, "CP : Compare"};
  m_instructionArray[instr_index++] = {0x0000, 0b0000010000000000, 0b1111110000000000, InstructionAVR_en::CPC, 2, 1, "CPC : Compare with Carry"};
  m_instructionArray[instr_index++] = {0x0000, 0b0011000000000000, 0b1111000000000000, InstructionAVR_en::CPI, 2, 1, "CPI : Compare with Immediate"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111110000000000, 0b1111111000001000, InstructionAVR_en::SBRC, 2, 1, "SBRC : Skip if Bit in Register is Cleared"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111111000000000, 0b1111111000001000, InstructionAVR_en::SBRS, 2, 1, "SBRS : Skip if Bit in Register is Set"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001100100000000, 0b1111111100000000, InstructionAVR_en::SBIC, 2, 1, "SBIC : Skip if Bit in I/O Register is Cleared"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001101100000000, 0b1111111100000000, InstructionAVR_en::SBIS, 2, 1, "SBIS : Skip if Bit in I/O Register is Set"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000001, 0b1111110000000111, InstructionAVR_en::BREQ, 2, 1, "BREQ : Branch if Equal"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111010000000001, 0b1111110000000111, InstructionAVR_en::BRNE, 2, 1, "BRNE : Branch if Not Equal"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000000, 0b1111110000000111, InstructionAVR_en::BRCS, 2, 1, "BRCS : Branch if Carry Set"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111010000000000, 0b1111110000000111, InstructionAVR_en::BRCC, 2, 1, "BRCC : Branch if Carry Cleared"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000000, 0b1111110000000111, InstructionAVR_en::BRSH, 2, 1, "BRSH : Branch if Same or Higher (Same as BRCC)"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000000, 0b1111110000000111, InstructionAVR_en::BRLO, 2, 1, "BRLO : Branch if Lower (Same as BRCS)"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000010, 0b1111110000000111, InstructionAVR_en::BRMI, 2, 1, "BRMI : Branch if Minus"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111010000000010, 0b1111110000000111, InstructionAVR_en::BRPL, 2, 1, "BRPL : Branch if Plus"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111010000000100, 0b1111110000000111, InstructionAVR_en::BRGE, 2, 1, "BRGE : Branch if Greater or Equal (Signed)"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000100, 0b1111110000000111, InstructionAVR_en::BRLT, 2, 1, "BRLT : Branch if Less Than (Signed)"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000101, 0b1111110000000111, InstructionAVR_en::BRHS, 2, 1, "BRHS : Branch if Half Carry Flag is Set"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111010000000101, 0b1111110000000111, InstructionAVR_en::BRHC, 2, 1, "BRHC : Branch if Half Carry Flag is Cleared"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000110, 0b1111110000000111, InstructionAVR_en::BRTS, 2, 1, "BRTS : Branch if T Flag Set"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111010000000110, 0b1111110000000111, InstructionAVR_en::BRTC, 2, 1, "BRTC : Branch if T Flag Cleared"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000011, 0b1111110000000111, InstructionAVR_en::BRVS, 2, 1, "BRVS : Branch if Overflow Flag is Set"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111010000000011, 0b1111110000000111, InstructionAVR_en::BRVC, 2, 1, "BRVC : Branch if Overflow Flag is Cleared"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000111, 0b1111110000000111, InstructionAVR_en::BRIE, 2, 1, "BRIE : Branch if Interrupt Enabled"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111010000000111, 0b1111110000000111, InstructionAVR_en::BRID, 2, 1, "BRID : Branch if Interrupt Disabled"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111000000000000, 0b1111110000000000, InstructionAVR_en::BRBS, 2, 1, "BRBS : Branch if Bit in SREG is Set"};
  m_instructionArray[instr_index++] = {0x0000, 0b1111010000000000, 0b1111110000000000, InstructionAVR_en::BRBC, 2, 1, "BRBC : Branch if Bit in SREG is Cleared"};

  //Bit and Bit-test Instructions:
  m_instructionArray[instr_index++] = {0x0000, 0b1001101000000000, 0b1111111100000000, InstructionAVR_en::SBI, 2, 1, "SBI : Set Bit in I/O Register"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001100000000000, 0b1111111100000000, InstructionAVR_en::CBI, 2, 1, "CBI : Clear Bit in I/O Register"};
  m_instructionArray[instr_index++] = {0x0000, 0b0000110000000000, 0b1111110000000000, InstructionAVR_en::LSL, 2, 1, "LSL : Logical Shift Left (Same as ADD Rd,Rd)"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000000110, 0b1111111000001111, InstructionAVR_en::LSR, 2, 1, "LSR : Logical Shift Right"};
  m_instructionArray[instr_index++] = {0x0000, 0b0001110000000000, 0b1111110000000000, InstructionAVR_en::ROL, 2, 1, "ROL : Rotate Left Through Carry (Same as ADC Rd,Rd)"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000000111, 0b1111111000001111, InstructionAVR_en::ROR, 2, 1, "ROR : Rotate Right Through Carry"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000000101, 0b1111111000001111, InstructionAVR_en::ASR, 2, 1, "ASR : Arithmetic Shift Right"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000000010, 0b1111111000001111, InstructionAVR_en::SWAP, 2, 1, "SWAP : Swap Nibbles"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010001111000, 0b1111111111111111, InstructionAVR_en::SEI, 2, 1, "SEI : Set Global Interrupt Flag"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000001000, 0b1111111110001111, InstructionAVR_en::BSET, 2, 1, "BSET : Flag Set (where sss is the bit in SREG)"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010011111000, 0b1111111111111111, InstructionAVR_en::CLI, 2, 1, "CLI : Clear Global Interrupt Flag"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010010001000, 0b1111111110001111, InstructionAVR_en::BCLR, 2, 1, "BCLR : Flag Clear (where sss is the bit in SREG)"};

  //MCU Control Instructions:
  m_instructionArray[instr_index++] = {0x0000, 0b0000000000000000, 0b1111111111111111, InstructionAVR_en::NOP, 2, 1, "NOP : No Operation"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010110001000, 0b1111111111111111, InstructionAVR_en::SLEEP, 2, 1, "SLEEP : Sleep"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010110101000, 0b1111111111111111, InstructionAVR_en::WDR, 2, 1, "WDR : Watchdog Reset"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010110011000, 0b1111111111111111, InstructionAVR_en::BREAK, 2, 1, "BREAK : Break"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000001111, 0b1111111000001111, InstructionAVR_en::POP, 2, 1, "POP : Load register Rd with a byte from the STACK"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001001000001111, 0b1111111000001111, InstructionAVR_en::PUSH, 2, 1, "PUSH : stores the contents of register Rr on the STACK"};
  m_instructionArray[instr_index++] = {0x0000, 0b1011000000000000, 0b1111100000000000, InstructionAVR_en::IN, 2, 1, "IN : Load an I/O Location to Register"};
  m_instructionArray[instr_index++] = {0x0000, 0b1011100000000000, 0b1111100000000000, InstructionAVR_en::OUT, 2, 1, "OUT : Store Register to I/O Location"};

  //***************** 32-bit Opcode *****************
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000001110, 0b1111111000001110, InstructionAVR_en::CALL, 4, 1, "CALL : Long Call to a Subroutine"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001010000001100, 0b1111111000001110, InstructionAVR_en::JMP, 4, 1, "JMP : Jump"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001000000000000, 0b1111111000001111, InstructionAVR_en::LDS, 4, 1, "LDS : Load Direct from Data Space"};
  m_instructionArray[instr_index++] = {0x0000, 0b1001001000000000, 0b1111111000001111, InstructionAVR_en::STS, 4, 1, "STS : Store Direct to Data Space"};
}

InstructionAVR::~InstructionAVR()
{
}

// Implement the decode16bits function for InstructionAVR
bool InstructionAVR::decode(Instruction_st &decoded_instruction, uint16_t instructionL, uint16_t instructionH, uint8_t &instruction_size)
{
  // std::cout << "Decoding AVR instruction: " << std::hex << instructionL << std::dec << std::endl;
  bool instructionFound = false;
  uint32_t instructionRaw;

  for (const auto &entry : m_instructionArray)
  {
    if ((instructionL & entry.mask) == entry.pattern && entry.mask != 0)
    {

      instructionFound = true;
      instruction_size = entry.instructionSize;

      if (instruction_size == 2)
      {
        instructionRaw = instructionL;
        AVR_tlm_sim::log::printHexBytes(instructionL, 2, "[InstructionAVR] ");
      }
      else if (instruction_size == 4)
      {
        instructionRaw = (uint32_t)(instructionH) << 16 | (uint32_t)(instructionL);
        AVR_tlm_sim::log::printHexBytes(instructionRaw, 4, "[InstructionAVR] ");
      }

      decoded_instruction = entry;
      decoded_instruction.instructionRaw = instructionRaw;
      break;
    }
  }

  if (!instructionFound)
  {
    AVR_tlm_sim::log::printInfo("[InstructionAVR] Unknown instruction.");
    instruction_size = 2;
  }

  return instructionFound;
}
