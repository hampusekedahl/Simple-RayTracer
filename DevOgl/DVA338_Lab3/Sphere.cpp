
#include "Sphere.h"


bool Sphere::hit(const Ray & r, HitRec & rec) const {	
	
	Vec3f v = c - r.o;
	float s = v.dot(r.d);
	float vLenSq = v.dot(v);
	float radSq = this->r * this->r; 
	if (s < 0 && vLenSq > radSq) {
		return false;
	}
	float mSq = vLenSq - s * s;
	if (mSq > radSq) {
		return false;
	}
	
	float B = 2 * (r.o - c).dot(r.d);
	float C = (r.o - c).dot(r.o - c) - radSq;

	float t1 = ((-B) + sqrt(pow(B, 2) - 4 * C)) / 2;
	float t2 = ((-B) - sqrt(pow(B, 2) - 4 * C)) / 2;

	if (t1 > t2) t1 = t2;

	if (t1 < rec.tHit || rec.tHit == 0.0) {
		rec.tHit = t1;
		rec.material = m;
		computeSurfaceHitFields(r, rec);
	}


	rec.anyHit = true;
	return true;
}


void Sphere::computeSurfaceHitFields(const Ray & r, HitRec & rec) const {
	rec.p = r.o + r.d * rec.tHit;
	rec.n = (rec.p - c).normalize();
}
