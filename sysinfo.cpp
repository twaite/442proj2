#include "sysinfo.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

using namespace std;


double get_uptime() {
  return 0.0;
}


LoadAverage get_load_average() {
  return LoadAverage();
}


MemoryInfo get_memory_info() {
  return MemoryInfo();
}


vector<Cpu> get_cpu_info() {
  return vector<Cpu>();
}


Process get_process(int pid, const char* basedir) {
  return Process();
}


vector<Process> get_all_processes(const char* basedir) {
  return vector<Process>();
}


SystemInfo get_system_info() {
  return SystemInfo();
}
