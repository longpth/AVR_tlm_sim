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

    // Process GDB command and send response

    // Example response
    const char* response = "OK";
    send(new_socket, response, strlen(response), 0);
  }
}