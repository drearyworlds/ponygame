#pragma once

#include <nowarn/windows.h>
#include <nowarn/stdint.h>
#include <nowarn/exception>
#include <nowarn/functional>

namespace DX {
    // Helper class for animation and simulation timing.
    class StepTimer {
    private:
        // Source timing data uses QPC units.
        LARGE_INTEGER _QpcFrequency;
        LARGE_INTEGER _QpcLastTime;
        uint64_t _QpcMaxDelta;

        // Derived timing data uses a canonical tick format.
        uint64_t _ElapsedTicks;
        uint64_t _TotalTicks;
        uint64_t _LeftOverTicks;

        // Members for tracking the framerate.
        uint32_t _FrameCount;
        uint32_t _FramesPerSecond;
        uint32_t _FramesThisSecond;
        uint64_t _QpcSecondCounter;

        // Members for configuring fixed timestep mode.
        bool _FixedTimeStep;
        uint64_t _TargetElapsedTicks;

    public:
        StepTimer() noexcept(false);

        // Get elapsed time since the previous Update call.
        uint64_t GetElapsedTicks() const;
        double GetElapsedSeconds() const;

        // Get total time since the start of the program.
        uint64_t GetTotalTicks() const;
        double GetTotalSeconds() const;

        // Get total number of updates since start of the program.
        uint32_t GetFrameCount() const;

        // Get the current framerate.
        uint32_t GetFramesPerSecond() const;

        // Set whether to use fixed or variable timestep mode.
        void SetFixedTimeStep(bool isFixedTimestep);

        // Set how often to call Update when in fixed timestep mode.
        void SetTargetElapsedTicks(uint64_t targetElapsed);
        void SetTargetElapsedSeconds(double targetElapsed);

        static double TicksToSeconds(uint64_t ticks);
        static uint64_t SecondsToTicks(double seconds);

        // After an intentional timing discontinuity (for instance a blocking IO operation)
        // call this to avoid having the fixed timestep logic attempt a set of catch-up 
        // Update calls.
        void ResetElapsedTime();

        // Update timer state, calling the specified Update function the appropriate number of times.
        void Tick(std::function<void()> update);
    };
}