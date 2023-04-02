#include "light.h"

void Light::computeAmbientColor(HitRec& rec) const {

	rec.color = rec.material.Ambient.multCoordwise(Ambient);
}

void Light::computeDiffuseColor(HitRec& rec) const {
	float DiffuseComponent = rec.n.normalize().dot((Position - rec.p).normalize());

	if (DiffuseComponent < 0) DiffuseComponent = 0;
	if (DiffuseComponent > 1) DiffuseComponent = 1;

	Vec3f Color = rec.material.Diffuse.multCoordwise(Diffuse) * DiffuseComponent;
	rec.color += Color;
}

void Light::computeSpecularColor(const Ray& r, HitRec& rec) const {
	Vec3f L = (Position - rec.p).normalize();
	Vec3f V = (r.o - rec.p).normalize();
	Vec3f R = (rec.n.normalize()) * (2 * L.dot(rec.n.normalize())) - L;

	float SpecularComponent = R.dot(V);
	if (SpecularComponent < 0) SpecularComponent = 0;
	if (SpecularComponent > 1) SpecularComponent = 1;

	SpecularComponent = pow(SpecularComponent, rec.material.Shininess);

	Vec3f Color = rec.material.Specular.multCoordwise(Specular) * SpecularComponent;
	rec.color += Color;
}