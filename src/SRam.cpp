#include "SRam.h"

SRam::SRam(const sc_module_name &name)
    : sc_module(name),               // Initialize the sc_module base part of this class
      bus_in_socket("bus_in_socket") // Initialize the TLM bus_in_socket with its name
{
  // Register the b_transport method as the transport handler for incoming transactions
  bus_in_socket.register_b_transport(this, &SRam::b_transport);

  // Initialize the SRAM data to 0
  memset(&m_sram_data, 0, sizeof(m_sram_data));

  // If you need to perform any additional setup or registrations, do it here
}

void SRam::b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
{
  tlm::tlm_command cmd = trans.get_command();
  uint16_t addr = trans.get_address();
  unsigned char *ptr = trans.get_data_ptr();
  unsigned int len = trans.get_data_length();

  // Check if the access is within the bounds of the flash memory
  if (addr >= SRAM_DEFAULT_SIZE || (addr + len) > SRAM_DEFAULT_SIZE)
  {
    trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    return;
  }

  // Perform the operation
  if (cmd == tlm::TLM_READ_COMMAND)
  {
    // Copy data from memory to payload's data pointer
    memcpy(ptr, &m_sram_data.sram_data[addr], len);
  }
  else if (cmd == tlm::TLM_WRITE_COMMAND)
  {
    // Copy data from payload's data pointer to memory
    memcpy(&m_sram_data.sram_data[addr], ptr, len);
  }
  else
  {
    // Command not supported
    trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
    return;
  }

  // Set response status to indicate successful completion
  trans.set_response_status(tlm::TLM_OK_RESPONSE);
}