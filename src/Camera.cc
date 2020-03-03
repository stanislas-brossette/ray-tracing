#include "Camera.hh"

Camera::Camera(const Frame3& frame, int fovX, int fovY, int resX, int resY)
  : frame_(frame),
    fovX_(fovX),
    fovY_(fovY),
    resX_(resX),
    resY_(resY)
{
}

Camera::~Camera()
{
}

LightRay Camera::castRandomRay() const
{
  int randX = std::rand();
  int randY = std::rand();

  double randXfov = double(randX % (fovX_*10))/10.0 - double(fovX_) / 2;
  double randZfov = double(randY % (fovY_*10))/10.0 - double(fovY_) / 2;
  Vector3 rotV = frame_.vy_.rotateX(randXfov);
  rotV = rotV.rotateZ(randZfov);

  double randXpixel = (randXfov + double(fovX_)/2)*double(resX_)/double(fovX_);
  double randYpixel = (randZfov + double(fovY_)/2)*double(resY_)/double(fovY_);

  LightRay lr(frame_.o_, rotV, randXpixel, randYpixel);
  return lr;
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
