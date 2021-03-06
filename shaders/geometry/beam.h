#ifndef INCLUDED_GEOMETRY_BEAM_H
#define INCLUDED_GEOMETRY_BEAM_H

#include "math/projective.h"
#include "geometry/sphere.h"
#include "geometry/ray.h"

struct Beam {
  vec4 origin;   // xyz: origin,   w: radius
  vec4 dir;      // xyz: unit dir, w: delta radius per unit time
  vec3 recipDir; // vec3(1.)/dir.xyz
};

Beam beam(vec4 origin, vec4 dir) {
  return Beam(origin, dir, vec3(1.0)/dir.xyz);
}

Beam beam(vec3 origin, vec4 dir) {
  // not a vector, but 0 radius at origin
  return Beam(vector(origin), dir, vec3(1.0)/dir.xyz);
}

Beam beam(vec3 origin, vec3 dir) {
  return beam(vector(origin), vector(dir));
}

// Build a beam that starts at 'a' and goes to be growing/shrinking linearly to match 'b' whenever it gets
// there at unit speed.
Beam beam(Sphere a, Sphere b) {
  vec4 d = b.data - a.data;
  return beam(a.data,d*inversesqrt(dot(d.xyz,d.xyz)));
}

Sphere at(Beam a, float t) {
  return Sphere(a.origin + t*a.dir);
}

// find the ray for the center of this beam
Ray ray(Beam a) {
  return Ray(a.origin.xyz,a.dir.xyz,a.recipDir.xyz);
}

// NB: may give nonsensical radii when z = infinity.
Beam beam(Ray a) {
  const vec4 v = vec4(1.0,1.0,1.0,0.0);
  return Beam(a.origin.xyzz * v, a.dir.xyzz * v, a.recipDir);
}

#endif // INCLUDED_GEOMETRY_BEAM_H
