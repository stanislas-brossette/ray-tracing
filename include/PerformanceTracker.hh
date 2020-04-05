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
    void startRenderTimer();
    void endRenderTimer();
    void resetAll();
    void resetRays();
    std::string describe();

private:
    static PerformanceTracker instance_;
    PerformanceTracker ();
    ~PerformanceTracker ();
    std::atomic<int> nItems_{0};
    std::atomic<int> nPolygons_{0};
    std::atomic<int> nPrimaryRays_{0};
    std::atomic<int> nRays_{0};
    std::atomic<int> nCallToIntersect_{0};
    std::atomic<int> nCallToIntersectPolygon_{0};
    std::atomic<int> nIntersections_{0};
    std::chrono::steady_clock::time_point beginRender_, endRender_;
};
