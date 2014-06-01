#ifndef TIMEMES_H
#define TIMEMES_H

#include "timeMes.h"
#include <Windows.h>
double startTimer();
double endTimer();
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

#endif // TIMEMES_H
