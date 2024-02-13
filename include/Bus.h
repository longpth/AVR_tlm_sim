#pragma once

#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <systemc.h>
#include "Mytype.h"

class Bus : public sc_module
{
public:
  // TLM sockets
  tlm_utils::simple_target_socket<Bus> bus_in_socket;
  tlm_utils::simple_initiator_socket<Bus> bus_out_socket;

  // Constructor
  Bus(const sc_module_name &name);

  // Destructor
  virtual ~Bus();

  // b_transport method
  void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay);

private:
  // Private members and helper functions (if any)
};