#include <iomanip>
#include "Flash.h"

#define DEBUG

Flash::Flash(const sc_module_name &name, int flashSize) : sc_module(name), bus_in_socket("bus_in_socket"), m_programflash_size(flashSize)
{
  // Initialize the flash memory array here if needed
  m_programflash_data.fill(0);
  // Register the b_transport callback with the target socket
  bus_in_socket.register_b_transport(this, &Flash::b_transport);
}

Status Flash::loadHex(const std::string &hexFile, bool isIntelHex)
{
  Status ret = Status::OK; // Initialize the return variable to true
  if (isIntelHex)
  {
    std::ifstream file(hexFile);

    if (!file.is_open())
    {
      std::cerr << "Failed to open file: " << hexFile << std::endl;
      ret = Status::ERROR; // Set ret to false on failure
    }
    else
    {
      int highestAddress = -1; // Initialize with a negative value

      std::string line;
      while (std::getline(file, line))
      {
        if (line.empty() || line[0] != ':')
        {
          std::cerr << "Invalid hex record: " << line << std::endl;
          // Handle the error as needed
          return Status::ERROR; // Terminate the program on an error
        }

        std::string byteCountStr = line.substr(1, 2);
        std::string addressStr = line.substr(3, 4);
        std::string recordTypeStr = line.substr(7, 2);
        std::string dataStr = line.substr(9, line.length() - 11);

        int byteCount = std::stoi(byteCountStr, nullptr, 16);
        int address = std::stoi(addressStr, nullptr, 16);
        int recordType = std::stoi(recordTypeStr, nullptr, 16);

        if (recordType == 0x00)
        {
          for (int i = 0; i < byteCount; ++i)
          {
            std::string byteStr = dataStr.substr(i * 2, 2);
            uint8_t byteValue = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
            m_programflash_data[address + i] = byteValue;
          }

          // Update the highest address encountered
          highestAddress = std::max(highestAddress, address + byteCount - 1);
        }
      }

      // Calculate the size of the hex file in bytes
      int hexFileSize = highestAddress + 1;

      m_programflash_size = hexFileSize;

      file.close();

#ifdef DEBUG
      for (int i = 0; i < 160; i += 16) // Print in groups of 16 bytes
      {
        std::cout << "0x" << std::setw(4) << std::setfill('0') << i << ": ";

        for (int j = 0; j < 16; j++)
        {
          std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(m_programflash_data[i + j]) << " ";
        }

        std::cout << "\n";
      }
#endif
    }
  }
  else
  {
    //TODO: motorola hex not yet implemented
    ret = Status::ERROR;
  }
  return ret;
}

void Flash::b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
{
  tlm::tlm_command cmd = trans.get_command();
  sc_dt::uint64 addr = trans.get_address();
  unsigned char *ptr = trans.get_data_ptr();
  unsigned int len = trans.get_data_length();
  unsigned char *byt = trans.get_byte_enable_ptr();
  unsigned int wid = trans.get_streaming_width();

  // Check if the access is within the bounds of the flash memory
  if (addr >= m_programflash_size || (addr + len) > m_programflash_size + 4 /*because we take 4 bytes for each instruction*/)
  {
    trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    return;
  }

  // Perform the operation
  if (cmd == tlm::TLM_READ_COMMAND)
  {
    // Copy data from memory to payload's data pointer
    memcpy(ptr, &m_programflash_data[addr], len);
  }
  else if (cmd == tlm::TLM_WRITE_COMMAND)
  {
    // Copy data from payload's data pointer to memory
    memcpy(&m_programflash_data[addr], ptr, len);
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