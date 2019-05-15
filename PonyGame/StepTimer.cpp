#include "StepTimer.h"
#include "Constants.h"

using namespace DX;

// Helper class for animation and simulation timing.
StepTimer::StepTimer() noexcept(false) :
    _ElapsedTicks(0),
    _TotalTicks(0),
    _LeftoverTicks(0),
    _FrameCount(0),
    _FramesPerSecond(0),
    _FramesThisSecond(0),
    _QpcSecondCounter(0),
    _FixedTimeStep(false),
    _TargetElapsedTicks(TICKS_PER_SECOND / SECONDS_PER_MINUTE) {
    if (!QueryPerformanceFrequency(&_QpcFrequency)) {
        throw std::exception("QueryPerformanceFrequency");
    }

    if (!QueryPerformanceCounter(&_QpcLastTime)) {
        throw std::exception("QueryPerformanceCounter");
    }

    // Initialize max delta to 1/10 of a second.
    _QpcMaxDelta = static_cast<uint64_t>(_QpcFrequency.QuadPart / 10);
}

// Get elapsed time since the previous Update call.
uint64_t StepTimer::GetElapsedTicks() const {
    return _ElapsedTicks;
}

double StepTimer::GetElapsedSeconds() const {
    return TicksToSeconds(_ElapsedTicks);
}

// Get total time since the start of the program.
uint64_t StepTimer::GetTotalTicks() const {
    return _TotalTicks;
}

double StepTimer::GetTotalSeconds() const {
    return TicksToSeconds(_TotalTicks);
}

// Get total number of updates since start of the program.
uint32_t StepTimer::GetFrameCount() const { return _FrameCount; }

// Get the current framerate.
uint32_t StepTimer::GetFramesPerSecond() const { return _FramesPerSecond; }

// Set whether to use fixed or variable timestep mode.
void StepTimer::SetFixedTimeStep(bool isFixedTimestep) { _FixedTimeStep = isFixedTimestep; }

// Set how often to call Update when in fixed timestep mode.
void StepTimer::SetTargetElapsedTicks(uint64_t targetElapsed) {
    _TargetElapsedTicks = targetElapsed;
}

void StepTimer::SetTargetElapsedSeconds(double targetElapsed) {
    _TargetElapsedTicks = SecondsToTicks(targetElapsed);
}

double StepTimer::TicksToSeconds(uint64_t ticks) {
    return static_cast<double>(ticks) / TICKS_PER_SECOND;
}

uint64_t StepTimer::SecondsToTicks(double seconds) {
    return static_cast<uint64_t>(seconds * TICKS_PER_SECOND);
}

// After an intentional timing discontinuity (for instance a blocking IO operation)
// call this to avoid having the fixed timestep logic attempt a set of catch-up 
// Update calls.

void StepTimer::ResetElapsedTime() {
    if (!QueryPerformanceCounter(&_QpcLastTime)) {
        throw std::exception("QueryPerformanceCounter");
    }

    _LeftoverTicks = 0;
    _FramesPerSecond = 0;
    _FramesThisSecond = 0;
    _QpcSecondCounter = 0;
}

void StepTimer::Tick(const std::function <void(const double& elapsedSecs)>& update) {
    // Query the current time.
    LARGE_INTEGER currentTime;

    if (!QueryPerformanceCounter(&currentTime)) {
        throw std::exception("QueryPerformanceCounter");
    }

    uint64_t timeDelta = static_cast<uint64_t>(currentTime.QuadPart - _QpcLastTime.QuadPart);

    _QpcLastTime = currentTime;
    _QpcSecondCounter += timeDelta;

    // Clamp excessively large time deltas (e.g. after paused in the debugger).
    if (timeDelta > _QpcMaxDelta) {
        timeDelta = _QpcMaxDelta;
    }

    // Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp.
    timeDelta *= TICKS_PER_SECOND;
    timeDelta /= _QpcFrequency.QuadPart;

    uint32_t lastFrameCount = _FrameCount;

    if (_FixedTimeStep) {
        // Fixed timestep update logic

        // If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp
        // the clock to exactly match the target value. This prevents tiny and irrelevant errors
        // from accumulating over time. Without this clamping, a game that requested a 60 fps
        // fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
        // accumulate enough tiny errors that it would drop a frame. It is better to just round 
        // small deviations down to zero to leave things running smoothly.
        if (static_cast<uint64_t>(std::abs(static_cast<int64_t>(timeDelta - _TargetElapsedTicks))) < TICKS_PER_SECOND / 4000) {
            timeDelta = _TargetElapsedTicks;
        }

        _LeftoverTicks += timeDelta;

        while (_LeftoverTicks >= _TargetElapsedTicks) {
            _ElapsedTicks = _TargetElapsedTicks;
            _TotalTicks += _TargetElapsedTicks;
            _LeftoverTicks -= _TargetElapsedTicks;
            _FrameCount++;

            update(GetElapsedSeconds());
        }
    } else {
        // Variable timestep update logic.
        _ElapsedTicks = timeDelta;
        _TotalTicks += timeDelta;
        _LeftoverTicks = 0;
        _FrameCount++;

        update(GetElapsedSeconds());
    }

    // Track the current framerate.
    if (_FrameCount != lastFrameCount) {
        _FramesThisSecond++;
    }

    if (_QpcSecondCounter >= static_cast<uint64_t>(_QpcFrequency.QuadPart)) {
        _FramesPerSecond = _FramesThisSecond;
        _FramesThisSecond = 0;
        _QpcSecondCounter %= _QpcFrequency.QuadPart;
    }
}