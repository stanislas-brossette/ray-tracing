#pragma once

#include <iostream>
#include <atomic>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

class PerformanceTracker
{
public:
    static PerformanceTracker& instance();
    void incrementItems();
    void incrementPolygons();
    void incrementPrimaryRays();
    void incrementRays();
    void incrementCallToIntersect();
    void incrementCallToIntersectPolygon();
    void incrementIntersections();
    void startPreprocessingTimer();
    void endPreprocessingTimer();
    void startRenderTimer();
    void endRenderTimer();
    void resetAll();
    void resetRays();
    std::string describe();

private:
    static PerformanceTracker instance_;
    PerformanceTracker ();
    ~PerformanceTracker ();
    std::atomic<long> nItems_{0};
    std::atomic<long> nPolygons_{0};
    std::atomic<long> nPrimaryRays_{0};
    std::atomic<long> nRays_{0};
    std::atomic<long> nCallToIntersect_{0};
    std::atomic<long> nCallToIntersectPolygon_{0};
    std::atomic<long> nIntersections_{0};
    std::chrono::steady_clock::time_point beginRender_, endRender_;
    std::chrono::steady_clock::time_point beginPreprocessing_, endPreprocessing_;
};
