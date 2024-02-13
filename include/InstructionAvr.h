#pragma once

/*
***************** 16-bit Opcode **********************

Arithmetic Instructions:
0000 11rd dddd rrrr - ADD: Add without Carry
0001 11rd dddd rrrr - ADC: Add with Carry
1001 0110 KKdd KKKK - ADIW: Add Immediate to Word
0001 10rd dddd rrrr - SUB: Subtract without Carry
0101 KKKK dddd KKKK - SUBI: Subtract Immediate
0000 10rd dddd rrrr - SBC: Subtract with Carry
0100 KKKK dddd KKKK - SBCI: Subtract Immediate with Carry
0010 00rd dddd rrrr - AND: Logical AND
0111 KKKK dddd KKKK - ANDI: Logical AND with Immediate
0010 10rd dddd rrrr - OR: Logical OR
0110 KKKK dddd KKKK - ORI: Logical OR with Immediate
0010 01rd dddd rrrr - EOR: Exclusive OR
1001 010d dddd 0000 - COM: One's Complement
1001 010d dddd 0001 - NEG: Two's Complement
0110 KKKK dddd KKKK - SBR: Set Bit(s) in Register (Same as ORI)
0111 KKKK dddd KKKK - CBR: Clear Bit(s) in Register (ANDI with K complemented)
1001 010d dddd 0011 - INC: Increment
1001 010d dddd 1010 - DEC: Decrement
0010 00rd dddd rrrr - TST: Test for Zero or Minus (Same as AND)
0010 01rd dddd rrrr - CLR: Clear Register (Same as EOR with the same register)
1110 KKKK dddd KKKK - SER: Set Register

Branch Instructions:
1100 kkkk kkkk kkkk - RJMP: Relative Jump
1101 kkkk kkkk kkkk - RCALL: Relative Call to Subroutine
1001 0101 0000 1000 - RET: Return from Subroutine
1001 0101 0001 1000 - RETI: Return from Interrupt
0001 00rd dddd rrrr - CPSE: Compare, Skip if Equal
0001 01rd dddd rrrr - CP: Compare
0000 01rd dddd rrrr - CPC: Compare with Carry
0011 KKKK dddd KKKK - CPI: Compare with Immediate
1111 110r rrrr 0bbb - SBRC: Skip if Bit in Register is Cleared
1111 111r rrrr 0bbb - SBRS: Skip if Bit in Register is Set
1001 1001 AAAA Abbb - SBIC: Skip if Bit in I/O Register is Cleared
1001 1011 AAAA Abbb - SBIS: Skip if Bit in I/O Register is Set
1111 00kk kkkk ksss - BRBS: Branch if Bit in SREG is Set
1111 01kk kkkk ksss - BRBC: Branch if Bit in SREG is Cleared
1111 00kk kkkk k001 - BREQ: Branch if Equal
1111 01kk kkkk k001 - BRNE: Branch if Not Equal
1111 00kk kkkk k000 - BRCS: Branch if Carry Set
1111 01kk kkkk k000 - BRCC: Branch if Carry Cleared
1111 00kk kkkk k000 - BRSH: Branch if Same or Higher (Same as BRCC)
1111 00kk kkkk k000 - BRLO: Branch if Lower (Same as BRCS)
1111 00kk kkkk k010 - BRMI: Branch if Minus
1111 01kk kkkk k010 - BRPL: Branch if Plus
1111 01kk kkkk k100 - BRGE: Branch if Greater or Equal (Signed)
1111 00kk kkkk k100 - BRLT: Branch if Less Than (Signed)
1111 00kk kkkk k101 - BRHS: Branch if Half Carry Flag is Set
1111 01kk kkkk k101 - BRHC: Branch if Half Carry Flag is Cleared
1111 00kk kkkk k110 - BRTS: Branch if T Flag Set
1111 01kk kkkk k110 - BRTC: Branch if T Flag Cleared
1111 00kk kkkk k011 - BRVS: Branch if Overflow Flag is Set
1111 01kk kkkk k011 - BRVC: Branch if Overflow Flag is Cleared
1111 00kk kkkk k111 - BRIE: Branch if Interrupt Enabled
1111 01kk kkkk k111 - BRID: Branch if Interrupt Disabled

Bit and Bit-test Instructions:
1001 1010 AAAA Abbb - SBI: Set Bit in I/O Register
1001 1000 AAAA Abbb - CBI: Clear Bit in I/O Register
0000 11dd dddd dddd - LSL: Logical Shift Left (Same as ADD Rd,Rd)
1001 010d dddd 0110 - LSR: Logical Shift Right
0001 11dd dddd dddd - ROL: Rotate Left Through Carry (Same as ADC Rd,Rd)
1001 010d dddd 0111 - ROR: Rotate Right Through Carry
1001 010d dddd 0101 - ASR: Arithmetic Shift Right
1001 010d dddd 0010 - SWAP: Swap Nibbles
1001 0100 0sss 1000 - BSET: Flag Set (where sss is the bit in SREG)
1001 0100 1sss 1000 - BCLR: Flag Clear (where sss is the bit in SREG)

MCU Control Instructions:
0000 0000 0000 0000 - NOP: No Operation
1001 0101 1000 1000 - SLEEP: Sleep
1001 0101 1010 1000 - WDR: Watchdog Reset
1001 0101 1001 1000 - BREAK: Break

***************** 32-bit Opcode *****************
1001 010k kkkk 111k kkkk kkkk kkkk kkkk - CALL - Long Call to a Subroutine
1001 010k kkkk 110k kkkk kkkk kkkk kkkk - JMP - Jump
1001 000d dddd 0000 kkkk kkkk kkkk kkkk - LDS - Load Direct from Data Space
1001 001d dddd 0000 kkkk kkkk kkkk kkkk - STS - Store Direct to Data Space
*/

