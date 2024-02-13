#include <systemc.h>
#include <tlm.h>
#include "MicroSim.h"

int sc_main(int argc, char *argv[])
{
  cout << "sc_main of the vmcu_tlm_sin" << endl;

  std::cout << "Have " << argc << " arguments:\n";
  for (int i = 0; i < argc; ++i) {
      std::cout << argv[i] << "\n";
  }

  MicroSim mcu("atmega328p", argv[1]);

  // Start the simulation
  sc_start();

  return 0;
}