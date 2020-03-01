#include "Camera.hh"

Camera::Camera(const Vector3& pos, const Vector3& dir, int fovX, int fovY)
  : pos_(pos),
    dir_(dir),
    fovX_(fovX),
    fovY_(fovY)
{
}

Camera::~Camera()
{
}

LightRay Camera::castRandomRay() const
{
  std::cout << "Camera::castRandomRay" << std::endl;
  double randX = double(std::rand() % fovX_) - double(fovX_) / 2;
  double randZ = double(std::rand() % fovY_) - double(fovY_) / 2;
  std::cout << "randX: " << randX << std::endl;
  std::cout << "randZ: " << randZ << std::endl;
  std::cout << "dir_ before rotation: " << dir_ << std::endl;
  Vector3 rotV = dir_.rotateX(randX);
  std::cout << "dir_ after rotation of angle " << randX << "around X axis: " << rotV << std::endl;
  rotV = rotV.rotateZ(randZ);
  std::cout << "dir_ after rotation of angle " << randZ << "around Z axis: " << rotV << std::endl;

  std::cout << "pos_:" << pos_ << std::endl;
  LightRay lr(pos_, rotV);
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
