#pragma once
#include "cpu.h"
#include "load_average.h"
#include "process.h"
#include <vector>


/**
 * Data structure that aggregates all required information about a system at a
 * given point in time.
 */
struct SystemInfo {
  // The number of processes currently existing on the system.
  unsigned num_processes;

  // The total number of threads (kernel and user-land) currently existing on
  // the system.
  unsigned num_threads;

  // The total number of user-land threads currently existing on the system.
  unsigned num_user_threads;

  // The total number of kernel threads currently existing on the system.
  unsigned num_kernel_threads;

  // The total number of tasks that are currently in the 'running' state.
  unsigned num_running;

  // The amount of time, in seconds, that the system has been running.
  double uptime;

  // The average load of the system over the last 1, 5, and 15 minutes.
  LoadAverage load_average;

  // Information about the time each CPU (and the CPUs overall) have spent in
  // various states
  std::vector<Cpu> cpus;

  // A list of all processes currently existing on the system.
  std::vector<Process> processes;
};
