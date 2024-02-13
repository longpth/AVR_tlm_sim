#pragma once

#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <systemc.h>
#include <string>
#include "Mytype.h"

#define FLASH_MAX_SIZE (8 * 1024 * 1024)
#define FLASH_DEFAULT_SIZE (32 * 1024)

class Flash : public sc_module
{
public:
  // Constructor
  Flash(const sc_module_name &name, int flashSize = FLASH_DEFAULT_SIZE);

  // Destructor
  ~Flash() {}

  // Method to load a hex file into the flash memory
  Status loadHex(const std::string &hexFile, bool isIntelHex = true);

  // The TLM socket for incoming communication
  tlm_utils::simple_target_socket<Flash> bus_in_socket;

  // The blocking transport method
  virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay);

private:
  std::array<uint8_t, FLASH_MAX_SIZE> m_programflash_data;
  uint32_t m_programflash_size;
};
