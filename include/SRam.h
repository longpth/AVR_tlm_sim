#pragma once

#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <systemc.h>
#include <string>
#include "Mytype.h"

#define SRAM_DEFAULT_SIZE (2 * 1024)

union sram_un
{
  struct sram_st
  {
    uint8_t registers[32];                           // 32 General Purpose Registers 0x0000-0x001F
    uint8_t io_registers[64];                        // 64 I/O Registers             0x0020-0x005F
    uint8_t ext_io_registers[160];                   // 160 Extended I/O Registers   0x0060-0x00FF
    uint8_t sram[SRAM_DEFAULT_SIZE - 160 - 64 - 32]; // Internal SRAM with 1048 bytes of memory 0x0100-0x08FF
  };
  uint8_t sram_data[SRAM_DEFAULT_SIZE];
};

class SRam : public sc_module
{
public:
  // Constructor
  SRam(const sc_module_name &name);

  // Destructor
  ~SRam() {}

  // The TLM socket for incoming communication
  tlm_utils::simple_target_socket<SRam> bus_in_socket;

  // The blocking transport method
  virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay);

private:
  sram_un m_sram_data;
};