#include <iostream>
#include <map>
#include "Instruction.h"

enum InstructionAVR_en
{
  ADD = 0,
  ADC,
  ADIW,
  SUB,
  SUBI,
  SBC,
  SBCI,
  MULS,
  MULSU,
  AND,
  ANDI,
  OR,
  ORI,
  EOR,
  COM,
  NEG,
  SBR,
  CBR,
  INC,
  DEC,
  TST,
  CLR,
  SER,
  LDI,
  LD_X1,
  LD_X2,
  LD_X3,
  LD_Y1,
  LD_Y2,
  LD_Y3,
  LD_Y4,
  LD_Z1,
  LD_Z2,
  LD_Z3,
  LD_Z4,
  ST_X1,
  ST_X2,
  ST_X3,
  ST_Y1,
  ST_Y2,
  ST_Y3,
  ST_Y4,
  ST_Z1,
  ST_Z2,
  ST_Z3,
  ST_Z4,
  LPM_1,
  LPM_2,
  LPM_3,
  SBIW,
  STS16bit,
  MOV,
  MOVW,
  RJMP,
  RCALL,
  RET,
  RETI,
  CPSE,
  CP,
  CPC,
  CPI,
  SBRC,
  SBRS,
  SBIC,
  SBIS,
  BREQ,
  BRNE,
  BRCS,
  BRCC,
  BRSH,
  BRLO,
  BRMI,
  BRPL,
  BRGE,
  BRLT,
  BRHS,
  BRHC,
  BRTS,
  BRTC,
  BRVS,
  BRVC,
  BRIE,
  BRID,
  BRBS,
  BRBC,
  SBI,
  CBI,
  LSL,
  LSR,
  ROL,
  ROR,
  ASR,
  SWAP,
  SEI,
  BSET,
  CLI,
  BCLR,
  NOP,
  SLEEP,
  WDR,
  BREAK,
  POP,
  PUSH,
  IN,
  OUT,
  CALL,
  JMP,
  LDS,
  STS,
  END_OF_ENUM,
};

// Implementation of InstructionAVR
class InstructionAVR : public Instruction
{
public:
  InstructionAVR();
  ~InstructionAVR();
  bool decode(Instruction_st &decoded_instruction, uint16_t instructionL, uint16_t instructionH, uint8_t &instruction_size) override;

private:
  std::array<Instruction_st, END_OF_ENUM> m_instructionArray;
};
