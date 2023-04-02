#include "Triangle.h"


bool Triangle::hit(const Ray& r, HitRec& rec) const {
    const float EPSILON = 0.0000001;
    Vec3f edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = vertex2 - vertex1;
    edge2 = vertex3 - vertex1;
    h = r.d.cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return false;    
    f = 1.0 / a;
    s = r.o - vertex1;
    u = f * s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = s.cross(edge1);
    v = f * r.d.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
  
    float t = f * edge2.dot(q);
    if (t > EPSILON)
    {

        if (t < rec.tHit || rec.tHit == 0.0) {
            rec.tHit = t;
            rec.material = m;
            computeSurfaceHitFields(r, rec);
        }
        rec.anyHit = true;
        return true;
    }
    else 
        return false;
}


void Triangle::computeSurfaceHitFields(const Ray& r, HitRec& rec) const {
    rec.p = r.o + r.d * rec.tHit;
    rec.n = -((vertex2 - vertex1).cross(vertex3 - vertex1)).normalize();
}