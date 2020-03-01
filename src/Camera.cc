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
  std::cout << "Camera::castRandomRay" << std::endl;
  int randX = std::rand();
  int randY = std::rand();

  double randXfov = double(randX % (fovX_*10))/10.0 - double(fovX_) / 2;
  double randZfov = double(randY % (fovY_*10))/10.0 - double(fovY_) / 2;
  std::cout << "randXfov: " << randXfov << std::endl;
  std::cout << "randZfov: " << randZfov << std::endl;
  std::cout << "dir_ before rotation: " << dir_ << std::endl;
  Vector3 rotV = dir_.rotateX(randXfov);
  std::cout << "dir_ after rotation of angle " << randXfov << "around X axis: " << rotV << std::endl;
  rotV = rotV.rotateZ(randZfov);
  std::cout << "dir_ after rotation of angle " << randZfov << "around Z axis: " << rotV << std::endl;

  double randXpixel = (randXfov + double(fovX_)/2)*double(resX_)/double(fovX_);
  double randYpixel = (randZfov + double(fovY_)/2)*double(resY_)/double(fovY_);

  std::cout << "pos_:" << pos_ << std::endl;
  std::cout << "pixel: " << randXpixel << ", " << randYpixel << std::endl;
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
