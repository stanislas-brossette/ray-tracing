#include "Camera.hh"
#include "utils.hh"

Camera::Camera()
  : frame_(),
    fovX_(),
    fovY_(),
    resX_()
{
    focalDist_ = 0;
    resY_ = 0;
}

Camera::Camera(const CamData& cData)
  : frame_(),
    fovX_(cData.fovX),
    fovY_(cData.fovY),
    resX_(cData.resX)
{
    frame_.translate(cData.pos);
    frame_.rotate(cData.rotAxis, cData.rotAngle);
    focalDist_ = double(resX_)/(2*std::tan(deg2rad(fovX_)/2));
    resY_ = int(focalDist_ * 2 * std::tan(deg2rad(fovY_)/2));
}

Camera::Camera(const Frame3& frame, double fovX, double fovY, int resX)
  : frame_(frame),
    fovX_(fovX),
    fovY_(fovY),
    resX_(resX)
{
    focalDist_ = double(resX_)/(2*std::tan(deg2rad(fovX_)/2));
    resY_ = int(focalDist_ * 2 * std::tan(deg2rad(fovY_)/2));
}

Camera::~Camera()
{
}

Vector3 Camera::pixelToDir(const Pixel& px) const
{
    Vector3 dir;

    double dx = double(px.x_) - double(resX_)/2;
    double dy = double(px.y_) - double(resY_)/2;
    dir.x_ = dx;
    dir.y_ = focalDist_;
    dir.z_ = dy;
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
