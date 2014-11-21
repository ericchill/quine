#ifndef INCLUDED_PLANE_H
#define INCLUDED_PLANE_H 1

struct Plane {
  vec4 data;
};

vec3 normal(Plane a) {
  return a.data.xyz;
}

// compute a signed distance to the plane
float distance(Plane p, vec3 q) { return dot(p.data, vec4(q,1.0)); }

// project a point onto a plane
vec3 projectToPlane(Plane p, vec3 q) {
  return q - distance(p,q) * normal(p);
}

// construct a plane from a normal and a distance
Plane plane(vec3 n, float d) {
  return Plane(vec4(n,d));
}

// construct a plane from a normal and a point
Plane plane(vec3 n, vec p) {
  return plane(n,dot(n,p));
}

// construct a plane from 3 non-collinear points
Plane plane(vec3 a, vec3 b, vec3 c) {
  return plane(cross(b-a,c-a),a);
}

#endif