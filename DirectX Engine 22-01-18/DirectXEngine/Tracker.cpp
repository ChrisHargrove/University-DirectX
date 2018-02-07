#include "Tracker.h"

/*******************************************************************************************************************
	Function that initializes all the trackers default settings
*******************************************************************************************************************/
bool Tracker::Initialize()
{
	//-------------------------------------------- Initialize the timer tracker for the in-game delta time

	//-------------------------------------------- Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0) { return false; }

	//-------------------------------------------- Find out how many times the frequency counter ticks every millisecond
	m_ticksPerMs = (float)(m_frequency / 1000);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	//-------------------------------------------- Initialize the frames per second tracker, for outputting to screen using 2D font rendering
	m_startFps = timeGetTime();

	//-------------------------------------------- Initialize the CPU tracker, which allows us to query the usage of CPU
	PDH_STATUS status;

	//-------------------------------------------- Create a query object to poll CPU usage
	status = PdhOpenQuery(nullptr, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS) { m_canReadCpu = false; }

	//-------------------------------------------- Set query object to poll all CPU's in the system
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS) { m_canReadCpu = false; }

	return true;
}


/*******************************************************************************************************************
	Function that updates all of the trackers every frame
*******************************************************************************************************************/
void Tracker::Update()
{
	//-------------------------------------------- Update delta time
	INT64 currentTime = 0;
	float timeDifference = 0;
	
	//--------------------------------------------  Query the current time
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	//--------------------------------------------  Calculate the difference in time since the last time we queried for the current time
	timeDifference = (float)(currentTime - m_startTime);

	//--------------------------------------------  Calculate the frame time by the time difference over the timer speed resolution
	m_frameTime = timeDifference / m_ticksPerMs;

	//--------------------------------------------  Restart the timer
	m_startTime = currentTime;

	//-------------------------------------------- Update frames per second (FPS)
	m_count++;

	//-------------------------------------------- If one second has passed then update the frame per second speed
	if (timeGetTime() >= (m_startFps + 1000))
	{
		m_framesPerSec = m_count;
		m_count = 0;

		m_startFps = timeGetTime();
	}

	//-------------------------------------------- Update CPU usage
	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCpu)
	{
		//-------------------------------------------- If it has been 1 second then update the current CPU usage and reset the 1 second timer again
		if ((m_lastSampleTime + 1000) < GetTickCount())
		{
			m_lastSampleTime = GetTickCount();

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
		}
	}

}


/*******************************************************************************************************************
	Function that shuts down all necessary tracker procedures
*******************************************************************************************************************/
void Tracker::Shutdown()
{
	if (m_canReadCpu) { PdhCloseQuery(m_queryHandle); m_queryHandle = nullptr; }
}


/*******************************************************************************************************************
	Accessor methods
*******************************************************************************************************************/
float Tracker::GetTime()	{ return m_frameTime; }
int Tracker::GetFps()		{ return m_framesPerSec; }

int Tracker::GetCpuPercentage()
{
	int usage = 0;

	//-------------------------------------------- If we can read the CPU from the operating system then return the current usage.  If not then return zero.
	if (m_canReadCpu)	{ usage = (int)m_cpuUsage; }
	else				{ usage = 0; }

	return usage;
}


/*******************************************************************************************************************
	Static variables initialization
*******************************************************************************************************************/
INT64 Tracker::m_frequency					= 0;
float Tracker::m_ticksPerMs					= 0;
INT64 Tracker::m_startTime					= 0;
float Tracker::m_frameTime					= 0;

int Tracker::m_framesPerSec					= 0;
int Tracker::m_count						= 0;
unsigned long Tracker::m_startFps			= 0;

bool Tracker::m_canReadCpu					= true;
HQUERY Tracker::m_queryHandle				= nullptr;
HCOUNTER Tracker::m_counterHandle			= nullptr;
unsigned long Tracker::m_lastSampleTime		= GetTickCount();
long Tracker::m_cpuUsage					= 0;