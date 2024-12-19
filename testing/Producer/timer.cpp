#include "pch.h"
#include <math.h>
#include "timer.h"

PerformanceTimer timer;

PerformanceTimer::PerformanceTimer() {
	QueryPerformanceFrequency(&counterFrequency);	
}

PerformanceTimer::~PerformanceTimer() {
}

void PerformanceTimer::start() {
	QueryPerformanceCounter(&t0); 
}

void PerformanceTimer::stop() {
	QueryPerformanceCounter(&t); 
}

int PerformanceTimer::getElapsedTime() {//[ms]
	return((int)floor(0.5+(t.QuadPart-t0.QuadPart)*1000.0/counterFrequency.QuadPart));
}

void PerformanceTimer::wait(int ms) {//[ms]
	LARGE_INTEGER t0, t;					
	QueryPerformanceCounter(&t0);
	while(true) {
		QueryPerformanceCounter(&t);
		if ((t.QuadPart-t0.QuadPart)*1000.0/counterFrequency.QuadPart>ms)
			break;
	}
}
