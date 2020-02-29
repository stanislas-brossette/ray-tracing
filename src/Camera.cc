#include "Camera.hh"

Camera::Camera(const Vector3& pos, const Vector3& dir, double fov)
  : pos_(pos),
    dir_(dir),
    fov_(fov)
{
}

Camera::~Camera()
{
}

std::string Camera::describe() const
{
  std::stringstream ss;
  ss << "=== Camera ===\n";
  ss << "pos: " << pos_ << "\n";
  ss << "dir: " << dir_ << "\n";
  ss << "fov: " << fov_;
  return ss.str();
}
