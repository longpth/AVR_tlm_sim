#include "Bus.h"

Bus::Bus(const sc_module_name &name) : sc_module(name), bus_in_socket("bus_in_socket"), bus_out_socket("bus_out_socket")
{
  // Registering the b_transport method with the target socket
  bus_in_socket.register_b_transport(this, &Bus::b_transport);
}

Bus::~Bus()
{
  // Destructor implementation (if needed)
}

void Bus::b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
{
  // Simple pass-through behavior
  // Forward the transaction to the initiator socket
  bus_out_socket->b_transport(trans, delay);
}

// Additional bus functionalities can be implemented here