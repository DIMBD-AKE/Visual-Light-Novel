#pragma once
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "pdh.lib")
#include <mmsystem.h>
#include <Pdh.h>
#include <Psapi.h>

class Profiler
{
	SINGLETON(Profiler)

public:
	void Update();

	int GetFPS() { return fps; }
	int GetCPUUsage() { return cpuUsage; }
	int GetMemoryUsage() { return memoryUsage; }
	float GetDeltaTime() { return deltaTime; }

private:
	ULONG startTime;
	ULONG prevTime;
	float deltaTime;

	// FPS
	int fps, count;

	// CPU
	bool canReadCPU;
	HQUERY queryHandle;
	HCOUNTER counterHandle;
	long cpuUsage;

	// Memory
	MEMORYSTATUSEX memInfo;
	int memoryUsage;
};

#define PROFILER Profiler::GetInstance()