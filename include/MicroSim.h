#pragma once

#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <systemc.h>
#include <string>
#include <thread>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <atomic> // For std::atomic_flag
#include "Mytype.h"
#include "CPU.h"
#include "Flash.h"
#include "Bus.h"
#include "SRam.h"

class MicroSim : public sc_module
{
public:
  CPU *m_cpu;
  Flash *m_flash;
  Bus *m_busProgam;
  SRam *m_sram;
  Bus *m_busData;

  MicroSim(const sc_module_name &name, const std::string &hexFile);

  // Destructor to clean up allocated memory
  ~MicroSim();

  void startGdbServer();

  void stopGdbServer();

private:
  std::thread m_gdb_server_thread;
  std::atomic<bool> m_stop_gdb_server;

  void runGdbServer();
  std::string processGDBCommand(std::string command);
  std::string examineMemory(const std::string& address, int count);
  std::string displayRegisters();
  std::string displayMemoryAsString(const std::string& address, int count);
  std::string displayMemoryAsInstructions(const std::string& address, int count);
};
