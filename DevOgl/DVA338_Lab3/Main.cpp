#include<vector>
#include<iostream>
#include <chrono>
using namespace std;

#include <glut.h>
#include <freeglut.h>

#include "Vec3.h"
#include "Mat4.h"
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Camera.h"
#include "Light.h"


int sphere_intersection_tests;
bool slower_render = false;

class Scene {
public:
	vector<Sphere> spheres;
	vector<Triangle> triangles;
	vector<Light> lights;

	Scene(void) {
		 
	}
	void addLight(Light& l) {
		lights.push_back(l);
		//cout << "Light added: " << "x  = " << lights[lights.size()-1].Position.x << endl;
	}

	void addSphare(Sphere & s) {
		spheres.push_back(s); 
		//cout << "Sphere added: " << "r = " << spheres[spheres.size()-1].r << endl;
	}

	void addTriangle(Triangle& t) {
		triangles.push_back(t);
		//cout << "Triangle added: " << endl;
	}

};


void glSetPixel(int x, int y, Vec3f & c) {
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

class SimpleRayTracer {
private: 
	Camera* camera;
	Scene * scene;
	Image * image;
	Vec3f bgColor = Vec3f(0.2f, 0.2f, 0.4f);

	Vec3f getEyeRayDirection(int x, int y) {
		float imageAspectRatio = float(image->getWidth()) / float(image->getHeight()); 
		float Px = (2 * ((x + 0.5) / float(image->getWidth())) - 1) * tan(90 / 2 * 3.14 / 180) * imageAspectRatio;
		float Py = (1 - 2 * ((y + 0.5) / float(image->getHeight())) * tan(90 / 2 * 3.14 / 180));
		Vec3f rayOrigin = Vec3f(0.0f, 0.0f, 0.0f);
		Mat4f T;
		T = MatMatMul(RotationX(camera->Direction), MatMatMul(RotationY(camera->Direction), RotationZ(camera->Direction)));
		Vec3f rayOriginWorld, rayPWorld;
		rayOriginWorld = T.MatVecMul(rayOrigin);
		rayPWorld = T.MatVecMul(Vec3f(Px, Py, camera->Zoom));
		Vec3f rayDirection = rayPWorld - rayOriginWorld;
		rayDirection.normalize(); 
		return rayDirection;
	}


public:
	SimpleRayTracer(Scene * scene, Image * image, Camera * camera) {
		this->camera = camera;
		this->scene = scene;
		this->image = image;
	}

	void rayTrace(const Ray& ray, HitRec& hitRec, int depth) {
		searchClosestHit(ray, hitRec);
		if (hitRec.anyHit) Lightning(ray, hitRec, depth);
		else hitRec.color = bgColor;
	}

	void searchClosestHit(const Ray & ray, HitRec & hitRec) {
		int i;
		for (i = 0; i < scene->spheres.size(); i++) {
			scene->spheres[i].hit(ray, hitRec);
		}
		sphere_intersection_tests += i;
		
		for (i = 0; i < scene->triangles.size(); i++) {
			scene->triangles[i].hit(ray, hitRec);
		}
	}

	void Lightning(const Ray& ray, HitRec& hitRec, int depth) {
		Ray shadowRay;
		HitRec shadowRec;
		shadowRay.o = hitRec.p + hitRec.n * 0.001;
		shadowRay.d = (scene->lights[0].Position - hitRec.p).normalize();
		shadowRec.anyHit = false;
		shadowRec.tHit = 0.0;

		int i;

		for (i = 0; i < scene->spheres.size(); i++) {
			scene->spheres[i].hit(shadowRay, shadowRec);
		}

		sphere_intersection_tests += i;

		for (i = 0; i < scene->triangles.size(); i++) {
			scene->triangles[i].hit(shadowRay, shadowRec);
		}
		
		hitRec.color = hitRec.material.Ambient;

		if (shadowRec.anyHit == false) {
			for (i = 0; i < scene->lights.size(); i++) {
				scene->lights[i].computeDiffuseColor(hitRec);
				scene->lights[i].computeSpecularColor(ray, hitRec);
			}
		}
		if(!slower_render){
		if (hitRec.material.Reflection > 0) {
			Ray reflectionRay;
			HitRec reflectionRec;

			Vec3f L = (ray.o - hitRec.p).normalize();
			Vec3f R = (hitRec.n.normalize()) * (2 * L.dot(hitRec.n.normalize())) - L;
			reflectionRec.anyHit = false;
			reflectionRec.tHit = 0.0;
			reflectionRay.o = hitRec.p + hitRec.n * 0.001;
			reflectionRay.d = R.normalize();

			if (depth < 3) rayTrace(reflectionRay, reflectionRec, depth + 1);

			if (reflectionRec.anyHit == true) {
				hitRec.color += reflectionRec.color * hitRec.material.Reflection;
			}
		}
		}

	}

	void fireRays(void) { 
		Ray ray;
		HitRec hitRec;
		ray.o = camera->Origin;
		for (int y = 0; y < image->getHeight(); y++) {
			for (int x = 0; x < image->getWidth(); x++) {
				hitRec.anyHit = false;
				hitRec.tHit = 0.0;
				ray.d = getEyeRayDirection(x, y);
				rayTrace(ray, hitRec, 0);
				image->setPixel(x, y, hitRec.color);
				glSetPixel(x, y, hitRec.color);
			}
		}
	}
};

SimpleRayTracer * rayTracer;
Camera* camera;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	sphere_intersection_tests = 0;

	auto start = std::chrono::high_resolution_clock::now();
	rayTracer->fireRays();
	if (slower_render) {
		slower_render = false;
		glutPostRedisplay();
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> float_ms = end - start;
	std::cout << "Time to render scene:" << float_ms.count() << " milliseconds :: " << " sphere_intersection_tests:" << sphere_intersection_tests << std::endl;

	glFlush();
}

void changeSize(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glViewport(0,0,w,h);
}

void keypress(unsigned char key, int x, int y) {
	switch (key) {
	case 'z':
		slower_render = true;
		camera->Origin.z -= 2.0f;
		break;
	case 'Z':
		slower_render = true;
		camera->Origin.z += 2.0f;
		break;
	case 'w':
		slower_render = true;
		camera->Origin.y -= 2.0f;
		break;
	case 's':
		slower_render = true;
		camera->Origin.y += 2.0f;
		break;
	case 'a':
		slower_render = true;
		camera->Origin.x -= 2.0f;
		break;
	case 'd':
		slower_render = true;
		camera->Origin.x += 2.0f;
		break;
	case 'j':
		slower_render = true;
		camera->Direction.y -= 2.0f;
		break;
	case 'J':
		slower_render = true;
		camera->Direction.y += 2.0f;
		break;
	case 'k':
		slower_render = true;
		camera->Direction.z -= 2.0f;
		break;
	case 'K':
		slower_render = true;
		camera->Direction.z += 2.0f;
		break;
	case 'i':
		slower_render = true;
		camera->Direction.x -= 2.2f;
		break;
	case 'I':
		slower_render = true;
		camera->Direction.x += 2.0f;
		break;
	case 'f':
		camera->Zoom -=0.5f;
		break;
	case 'F':
		camera->Zoom +=0.5f;
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void init(void)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Redovisning Lab 3");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keypress);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

	Material jade(Vec3f(0.135f, 0.2225f, 0.1575f), Vec3f(0.54f, 0.89f, 0.63f), Vec3f(0.316228f, 0.316228f, 0.316228f), 12.8f, 0.5f);
	Material gold(Vec3f(0.24725f, 0.1995f, 0.0745f), Vec3f(0.75164f, 0.60648f, 0.22648f), Vec3f(0.628281f, 0.555802f, 0.366065f), 51.2f, 0.5f);
	Material silver(Vec3f(0.19225f, 0.19225f, 0.19225f), Vec3f(0.50754f, 0.50754f, 0.50754f), Vec3f(0.508273f, 0.508273f, 0.508273f), 51.2f, 0.5f);
	Material redwall(Vec3f(0.1745f, 0.01175f, 0.01175f), Vec3f(0.61424f, 0.04136f, 0.04136f), Vec3f(0.727811f, 0.626959f, 0.626959f), 76.2f, 0.5f);
	Scene * scene = new Scene;

	/*
	//scene 1
	Sphere s(Vec3f(0.0f, 0.0f, -13.0f), 3.0f, jade);
	Sphere s1(Vec3f(8.0f, 0.0f, -13.0f), 3.0f, gold);
	Sphere s2(Vec3f(-8.0f, 0.0f, -13.0f), 3.0f, silver);
	scene->addSphare(s);
	scene->addSphare(s1);
	scene->addSphare(s2);
	Triangle t(Vec3f(-25.0f, 10.0f, -25.0f), Vec3f(25.0f, 10.0f, -25.0f), Vec3f(-25.0f, -10.0f, -25.0f), Vec3f(0.0f, 0.0f, 0.0f), redwall);
	Triangle t1(Vec3f(-25.0f, -10.0f, -25.0f), Vec3f(25.0f, 10.0f, -25.0f), Vec3f(25.0f, -10.0f, -25.0f), Vec3f(0.0f, 0.0f, 0.0f), redwall);
	scene->addTriangle(t);
	scene->addTriangle(t1);
	Light light(Vec3f(0.0f, 0.0f, 10.0f), Vec3f(0.2f, 0.2f, 0.2f), Vec3f(1.0f, 1.0f, 1.0f), Vec3f(1.0f, 1.0f, 1.0f));
	scene->addLight(light);
	camera = new Camera(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f), -1.0f);
	*/

	/**/
	//scene 2
	Sphere s(Vec3f(0.0f, 0.0f, -35.0f), 3.0f, jade);
	Sphere s1(Vec3f(8.0f, 0.0f, -30.0f), 3.0f, gold);
	Sphere s2(Vec3f(-8.0f, 0.0f, -25.0f), 3.0f, silver);
	scene->addSphare(s);
	scene->addSphare(s1);
	scene->addSphare(s2);
	Triangle t(Vec3f(25.0f, 5.0f, -50.0f), Vec3f(-25.0f, 5.0f, -50.0f), Vec3f(-25.0f, 5.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f), redwall);
	Triangle t1(Vec3f(25.0f, 5.0f, -50.0f), Vec3f(-25.0f, 5.0f, 0.0f), Vec3f(25.0f, 5.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f), redwall);
	scene->addTriangle(t);
	scene->addTriangle(t1);
	Light light(Vec3f(10.0f, -20.0f, -15.0f), Vec3f(0.2f, 0.2f, 0.2f), Vec3f(1.0f, 1.0f, 1.0f), Vec3f(1.0f, 1.0f, 1.0f));
	scene->addLight(light);
	camera = new Camera(Vec3f(0.0f, 0.0f, -12.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f), -10.0f);
	

	Image * image = new Image(640, 480);
	
	rayTracer = new SimpleRayTracer(scene, image, camera);
}

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	init();
	glutMainLoop();
}
