//============================================================================
// Name        : rdtscp_and_syscall_overhead.cpp
// Author      : Tõnis Lusmägi
// Version     :
// Copyright   : C 2019
// Description : Measure RDTSCP and syscall (CPUID) overhead
//============================================================================

#include <iostream>
#include "immintrin.h"
#include <cpuid.h>
#include <cstring>

using namespace std;

#define REPEATS 200000

inline __attribute__((always_inline))  uint64_t rdtscp(void)
{
        unsigned int low, high;

        asm volatile("rdtscp" : "=a" (low), "=d" (high));

        return low | ((uint64_t)high) << 32;
}

void cpu_info() {
	uint64_t start_cycle,end_cycle = 0;
	uint64_t overhead[REPEATS]= {0};
	uint64_t sum = 0, avg;

	char CPUBrandString[0x40];
	unsigned int CPUInfo[4] = {0,0,0,0};

	for (uint64_t i = 0; i < REPEATS; i++) {
		start_cycle = rdtscp();
		__cpuid(0x80000000, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
		end_cycle = rdtscp();
		overhead[i] = end_cycle-start_cycle;
		sum += overhead[i];
    }

	avg = sum / REPEATS;

	unsigned int nExIds = CPUInfo[0];
	memset(CPUBrandString, 0, sizeof(CPUBrandString));

	for (unsigned int i = 0x80000000; i <= nExIds; ++i) {
	    __cpuid(i, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);

	    if (i == 0x80000002)
	        memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
	    else if (i == 0x80000003)
	        memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
	    else if (i == 0x80000004)
	        memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}

	cout << "CPUID: syscall average overhead cycles for 200000 measurements: " << (int)avg << endl;
	cout << "CPUID: " << CPUBrandString << endl;
}

void rdtscp_overhead() {
	uint64_t start_cycle,end_cycle = 0;
	uint64_t overhead[REPEATS]= {0};
	uint64_t sum = 0, avg;

	for (uint64_t i = 0; i < REPEATS; i++) {
		start_cycle = rdtscp();
		end_cycle = rdtscp();
		overhead[i] = end_cycle-start_cycle;
		sum += overhead[i];
    }

	avg = sum / REPEATS;
	cout << "RDTSCP: average overhead cycles for 200000 measurements: " << (int)avg << endl;
}

int main() {

	rdtscp_overhead();
	cpu_info();

	return 0;
}
