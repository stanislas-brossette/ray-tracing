#include "Camera.hh"

Camera::Camera(const Vector3& pos, const Vector3& dir, int fovX, int fovY, int resX, int resY)
  : pos_(pos),
    dir_(dir),
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
  Vector3 rotV = dir_.rotateX(randXfov);
  rotV = rotV.rotateZ(randZfov);

  double randXpixel = (randXfov + double(fovX_)/2)*double(resX_)/double(fovX_);
  double randYpixel = (randZfov + double(fovY_)/2)*double(resY_)/double(fovY_);

  LightRay lr(pos_, rotV, randXpixel, randYpixel);
  return lr;
}

std::string Camera::describe() const
{
  std::stringstream ss;
  ss << "=== Camera ===\n";
  ss << "pos: " << pos_ << "\n";
  ss << "dir: " << dir_ << "\n";
  ss << "fov: " << fovX_ << ", " << fovY_;
  return ss.str();
}
