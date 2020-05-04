#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
float LinuxParser::MemoryUtilization() {
  float MemTotal, MemFree;
  string key, value, kb, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      	std::replace(line.begin(), line.end(), ':', ' ');
  		std::istringstream linestream(line);
    	while (linestream >> key >> value >> kb) {
        	if (key == "MemTotal") { MemTotal = std::stof(value);}
            if (key == "MemFree") { MemFree = std::stof(value);}
        	}
    	}
  }
  return (MemTotal - MemFree)/MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    long uptime_long;
  string uptime, idletime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  	std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    uptime_long = std::stol(uptime);
    }
  return uptime_long;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return (LinuxParser::IdleJiffies() + LinuxParser::ActiveJiffies());
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::istringstream linestream;
  string line, chuck;
  long jiffies;
  std::vector<string> v;
  if (stream.is_open()) {
    std::getline(stream, line);
    linestream.clear();
    linestream.str(line);
    for (size_t i = 0; i < 22; i++) {
      linestream >> chuck;
    }
    jiffies = std::stol(v[13]) + std::stol(v[14]) + std::stol(v[15]) +
              std::stol(v[16]);
  }
  return jiffies;
}

// TODO: Read and return the number of active jiffies for the system
  long LinuxParser::ActiveJiffies() {
  long aJiffies;
  vector<string> data = LinuxParser::CpuUtilization();
  for (string s : data) {
    aJiffies += std::stol(s);
  }
  aJiffies -= LinuxParser::IdleJiffies();
  aJiffies -= std::stol(data.back());
  return aJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    vector<string> data = LinuxParser::CpuUtilization();

  return (std::stol(data[3]) + std::stol(data[4]));
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
   vector<string> data;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::istringstream linestream;
  std::getline(stream, line);
  linestream.str(line);
  string chunk;
  linestream >> chunk;
  for (size_t i = 0; i < 10; i++) {
    linestream >> chunk;
    data.push_back(chunk);
  }

  return data;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::istringstream linestream;
  string line, key;
  int process = 0;
  while (stream.is_open()) {
    linestream.clear();
    std::getline(stream, line);
    linestream.str(line);
    linestream >> key;
    if (key == "processes") {
      linestream >> process;
      break;
    }
  }
  return process;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    std::ifstream stream(kProcDirectory + kStatFilename);
  std::istringstream linestream;
  string line, key;
  int process = 0;
  while (stream.is_open()) {
    linestream.clear();
    std::getline(stream, line);
    linestream.str(line);
    linestream >> key;
    if (key == "procs_running") {
      linestream >> process;
      break;
    }
  }
  return process;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::getline(stream, command);

  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::string line;
  std::string key;
  std::string value;
  int VmSize;
  std::string pid_st = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + pid_st + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          VmSize = std::stoi(value);
          VmSize /= 1024;  // 1MB = 1024KB
        }
      }
    }
  }
  return std::to_string(VmSize);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::istringstream linestream;
  string line, key;
  while (stream.is_open()) {
    linestream.clear();
    std::getline(stream, line);
    linestream.str(line);
    linestream >> key;
    if (key == "Uid:") {
      linestream >> key;
      break;
    }
  }
  return key;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string line;
  std::string name, uid, x;
  std::string user;
  std::string uid_st = LinuxParser::Uid(pid);
  std::string pid_st = std::to_string(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> x >> uid) {
        if (uid == uid_st) {
          user = name;
        }
      }
    }
  }
  return user;
}
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::istringstream linestream;
  string line, chuck;
  std::vector<string> v;
  if (stream.is_open()) {
    std::getline(stream, line);
    linestream.clear();
    linestream.str(line);
    for (size_t i = 0; i < 22; i++) {
      linestream >> chuck;
    }
    long time = std::stol(chuck) / sysconf(_SC_CLK_TCK);
    return time;
  }

  return 0;
}