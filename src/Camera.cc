#include "Camera.hh"
#include "utils.hh"

Camera::Camera()
  : frame_(),
    fovX_(),
    fovY_(),
    resX_(),
    allPixels_()
{
    focalDist_ = 0;
    resY_ = 0;
}

Camera::Camera(const CamData& cData)
  : frame_(),
    fovX_(cData.fovX),
    fovY_(cData.fovY),
    resX_(cData.resX),
    fovType_(cData.fovType)
{
    frame_.translate(cData.pos);
    frame_.rotate(cData.rotAxis, cData.rotAngle);
    focalDist_ = double(resX_)/(2*std::tan(deg2rad(fovX_)/2));
    resY_ = int(focalDist_ * 2 * std::tan(deg2rad(fovY_)/2));
    initAllPixelsVec();
}

Camera::Camera(const Frame3& frame, double fovX, double fovY, int resX)
  : frame_(frame),
    fovX_(fovX),
    fovY_(fovY),
    resX_(resX)
{
    focalDist_ = double(resX_)/(2*std::tan(deg2rad(fovX_)/2));
    resY_ = int(focalDist_ * 2 * std::tan(deg2rad(fovY_)/2));
    initAllPixelsVec();
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

    double dx = double(px.x_) - double(resX_)/2;
    double dy = double(px.y_) - double(resY_)/2;

    if(fovType_ == 0)
    {
        dir.x_ = dx;
        dir.y_ = focalDist_;
        dir.z_ = dy;
        dir.normalize();
    }
    else if (fovType_ == 1)
    {
        double theta = dx*fovX_/resX_;
        double phi = dy*fovY_/resY_;

        dir.x_ =  focalDist_*std::sin(deg2rad(theta))*std::cos(deg2rad(phi));
        dir.y_ =  focalDist_*std::cos(deg2rad(theta))*std::cos(deg2rad(phi));
        dir.z_ =  focalDist_*std::sin(deg2rad(phi));
    }
    else if (fovType_ == 2)
    {
        double theta = dx*fovX_/resX_;
        double phi = dy*fovY_/resY_;

        dir.x_ =  focalDist_*std::sin(deg2rad(theta));
        dir.y_ =  focalDist_*std::cos(deg2rad(theta))*std::cos(deg2rad(phi));
        dir.z_ =  focalDist_*std::cos(deg2rad(theta))*std::sin(deg2rad(phi));
    }

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

void Camera::castOrderedRay(LightRay& lr, Pixel& px, size_t index) const
{
    if (index >= allPixels_.size())
    {
        std::cout << "Error, requesting index " << index << ", which out of allPixels(size: " << allPixels_.size() << ")" << std::endl;
        return;
    }
    px.x_ = allPixels_[index].first;
    px.y_ = allPixels_[index].second;

    lr.origin_ = frame_.pointToWorld(frame_.o_);
    lr.dir_ = frame_.vecToWorld(pixelToDir(px));
}

std::string Camera::describe() const
{
    std::stringstream ss;
    ss << "=== Camera ===\n";
    ss << "frame.o: " << frame_.o_ << "\n";
    ss << "frame.vx: " << frame_.vx_ << "\n";
    ss << "frame.vy: " << frame_.vy_ << "\n";
    ss << "frame.vy: " << frame_.vz_ << "\n";
    ss << "fov: " << fovX_ << ", " << fovY_;
    return ss.str();
}

std::string CamData::describe() const
{
    std::stringstream ss;
    ss << "=== CamData ===\n";
    ss << "resX: " << resX << "\n";
    ss << "fovX: " << fovX << "\n";
    ss << "fovY: " << fovY << "\n";
    ss << "pos: " << pos << "\n";
    ss << "rotAxis: " << rotAxis << "\n";
    ss << "rotAngle: " << rotAngle << "\n";
    return ss.str();
}

void Camera::changeResolution(int resX, int resY)
{
    resX_ = resX;
    resY_ = resY;
    initAllPixelsVec();
}
