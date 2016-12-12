#pragma once

#include <chrono>
#include "Debug.hpp"

namespace FishEngine
{
    class Timer
    {
    public:
        Timer(std::string label);

        void SetLabel(std::string label)
        {
            m_label = label;
        }

        //void Reset()
        //{
        //    m_hasStarted = false;
        //}

        void Start();
        void Stop();
        void Print() const;
        void StopAndPrint();

        //friend std::ostream& operator<<(std::ostream& os, const Timer& timer) {
        //	log("Time info for [%s]:\n", timer.label);
        //	os << "Time info for [" << label << "]:"
        //}

    private:
        typedef std::chrono::high_resolution_clock::time_point time_point;
        //typedef std::chrono::time_point<std::chrono::steady_clock> time_point;
        time_point m_startTime;
        time_point m_stopTime;
        std::string m_label;
        //bool m_hasStarted = false;
    };
}