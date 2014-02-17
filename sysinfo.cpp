#include "sysinfo.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>

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
	string filename = string ( string ( basedir ) + "/" + convertInt(pid) + "/stat" );
	ifstream file ( filename.c_str() );
	long size, resident, share, trs, lrs, drs, dt;
	int pidt, tgid;
	string comm;
	string command_line;
	char state;
	int ppid, pgrp, session, tty_nr, tpgid;
	unsigned flags;
	unsigned long minflt, cminflt, majflt, cmajflt, utime, stime;
	long cutime, cstime, priority, nice, num_threads, itrealvalue;
	unsigned long long starttime;
	unsigned long vsize;
	long rss;
	unsigned long rsslim, startcode, endcode, startstack, kstkesp, 
		kstkeip, signalt, blocked, sigignore, sigcatch, wchan, nswap, cnswap;
	int exit_signal, processr;
	unsigned rt_priority, policy;
	unsigned long long delayacct_blkio_ticks;
	unsigned long guest_time;
	long cguest_time;
	vector<Process> threads;

	string junk;

	// Read from stat file
	if ( file.is_open() ) {

		file >> pidt;
		process.pid = pidt;

		file >> comm;
		strcpy(process.comm, comm.c_str());

		file >> state;
		process.state = state;

		file >> ppid;
		process.ppid = ppid;

		file >> pgrp;
		process.pgrp = pgrp;

		file >> session;
		process.session = session;

		file >> tty_nr;
		process.tty_nr = tty_nr;

		file >> tpgid;
		process.tpgid = tpgid;

		file >> flags;
		process.flags = flags;

		file >> minflt;
		process.minflt = minflt;

		file >> cminflt;
		process.cminflt = cminflt;

		file >> majflt;
		process.majflt = majflt;

		file >> cmajflt;
		process.cmajflt = cmajflt;

		file >> utime;
		process.utime = utime;

		file >> stime;
		process.stime = stime;

		file >> cutime;
		process.cutime = cutime;

		file >> cstime;
		process.cstime = cstime;

		file >> priority;
		process.priority = priority;

		file >> nice;
		process.nice = nice;

		file >> num_threads;
		process.num_threads = num_threads;

		file >> itrealvalue;
		process.itrealvalue = itrealvalue;

		file >> starttime;
		process.starttime = starttime;

		file >> vsize;
		process.vsize = vsize;

		file >> rss;
		process.rss = rss;

		file >> rsslim;
		process.rsslim = rsslim;

		file >> startcode;
		process.startcode = startcode;

		file >> endcode;
		process.endcode = endcode;

		file >> startstack;
		process.startstack = startstack;

		file >> kstkesp;
		process.kstkesp = kstkesp;

		file >> kstkeip;
		process. kstkeip = kstkeip;

		file >> signalt;
		process.signal = signalt;

		file >> blocked;
		process.blocked = blocked;

		file >> sigignore;
		process.sigignore = sigignore;

		file >> sigcatch;
		process.sigcatch = sigcatch;

		file >> wchan;
		process.wchan = wchan;

		file >> nswap;
		process.nswap = nswap;

		file >> cnswap;
		process.cnswap = cnswap;

		file >> exit_signal;
		process.exit_signal = exit_signal;

		file >> processr;
		process.processor = processr;

		file >> rt_priority;
		process.rt_priority = rt_priority;

		file >> policy;
		process.policy = policy;

		file >> delayacct_blkio_ticks;
		process.delayacct_blkio_ticks = delayacct_blkio_ticks;

		file >> guest_time;
		process.guest_time >> guest_time;

		file >> cguest_time;
		process.cguest_time = cguest_time;

		file.close();

	} else {

		cerr << "Error opening stat for pid: " << pid  << endl;

	}

	filename = string ( string ( basedir ) + "/" + convertInt(pid) + "/statm" );
	ifstream filem ( filename.c_str() );

	if ( filem.is_open() ) {

		filem >> process.size;
		filem >> process.resident;
		filem >> process.share;
		filem >> process.trs;
		filem >> process.lrs;
		filem >> process.drs;
		filem >> process.dt;

		filem.close();

	} else {

		cerr << "Error opening statm for pid: " << pid << endl;

	}

	filename = string ( string ( basedir ) + "/" + convertInt(pid) + "/status" );
	ifstream filestatus ( filename.c_str() );

	if ( filestatus.is_open() ) {

		for ( size_t i = 0; i < 6; i++ ) {

			filestatus >> junk;

		}

		filestatus >> process.tgid;

		filestatus.close();

	} else {

		cerr << "Error opening status for pid: " << pid << endl;

	}

	filename = string ( string ( basedir ) + "/" + convertInt(pid) + "/cmdline" );
	ifstream cmdline ( filename.c_str() );

	if ( cmdline.is_open() ) {

		cmdline >> command_line;

		int test = command_line.find("\0");

		command_line.replace(test, 2, " ");

		cout << command_line;

		process.command_line = command_line;

		cmdline.close();

	} else {

		cerr << "Error opening cmdline for pid: " << pid << endl;

	}



  	return process;
}


vector<Process> get_all_processes(const char* basedir) {

	vector<Process> processes = vector<Process>();
	unsigned long num_processes;

	string filename = string ( string (basedir)  + "/stat");
	ifstream file ( filename.c_str() );

	string temp;

	if ( file.is_open() ) {

		while ( !file.eof() ) {

			file >> temp;

			if ( temp == "processes" ) {
				file >> num_processes;
				break;
			}

		}

		file.close();

	} else {

		cerr << "Error opening /proc/stat" << endl;

	}

	for ( size_t i = 0; i < num_processes; i++ ) {

		processes.push_back(get_process(i, basedir));

	}


  	return processes;
}


SystemInfo get_system_info() {
  return SystemInfo();
}

string convertInt ( int number ) {
	stringstream ss;
	ss << number;
	return ss.str();
}
