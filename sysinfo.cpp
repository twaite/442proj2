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

	//Set up variables
	double seconds;
	ifstream uptime (string(string(PROC_ROOT) + "/uptime").c_str());

	//Check if file can be opened
	if ( uptime.is_open() ) {

		//Read in the time
		uptime >> seconds;

		//Close the file
		uptime.close();

		return seconds;

	} else {

		//error if file couldn't be opened
		cerr << "Error opening /proc/uptime" << endl;

	}

	return -1.0;
}


LoadAverage get_load_average() {

	//Variables
	LoadAverage loadavg = LoadAverage();
	string filename = string( string (PROC_ROOT) + "/loadavg" );
	ifstream loadavgfile ( filename.c_str() );

	//Try to open the file
	if ( loadavgfile.is_open() ) {

		//Read in the avg times
		loadavgfile >> loadavg.one_min;
		loadavgfile >> loadavg.five_mins;
		loadavgfile >> loadavg.fifteen_mins;

		//Close the file
		loadavgfile.close();

		return loadavg;

	} else {

		cerr << "Error opening /proc/loadavg" << endl;

	}

	return LoadAverage();
}


MemoryInfo get_memory_info() {

	//Variables
	MemoryInfo meminfo = MemoryInfo();
	string filename = string( string ( PROC_ROOT ) + "/meminfo" );
	ifstream mifile ( filename.c_str() );
	string name, kb;
	unsigned long long amount;

	//Read from the file.
	if ( mifile.is_open() ) {

		while ( !mifile.eof() ) {

			mifile >> name;
			mifile >> amount;
			mifile >> kb;

			if ( name == "MemTotal:" ) {
				meminfo.total_memory = amount;
			}

			if ( name == "MemFree:" ) {
				meminfo.free_memory = amount;
			}

			if ( name == "Buffers:" ) {
				meminfo.buffers_memory = amount;
			}

			if ( name == "Cached:" ) {
				meminfo.cached_memory = amount;
			}

			if ( name == "SwapTotal:" ) {
				meminfo.total_swap = amount;
			}

			if ( name == "SwapFree:" ) {
				meminfo.free_swap = amount;
				return meminfo;
			}

		}

	} else {

		cerr << "Error opening /proc/meminfo" << endl;

	}

  return MemoryInfo();
}


vector<Cpu> get_cpu_info() {

	//Variables
	vector<Cpu> cpus = vector<Cpu>();
	Cpu temp;
	string filename = string ( string ( PROC_ROOT ) + "/stat" );
	ifstream statfile ( filename.c_str() );
	string name;
	unsigned long long user_time, nice_time, system_time, idle_time,
	 io_wait_time, irq_time, softirq_time, steal_time, guest_time, guest_nice_time;

	//Read from file;
	if ( statfile.is_open() ) {

		temp = Cpu();

		while ( !statfile.eof() ) {

			statfile >> name;

			if ( name == "intr" ) {
				return cpus;
			}

			statfile >> user_time;
			statfile >> nice_time;
			statfile >> system_time;
			statfile >> idle_time;
			statfile >> io_wait_time;
			statfile >> irq_time;
			statfile >> softirq_time;
			statfile >> steal_time;
			statfile >> guest_time;
			statfile >> guest_nice_time;

			temp.user_time = user_time;
			temp.nice_time = nice_time;
			temp.system_time = system_time;
			temp.idle_time = idle_time;
			temp.io_wait_time = io_wait_time;
			temp.irq_time = irq_time;
			temp.softirq_time = softirq_time;
			temp.steal_time = steal_time;
			temp.guest_time = guest_time;
			temp.guest_nice_time = guest_nice_time;

			cpus.push_back(temp);

		}

	} else {

		cerr << "Error opening /proc/stat" << endl;

	}


  	return vector<Cpu>();
}


Process get_process(int pid, const char* basedir) {

	Process process = Process();
	string filename = string ( string ( PROC_ROOT ) + string ( pid ) + "/stat";
	ifstream file ( filename.c_str() );
	

  	return Process();
}


vector<Process> get_all_processes(const char* basedir) {
  return vector<Process>();
}


SystemInfo get_system_info() {
  return SystemInfo();
}
