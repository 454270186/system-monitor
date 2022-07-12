#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> token;
  token = LinuxParser::CpuUtilization();
  int Idle, idle, iowait;
  int user, nice, system, irq, softirq, steal;
  int Total;


  steal = std::stoi(token.back()); 
  token.pop_back();  
  softirq = std::stoi(token.back());
  token.pop_back();
  irq = std::stoi(token.back());
  token.pop_back();
  iowait = std::stoi(token.back());
  token.pop_back();
  idle = std::stoi(token.back());
  token.pop_back();
  system = std::stoi(token.back());
  token.pop_back();
  nice = std::stoi(token.back());
  token.pop_back();
  user = std::stoi(token.back());
  token.pop_back();
  Idle = idle+iowait;
  Total = Idle + user + nice + system + irq + softirq + steal;

  return (float)(Total - Idle)/Total;
}