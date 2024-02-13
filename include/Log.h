#pragma once

#include <iomanip>
#include <iostream>
#include <string>

// #define LOG_LEVEL_DEBUG
#define LOG_LEVEL_INFO
#define LOG_LEVEL_ERROR
// #define LOG_LEVEL_ALL

namespace AVR_tlm_sim
{
  namespace log
  {
    inline void printHexBytes(uint32_t number, uint8_t size, const std::string &infoMsg = "")
    {
#if defined(LOG_LEVEL_ALL) || defined(LOG_LEVEL_DEBUG)
      std::cout << "[DEBUG] " << infoMsg;
      for (int i = 0; i < size; ++i)
      {
        uint8_t byte = static_cast<uint8_t>(number & 0xFF); // Extract the least significant byte
        std::cout << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte) << " ";
        number >>= 8; // Shift right by 8 bits to get the next byte
      }
      std::cout << std::endl;
#endif
    }

#if defined(LOG_LEVEL_ALL) || defined(LOG_LEVEL_INFO)
    // Base case for the recursive variadic printInfo function
    inline void printInfo()
    {
      std::cout << std::endl;
    }

    // Recursive variadic printInfo function to handle multiple arguments
    template <typename T, typename... Args>
    void printInfo(const T &arg, Args... args)
    {
      extern bool infoPrefixPrinted;
      if (!infoPrefixPrinted)
      {
        std::cout << "[INFO] ";
        infoPrefixPrinted = true;
      }
      std::cout << arg;
      printInfo(args...);
      if (sizeof...(args) == 0)
      {
        infoPrefixPrinted = false;
      }
    }
#else
    // Define empty functions if LOG_LEVEL_ALL or LOG_LEVEL_INFO is not defined
    inline std::ostream &getDefaultOutputStream()
    {
      static std::ostream nullStream(nullptr);
      return nullStream;
    }

    inline void printInfo() {}
    template <typename T, typename... Args>
    void printInfo(const T &arg, Args... args) {}
#endif
  }
}
