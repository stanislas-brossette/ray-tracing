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
    void incrementPrimaryRays();
    void incrementCallToIntersect();
    void resetAll();
    void resetRays();
    std::string describe();

private:
    static PerformanceTracker instance_;
    PerformanceTracker ();
    ~PerformanceTracker ();
    std::atomic<int> nItems_{0};
    std::atomic<int> nPrimaryRays_{0};
    std::atomic<int> nCallToIntersect_{0};
};
