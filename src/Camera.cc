#include "Camera.hh"
#include "utils.hh"

Camera::Camera()
  : frame_(),
    fov_(0),
    resX_(0),
    target_(0, 0, 0),
    upGuide_(0, 0, 1),
    aspectRatio_(1),
    allPixels_(),
    focalDist_(0),
    screenWidth_(0),
    screenHeight_(0)
{
}

Camera::Camera(const CamData& cData)
  : frame_(),
    resX_(cData.resX),
    aspectRatio_(cData.aspectRatio),
    fov_(cData.fov),
    target_(cData.target),
    upGuide_(cData.upGuide)
{
    frame_.o_ = cData.pos;
    upGuide_.normalize();
    rotateToTarget();

    screenWidth_ = 2*std::atan(fov_);
    screenHeight_ = screenWidth_/aspectRatio_;
    resY_ = int(resX_/aspectRatio_);

    initAllPixelsVec();
}

void Camera::rotateToTarget()
{
    frame_.vy_ = target_ - frame_.o_;
    frame_.vy_.normalize();
    frame_.vx_ = frame_.vy_.cross(upGuide_);
    if(frame_.vx_.squaredNorm() <= 0.9) // equivalent to if(vx and upGuide_ are parallel)
    {
        upGuide_ = Vector3(0, 0.001, 1.0);
        upGuide_.normalize();
        frame_.vx_ = frame_.vy_.cross(upGuide_);
    }
    frame_.vx_.normalize();
    frame_.vz_ = frame_.vx_.cross(frame_.vy_);
}

void Camera::moveToDistanceToTarget(double d)
{
    Vector3 tp = frame_.o_ - target_;
    frame_.o_ = tp * (d/tp.norm()) + target_;
}

Camera::~Camera()
{
}

void Camera::initAllPixelsVec()
{
    allPixels_.resize(nPixels());
    for (int i = 0; i < resX_; ++i)
    {
        for (int j = 0; j < resY_; ++j)
        {
            allPixels_[i*resY_ + j] = std::make_pair<int, int>(int(i), int(j));
        }
    }
    std::random_shuffle(allPixels_.begin(), allPixels_.end());
}

Vector3 Camera::pixelToDir(const Pixel& px) const
{
    Vector3 dir;
    dir.x_ = - screenWidth_ / 2 + screenWidth_ * (px.x_+0.5) / resX_;
    dir.z_ = - screenHeight_ / 2 + screenHeight_ * (px.y_+0.5) / resY_;
    dir.y_ = 1.0;

    dir.normalize();
    return dir;
}

void Camera::castRandomRay(LightRay& lr, Pixel& px) const
{
    int randX = std::rand();
    int randY = std::rand();
    px.x_ = randX % resX_;
    px.y_ = randY % resY_;

    lr.origin_ = frame_.pointToWorld(frame_.o_);
    lr.dir_ = frame_.vecToWorld(pixelToDir(px));
}

void Camera::castRayAt(const int& pX, const int& pY, LightRay& lr, Pixel& px) const
{
    px.x_ = pX;
    px.y_ = pY;

    lr.origin_ = frame_.o_;
    lr.dir_ = frame_.vecToWorld(pixelToDir(px));
}


void Camera::castOrderedRay(LightRay& lr, Pixel& px, size_t index) const
{
    if (index >= allPixels_.size())
    {
        std::cout << "Error, requesting index " << index << ", which out of allPixels(size: " << allPixels_.size() << ")" << std::endl;
        return;
    }
    px.x_ = allPixels_[index].first;
    px.y_ = allPixels_[index].second;

    lr.origin_ = frame_.o_;
    lr.dir_ = frame_.vecToWorld(pixelToDir(px));
}

std::string Camera::describe() const
{
    std::stringstream ss;
    ss << "=== Camera ===\n";
    ss << "frame: " << frame_ << "\n";
    ss << "fov: " << fov_ << "\n";
    ss << "target: " << target_ << "\n";
    ss << "upGuide: " << upGuide_ << "\n";
    ss << "aspectRatio: " << aspectRatio_ << "\n";
    ss << "screenWidth: " << screenWidth_ << "\n";
    ss << "screenHeight: " << screenHeight_ << "\n";
    ss << "resX: " << resX_ << "\n";
    ss << "resY: " << resY_ << "\n";
    return ss.str();
}

std::string CamData::describe() const
{
    std::stringstream ss;
    ss << "=== CamData ===\n";
    ss << "fov: " << fov << "\n";
    ss << "target: " << target << "\n";
    ss << "upGuide: " << upGuide << "\n";
    ss << "aspectRatio: " << aspectRatio << "\n";
    ss << "pos: " << pos << "\n";
    ss << "rotAxis: " << rotAxis << "\n";
    ss << "rotAngle: " << rotAngle << "\n";
    ss << "resX: " << resX << "\n";
    return ss.str();
}

void Camera::changeResolution(int resX, int resY)
{
    resX_ = resX;
    resY_ = resY;
    initAllPixelsVec();
}

std::ostream& operator<<(std::ostream& os, const Camera& c)
{
    os << c.describe();
    return os;
}
