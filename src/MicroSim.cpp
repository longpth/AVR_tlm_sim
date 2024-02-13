#include "MicroSim.h"

MicroSim::MicroSim(const sc_module_name &name, const std::string &hexFile) : sc_module(name)
{
  // Create instances
  m_cpu = new CPU("cpu", sc_time(10, SC_NS));
  m_flash = new Flash("flash");
  m_flash->loadHex(hexFile);
  m_busProgam = new Bus("busProgram");
  m_sram = new SRam("sram");
  m_busData = new Bus("busData");

  // Connecting cpu to flash
  m_cpu->cpu_program_out_socket.bind(m_busProgam->bus_in_socket);
  m_busProgam->bus_out_socket.bind(m_flash->bus_in_socket);

  // Connect cpu to sram
  m_cpu->cpu_data_out_socket.bind(m_busData->bus_in_socket);
  m_busData->bus_out_socket.bind(m_sram->bus_in_socket);
}

MicroSim::~MicroSim()
{
  delete m_cpu;
  delete m_flash;
  delete m_busProgam;
  delete m_sram;
  delete m_busData;
}

void MicroSim::startGdbServer()
{
  m_gdb_server_thread = std::thread(&MicroSim::runGdbServer, this);
}

void MicroSim::stopGdbServer()
{
  m_gdb_server_thread.join();
}

void MicroSim::runGdbServer()
{
  
}