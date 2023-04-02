#ifndef _MAT4_H_
#define _MAT4_H_
#define _USE_MATH_DEFINES

#include "math.h"
#include "Vec3.h"

template<class T>
class Mat4 {
public:
	union {
		T e[16];
	};

	Mat4() { }
	Mat4(const T a[16]) {
		e[0] = a[0]; e[1] = a[1]; e[2] = a[2]; e[3] = a[3]; e[4] = a[4]; e[5] = a[5];e[6] = a[6]; e[7] = a[7]; e[8] = a[8];
		e[9] = a[9]; e[10] = a[10]; e[11] = a[11];e[12] = a[12]; e[13] = a[13]; e[14] = a[14];e[15] = a[15];
	}

	void set(const T a[16]) {
		e[0] = a[0]; e[1] = a[1]; e[2] = a[2]; e[3] = a[3]; e[4] = a[4]; e[5] = a[5];e[6] = a[6]; e[7] = a[7]; e[8] = a[8];
		e[9] = a[9]; e[10] = a[10]; e[11] = a[11];e[12] = a[12]; e[13] = a[13]; e[14] = a[14];e[15] = a[15];
	}
	
	bool normalizeIf(void);

	Vec3f MatVecMul(const Vec3f& v) const { return Vec3f(v.x * e[0] + v.y * e[4] + v.z * e[8] + e[12], v.x * e[1] + v.y * e[5] + v.z * e[9] + e[13], v.x * e[2] + v.y * e[6] + v.z * e[10] + e[14]); }
};

template<class T>
bool Mat4<T>::normalizeIf(void) {
	return true;
}

typedef Mat4<float> Mat4f;

Mat4f MatMatMul(Mat4f a, Mat4f b) {
	Mat4f c;
	int i, j, k;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			c.e[j * 4 + i] = 0.0;
			for (k = 0; k < 4; k++)
				c.e[j * 4 + i] += a.e[k * 4 + i] * b.e[j * 4 + k];
		}
	}
	return c;
}

Mat4f RotationX(Vec3f rotation) {
	Mat4f R;
	R.e[0] = 1.0f; R.e[4] = 0.0f; R.e[8] = 0.0f; R.e[12] = 0.0;
	R.e[1] = 0.0f; R.e[5] = cos(rotation.x * M_PI / 180.0); R.e[9] = -sin(rotation.x * M_PI / 180.0); R.e[13] = 0.0;
	R.e[2] = 0.0f; R.e[6] = sin(rotation.x * M_PI / 180.0); R.e[10] = cos(rotation.x * M_PI / 180.0); R.e[14] = 0.0;
	R.e[3] = 0.0f; R.e[7] = 0.0f; R.e[11] = 0.0f; R.e[15] = 1.0f;
	return R;
}

Mat4f RotationY(Vec3f rotation) {
	Mat4f R;
	R.e[0] = cos(rotation.y * M_PI / 180.0); R.e[4] = 0.0f; R.e[8] = sin(rotation.y * M_PI / 180.0); R.e[12] = 0.0;
	R.e[1] = 0.0f; R.e[5] = 1.0; R.e[9] = 0.0; R.e[13] = 0.0;
	R.e[2] = -sin(rotation.y * M_PI / 180.0); R.e[6] = 0.0; R.e[10] = cos(rotation.y * M_PI / 180.0);R.e[14] = 0.0;
	R.e[3] = 0.0f; R.e[7] = 0.0f; R.e[11] = 0.0f; R.e[15] = 1.0f;
	return R;
}

Mat4f RotationZ(Vec3f rotation) {
	Mat4f R;
	R.e[0] = cos(rotation.z * M_PI / 180.0); R.e[4] = -sin(rotation.z * M_PI / 180.0); R.e[8] = 0.0f; R.e[12] = 0.0;
	R.e[1] = sin(rotation.z * M_PI / 180.0); R.e[5] = cos(rotation.z * M_PI / 180.0); R.e[9] = 0.0f; R.e[13] = 0.0;
	R.e[2] = 0.0f; R.e[6] = 0.0f; R.e[10] = 1.0f; R.e[14] = 0.0;
	R.e[3] = 0.0f; R.e[7] = 0.0f; R.e[11] = 0.0f; R.e[15] = 1.0f;
	return R;
}

#endif
