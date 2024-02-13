#pragma once

#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <systemc.h>
#include <string>
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
};
