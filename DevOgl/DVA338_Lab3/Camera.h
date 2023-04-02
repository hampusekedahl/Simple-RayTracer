#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vec3.h"

class Camera {
public:
	Vec3f Origin, Direction, Up;
	float Zoom;
public:
	Camera(const Vec3f origin, const Vec3f direction, const Vec3f up, float zoom) : Origin(origin), Direction(direction), Up(up), Zoom(zoom) { }
};

#endif
