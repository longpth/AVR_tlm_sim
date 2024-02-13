#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include "MicroSim.h"

#define PORT_SERVER 6868
#define BUFFER_SIZE 1024

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
  m_stop_gdb_server.store(true);
  m_gdb_server_thread.join();
}

void MicroSim::runGdbServer() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};

  // Create a socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set socket options
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  // Bind socket to port
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT_SERVER);
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Listen for connections
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // Accept incoming connections
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  // Handle GDB commands
  while (!m_stop_gdb_server.load()) {
    memset(buffer, 0, BUFFER_SIZE);
    read(new_socket, buffer, BUFFER_SIZE);
    std::cout << "Received: " << buffer << std::endl;
    std::string command(buffer);
    // Process GDB command
    const char* response = processGDBCommand(command).c_str();
    // send the response
    send(new_socket, response, strlen(response), 0);
  }
}

std::string MicroSim::processGDBCommand(std::string command)
{
  std::string response;

  // Process GDB command and send response
  if (command.substr(0, 2) == "x ") {
      // Examine memory command
      // Extract address and count from command
      std::istringstream iss(command.substr(2));
      std::string address;
      int count;
      iss >> address >> count;
      response = examineMemory(address, count);
  } else if (command == "info registers") {
      // Display registers command
      response = displayRegisters();
  } else if (command.substr(0, 3) == "x/s") {
      // Display memory as string command
      // Extract address and count from command
      std::istringstream iss(command.substr(3));
      std::string address;
      int count;
      iss >> address >> count;
      response = displayMemoryAsString(address, count);
  } else if (command.substr(0, 3) == "x/i") {
      // Display memory as instructions command
      // Extract address and count from command
      std::istringstream iss(command.substr(3));
      std::string address;
      int count;
      iss >> address >> count;
      response = displayMemoryAsInstructions(address, count);
  } else {
      // Invalid command
      response = "Unknown command: " + command;
  }
}

// Function to examine memory
std::string MicroSim::examineMemory(const std::string& address, int count) {
    // TODO: Implement memory examination logic here
    // Example: Return "Memory content at address <address>: <content>"
    std::stringstream response;
    response << "Memory content at address " << address << ": <content>";
    return response.str();
}

// Function to display contents of all registers
std::string MicroSim::displayRegisters() {
    // TODO: Implement register display logic here
    // Example: Return "Register contents: <contents>"
    return "Register contents: <contents>";
}

// Function to display memory as a string
std::string MicroSim::displayMemoryAsString(const std::string& address, int count) {
    // TODO: Implement memory display as string logic here
    // Example: Return "Memory content as string at address <address>: <content>"
    std::stringstream response;
    response << "Memory content as string at address " << address << ": <content>";
    return response.str();
}

// Function to display memory as instructions
std::string MicroSim::displayMemoryAsInstructions(const std::string& address, int count) {
    // TODO: Implement memory display as instructions logic here
    // Example: Return "Memory content as instructions at address <address>: <content>"
    std::stringstream response;
    response << "Memory content as instructions at address " << address << ": <content>";
    return response.str();
}