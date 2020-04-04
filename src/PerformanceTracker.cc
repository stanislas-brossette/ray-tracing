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

void PerformanceTracker::incrementPolygons()
{
    nPolygons_++;
}

void PerformanceTracker::incrementPrimaryRays()
{
    nPrimaryRays_++;
}

void PerformanceTracker::incrementCallToIntersect()
{
    nCallToIntersect_++;
}

void PerformanceTracker::incrementCallToIntersectPolygon()
{
    nCallToIntersectPolygon_++;
}

void PerformanceTracker::startRenderTimer()
{
    beginRender_ = std::chrono::steady_clock::now();
}

void PerformanceTracker::endRenderTimer()
{
    endRender_ = std::chrono::steady_clock::now();
}

void PerformanceTracker::resetAll()
{
    nItems_ = 0;
    nPolygons_ = 0;
    resetRays();
}

void PerformanceTracker::resetRays()
{
    nPrimaryRays_ = 0;
    nCallToIntersect_ = 0;
    nCallToIntersectPolygon_ = 0;
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
    ss << "nPolygons: " << nPolygons_ << std::endl;
    ss << "nPrimaryRays_: " << nPrimaryRays_ << std::endl;
    ss << "nCallToIntersect_: " << nCallToIntersect_ << std::endl;
    ss << "nCallToIntersectPolygon_: " << nCallToIntersectPolygon_ << std::endl;
    ss << "totalRenderTime_: " << std::chrono::duration_cast<std::chrono::milliseconds>(endRender_ - beginRender_).count() << "[ms]" << std::endl;
    ss << "========================" << std::endl;
    return ss.str();
}

