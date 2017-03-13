#include "Timer.hpp"

FishEngine::Timer::Timer(std::string label) : m_label(label)
{
	Start();
}

void FishEngine::Timer::Start()
{
	m_startTime = std::chrono::high_resolution_clock::now();
	//m_hasStarted = true;
}

void FishEngine::Timer::Stop()
{
	//if (!m_hasStarted)
	//{
	//    Debug::LogWarning("timer %s not started.", m_label);
	//}
	m_stopTime = std::chrono::high_resolution_clock::now();
	//_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - _start).count();
}

void FishEngine::Timer::Print() const
{
	//info("Time info for [%s]: ", _label.c_str());
	auto elapse = m_stopTime - m_startTime;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapse).count();
	if (ms >= 1000)
		Debug::Log("Time info for [%s]: %f s", m_label.c_str(), float(ms) / 1000.0f);
	else
		Debug::Log("Time info for [%s]: %lld ms", m_label.c_str(), ms);
}

void FishEngine::Timer::StopAndPrint()
{
	Stop();
	Print();
}

