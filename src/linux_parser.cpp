#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <iostream>

#include "linux_parser.h"
#include <boost/algorithm/string.hpp>
#include "processor.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  string line;
  vector<string> token;
  vector<float> mem_use;
  string delim = " ";
  std::ifstream mem_file;
  string filename = "/proc/meminfo";
  mem_file.open(filename);
  
  for(int i = 0; i < 4; i++)
  {
    getline(mem_file, line);
    int pos = 0;
    
    
    //npos可以表示string的结束位子，是string::type_size 类型的，也就是find（）返回的类型。find函数在找不到指定值得情况下会返回string::npos
    while(pos = (int)line.find(delim) != (int)string::npos)
    {
      token.push_back(line.substr(0, pos));
      line.erase(0, pos + delim.length());
    }
    mem_use.push_back(stof(token.back()));
  }
	mem_file.close();
                      
	return (mem_use[1] - mem_use[0]) / mem_use[0];
   
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  vector<string> token;
  string delim = " ";
  std::ifstream uptime_file;
  //打开process status文件
  uptime_file.open("/proc/uptime");
  getline(uptime_file, line);//只读取第一行
  
  boost::algorithm::split(token, line, boost::is_any_of(" "));
  uptime_file.close();
  
  return stoi(token.front()) + stoi(token.back());
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {return 0;}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  vector<string> token;
  string delim = " ";
  std::ifstream proc_file;
  
  //打开porcess status文件
  proc_file.open("/proc/stat");
  getline(proc_file, line);
  int pos = 0;
  
  while(pos = (int)line.find(delim) != (int)string::npos)
  {
    token.push_back(line.substr(0, pos));
    line.erase(0, pos + delim.length());
  }
  
  token.erase(token.begin());
  token.erase(token.begin());
  token.pop_back();
  proc_file.close();
  
  return token;
  
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  vector<string> token;
  string target = "process";
  string delim = " ";
  std::ifstream proc_file;
  
  string file_name = "/proc/stat";
  proc_file.open(file_name);
  int pos = 0;
  
  while(!proc_file.eof())
  {
    getline(proc_file, line);
    if(line.find(target) != string::npos)
    {
      while(pos = (int)line.find(delim) != (int)string::npos)
      {
        token.push_back(line.replace(line.begin(), line.begin()+10, ""));
      }
    }
  }
  
  proc_file.close();
  return stoi(token.front()); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  vector<string> token;
  string delim = " ";
  string target = "procs_running";
  std::ifstream proc_file;
  
  string file_name = "/proc/stat";
  proc_file.open(file_name);
  int pos = 0;
  
  while(!proc_file.eof())
  {
    getline(proc_file, line);
    if(line.find(target) != string::npos)
    {
      while(pos = (int)line.find(delim) != (int)string::npos)
      {
        token.push_back(line.replace(line.begin(), line.begin()+14, ""));
      }
    }
  }
  proc_file.close();
  return stoi(token.front());
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string file_name;
  std::ifstream cmdline_file;
  
  file_name = "/proc/" + to_string(pid) + "/cmdline";
  cmdline_file.open(file_name);
  getline(cmdline_file, line);
  
  cmdline_file.close();
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  char buffer[10];
  vector<string> token;
  string delim = " ";
  string target = "VmRSS";
  std::ifstream stat_file;
  
  int pos = 0;
  string file_name = "/proc/" + to_string(pid) + "/status";
  stat_file.open(file_name);
  
  while(!stat_file.eof())
  {
    getline(stat_file, line);
    if(line.find(target) != string::npos)
    {
      while(pos = (int)line.find(delim) != (int)string::npos)
      {
        token.push_back(line.substr(0, pos));
        line.erase(0, pos + delim.length());
      }
      break;
    }
  }
  std::sprintf(buffer, "%.2f", stof(token.back())/1000.0);
  string output(buffer);
  stat_file.close();
  
  
  return output;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  vector<string> token;
  string delim = "\t";
  string target = "Uid";
  std::ifstream stat_file;
  int pos = 0;
  
  string file_name = "/proc/" + to_string(pid) + "/status";
  stat_file.open(file_name);
  while(!stat_file.eof())
  {
    getline(stat_file, line);
    if(line.find(target) != string::npos)
    {
      while(pos = (int)line.find(delim) != (int)string::npos)
      {
        token.push_back(line.substr(0, pos));
        line.erase(0, pos + delim.length());
      }
      break;
    }
  }
  
  stat_file.close();
  return token[1];
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string unsername;
  string uid = Uid(pid);
  string line;
  vector<string> token;
  string delim = ":";
  string target = "x:" + uid + ":";
  std::ifstream etc_file;
  int pos = 0;
  
  string file_name = "/etc/passwd";
  etc_file.open(file_name);
  
  while(!etc_file.eof())
  {
    getline(etc_file, line);
    if(line.find(target) != string::npos)
    {
      while(pos = (int)line.find(delim) != (int)string::npos)
      {
        token.push_back(line.substr(0, pos));
        line.erase(0, pos + delim.length());
        
      }
      break;
    }
  }
  etc_file.close();
  return token[0];
  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  vector<string> token;
  string delim = " ";
  std::ifstream uptime_file;
  string proc_stat = "/proc/" + to_string(pid) + "/stat";
  
  uptime_file.open(proc_stat);
  getline(uptime_file, line);
  boost::algorithm::split(token, line, boost::is_any_of(" "));
  uptime_file.close();           
  return stoi(token[21]);
}