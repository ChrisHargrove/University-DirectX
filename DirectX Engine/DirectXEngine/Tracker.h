#pragma once

/*******************************************************************************************************************
	Tracker.h, Tracker.cpp
	Created by Kim Kane
	Last updated: 27/01/2018

	Static class that keeps track of the delta time, FPS and CPU usage of users PC.

*******************************************************************************************************************/
#pragma comment(lib, "pdh.lib")
#pragma comment(lib, "winmm.lib")

#include <pdh.h>
#include <Windows.h>
#include <mmsystem.h>

class Tracker {

public:
	static bool Initialize();
	static void Update();
	static void Shutdown();

public:
	static float GetTime();
	static int GetFps();
	static int GetCpuPercentage();

private:
	Tracker();

private:
	static INT64			m_frequency;
	static float			m_ticksPerMs;
	static INT64			m_startTime;
	static float			m_frameTime;

	static int				m_framesPerSec;
	static int				m_count;
	static unsigned long	m_startFps;

	static bool				m_canReadCpu;
	static HQUERY			m_queryHandle;
	static HCOUNTER			m_counterHandle;
	static unsigned long	m_lastSampleTime;
	static long				m_cpuUsage;
};