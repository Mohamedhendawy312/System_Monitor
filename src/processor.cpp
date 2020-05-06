#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> value = LinuxParser::CpuUtilization();

  double user = std::stof(value[LinuxParser::kUser_]);
  double nice = std::stof(value[LinuxParser::kNice_]);
  double system = std::stof(value[LinuxParser::kSystem_]);
  double idle = std::stof(value[LinuxParser::kIdle_]);
  double iowait = std::stof(value[LinuxParser::kIOwait_]);
  double irq = std::stof(value[LinuxParser::kIRQ_]);
  double softirq = std::stof(value[LinuxParser::kSoftIRQ_]);
  double steal = std::stof(value[LinuxParser::kSteal_]);
  //double guest = std::stof(value[LinuxParser::kGuest_]);
  double guest_nice = std::stof(value[LinuxParser::kGuestNice_]);

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