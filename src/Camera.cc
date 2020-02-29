#include "Camera.hh"

Camera::Camera(const Vector3& pos, const Vector3& dir)
  : pos_(pos),
    dir_(dir)
{
}

Camera::~Camera()
{
}
