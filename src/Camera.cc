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
  std::cout << "castRandomRay" << std::endl;
  double randX = double(std::rand() % fovX_) - double(fovX_) / 2;
  double randY = double(std::rand() % fovY_) - double(fovY_) / 2;
  std::cout << "randX: " << randX << std::endl;
  std::cout << "randY: " << randY << std::endl;

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
