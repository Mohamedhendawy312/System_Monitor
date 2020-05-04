#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> value = LinuxParser::CpuUtilization();

  float user = std::stof(value[LinuxParser::kUser_]);
  float nice = std::stof(value[LinuxParser::kNice_]);
  float system = std::stof(value[LinuxParser::kSystem_]);
  float idle = std::stof(value[LinuxParser::kIdle_]);
  float iowait = std::stof(value[LinuxParser::kIOwait_]);
  float irq = std::stof(value[LinuxParser::kIRQ_]);
  float softirq = std::stof(value[LinuxParser::kSoftIRQ_]);
  float steal = std::stof(value[LinuxParser::kSteal_]);
  float guest = std::stof(value[LinuxParser::kGuest_]);
  float guest_nice = std::stof(value[LinuxParser::kGuestNice_]);

  float Total_CPU_time_since_boot =
      user + nice + system + idle + iowait + irq + softirq + steal;
  // std::cout << Total_CPU_time_since_boot << std::endl;
  float Total_CPU_Idle_time_since_boot = idle + iowait;
  // std::cout << Total_CPU_Idle_time_since_boot << std::endl;
  float Total_CPU_usage_time_since_boot =
      Total_CPU_time_since_boot - Total_CPU_Idle_time_since_boot;
  // std::cout << Total_CPU_usage_time_since_boot << std::endl;
  cpu_percentage = Total_CPU_usage_time_since_boot / Total_CPU_time_since_boot;

  return cpu_percentage;
}