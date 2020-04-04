#pragma once

#include <iostream>
#include <atomic>
#include <string>
#include <sstream>

class PerformanceTracker
{
public:
    static PerformanceTracker& instance();
    void incrementItems();
    void incrementPolygons();
    void incrementPrimaryRays();
    void incrementCallToIntersect();
    void incrementCallToIntersectPolygon();
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
    std::atomic<int> nCallToIntersect_{0};
    std::atomic<int> nCallToIntersectPolygon_{0};
};
