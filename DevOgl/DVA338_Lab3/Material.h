#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Vec3.h"

class Material {
public:
	Vec3f Ambient;
	Vec3f Diffuse;
	Vec3f Specular;
	float Shininess;
	float Reflection;

	Material() {};
	Material(const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular, float shininess, float reflection) : Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess), Reflection(reflection) { }
};

#endif
