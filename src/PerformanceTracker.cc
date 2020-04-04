#include "PerformanceTracker.hh"

PerformanceTracker PerformanceTracker::instance_;

PerformanceTracker::PerformanceTracker()
{
}

PerformanceTracker::~PerformanceTracker()
{
    std::cout << "dtor performanceTracker" << std::endl;
}

void PerformanceTracker::incrementItems()
{
    nItems_++;
}

void PerformanceTracker::incrementPrimaryRays()
{
    nPrimaryRays_++;
}

void PerformanceTracker::incrementCallToIntersect()
{
    nCallToIntersect_++;
}

void PerformanceTracker::resetAll()
{
    nItems_ = 0;
    resetRays();
}

void PerformanceTracker::resetRays()
{
    nPrimaryRays_ = 0;
}

PerformanceTracker& PerformanceTracker::instance()
{
    return instance_;
}

std::string PerformanceTracker::describe()
{
    std::stringstream ss;
    ss << "===== Performances =====" << std::endl;
    ss << "nItems: " << nItems_ << std::endl;
    ss << "nPrimaryRays_: " << nPrimaryRays_ << std::endl;
    ss << "nCallToIntersect_: " << nCallToIntersect_ << std::endl;
    ss << "========================" << std::endl;
    return ss.str();
}

