#ifndef TIMEMES_H
#define TIMEMES_H

#include "timeMes.h"
#include <Windows.h>
double startTimer();
double endTimer();
double frequency();
double startTimer()
{
    LARGE_INTEGER start;
    DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
    QueryPerformanceCounter(&start);
    SetThreadAffinityMask(GetCurrentThread(), oldmask);
    return start.QuadPart;
}
double endTimer()
{
    LARGE_INTEGER stop;
    DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
    QueryPerformanceCounter(&stop);
    SetThreadAffinityMask(GetCurrentThread(), oldmask);
    return stop.QuadPart;
}
double frequency()
{
	LARGE_INTEGER freq;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceFrequency(&freq);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return freq.QuadPart;
}

#endif // TIMEMES_H
