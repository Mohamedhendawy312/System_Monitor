#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid):pid_(pid){};

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  string line;
  string key;
  int kutime_ = 12;
  int kstime_ = 13;
  int kcutime_ = 14;
  int kcstime_ = 15;
  int kstarttime_ = 20;
  std::string pid_para;
  vector<string> value;
  std::string pid_st = std::to_string(pid_);
  std::ifstream filestream(LinuxParser::kProcDirectory + pid_st +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    if (pid_ == std::stoi(key)) {
      for (int i = 0; i < 21; i++) {
        linestream >> pid_para;
        value.push_back(pid_para);
        // std::cout << cpu_time << std::endl;
      }
    }
  }
  const float freq = sysconf(_SC_CLK_TCK);
  const float total_s =
      std::stol(value[kutime_]) +
      (std::stol(value[kstime_]) + std::stol(value[kcutime_]) +
       std::stol(value[kcstime_])) /
          freq;
  const float result = total_s / std::stol(value[kstarttime_]);
  return result;  // (22) starttime
}

// TODO: Return the command that generated this process
string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return mem_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return up_time_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return up_time_ < a.up_time_;
}