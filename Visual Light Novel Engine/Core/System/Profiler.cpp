#include "../../stdafx.h"
#include "Profiler.h"


Profiler::Profiler()
{
	startTime = timeGetTime();

	// FPS
	fps = 0;
	count = 0;

	// CPU
	PDH_STATUS status;
	canReadCPU = true;
	status = PdhOpenQuery(NULL, 0, &queryHandle);
	if (status != ERROR_SUCCESS)
		canReadCPU = false;
	status = PdhAddCounter(queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &counterHandle);
	if (status != ERROR_SUCCESS)
		canReadCPU = false;
	cpuUsage = 0;

	// MEMORY
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
}


Profiler::~Profiler()
{
	if (canReadCPU)
		PdhCloseQuery(queryHandle);
}

void Profiler::Update()
{
	count++;

	deltaTime = (timeGetTime() - prevTime) / 1000.0f;
	prevTime = timeGetTime();

	if (timeGetTime() >= startTime + 1000)
	{
		fps = count;
		count = 0;

		if (canReadCPU)
		{
			PDH_FMT_COUNTERVALUE value;
			PdhCollectQueryData(queryHandle);
			PdhGetFormattedCounterValue(counterHandle, PDH_FMT_LONG, NULL, &value);
			cpuUsage = value.longValue;
		}

		GlobalMemoryStatusEx(&memInfo);
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
		memoryUsage = pmc.WorkingSetSize / 1024 / 1024;

		startTime = timeGetTime();
	}	
}
