#include "PerformanceTracker.hh"

PerformanceTracker PerformanceTracker::instance_;

PerformanceTracker::PerformanceTracker()
{
}

PerformanceTracker::~PerformanceTracker()
{
}

void PerformanceTracker::incrementItems()
{
    nItems_++;
}

void PerformanceTracker::incrementPolygons()
{
    nPolygons_++;
}

void PerformanceTracker::incrementRays()
{
    nRays_++;
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

void PerformanceTracker::incrementIntersections()
{
    nIntersections_++;
}

void PerformanceTracker::startRenderTimer()
{
    beginRender_ = std::chrono::steady_clock::now();
}

void PerformanceTracker::endRenderTimer()
{
    endRender_ = std::chrono::steady_clock::now();
}

void PerformanceTracker::startPreprocessingTimer()
{
    beginPreprocessing_ = std::chrono::steady_clock::now();
}

void PerformanceTracker::endPreprocessingTimer()
{
    endPreprocessing_ = std::chrono::steady_clock::now();
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
    nIntersections_ = 0;
}

PerformanceTracker& PerformanceTracker::instance()
{
    return instance_;
}

std::string PerformanceTracker::describe()
{
    std::stringstream ss;
    ss << "=============== Performances ===============" << std::endl;
    ss << "|nItems:                   " << std::right << std::setw(16) << nItems_ << "|"<< std::endl;
    ss << "|nPolygons:                " << std::right << std::setw(16) << nPolygons_ << "|" << std::endl;
    ss << "|nPrimaryRays_:            " << std::right << std::setw(16) << nPrimaryRays_ << "|" << std::endl;
    ss << "|nRays_:                   " << std::right << std::setw(16) << nRays_ << "|" << std::endl;
    ss << "|nCallToIntersectPolygon_: " << std::right << std::setw(16) << nCallToIntersectPolygon_ << "|" << std::endl;
    ss << "|nCallToIntersect_:        " << std::right << std::setw(16) << nCallToIntersect_ << "|" << std::endl;
    ss << "|nIntersections_:          " << std::right << std::setw(16) << nIntersections_ << "|" << std::endl;
    ss << "|preprocessingTime_:       " << std::right << std::setw(12) << std::chrono::duration_cast<std::chrono::milliseconds>(endPreprocessing_ - beginPreprocessing_).count() << "[ms]|" << std::endl;
    ss << "|totalRenderTime_:         " << std::right << std::setw(12) << std::chrono::duration_cast<std::chrono::milliseconds>(endRender_ - beginRender_).count() << "[ms]|" << std::endl;
    ss << "============================================" << std::endl;
    return ss.str();
}

