uniform sampler1D u_packed0;    // pos = packed0.rgb, age = packed0.a, sign(age) indicates at rest vs. moving, FP32
uniform sampler1D u_packed1;    // up  = packed1.xy, two 8-bit advected quantities = up.zw                   , RGBA8888
// uniform sampler1D u_packed2; // vel = packed2.xyz, delta_age = packed2.a                                  , FP16

uniform mat3 u_invView;       // inverse view transformation matrix
uniform mat4 u_worldViewProj; // world view projection matrix

// TODO: pack?
varying float v_age; // age of the verte. sign indicates restfulness
varying vec3  v_pos; // vertex position
varying vec3  v_tex; // texture coords
varying vec2  v_adv; // two 8 bit advected quantities
// TODO: lookup more information in a type array and pass it as varying data to the fragment shader

// v%6, x      , y
// 0  , -1 r%2 ,  1  000 = 0, (r & 6) && ! (r % 2)
// 1  ,  1     ,  1  001 = 0
// 2  , -1     , -1  010 = 1
// 3  ,  1     ,  1  011 = 0
// 4  , -1     , -1  100 = 1
// 5  ,  1     , -1  110 = 1

void main() {
  uint q = gl_VertexID / 6;
  uint r = gl_VertexID % 6;
  uint o = gl_VertexID % 2;
  vec4 pos = texelFetch(u_packed0, q);
  vec4 up  = texelFetch(u_packed1, q);
  // compute quads here in the vertex shader as pairs of triangles since GL_QUADS doesn't work any more
  // use 'up' vector as a scale and to pick out the 2d rotation matrix
  vec2 offset    = mat2(up.xy,-up.y,up.x) * vec2(o ? 1.0 : -1.0, r & 6 ? (o ? 1.0 : -1.0) : 1.0);
  v_pos          = u_worldViewProj * vec4(vec3(offset, 0.0) * u_invView + pos.xyz, 1.0);
  v_age          = pos.a;
  v_advected     = up.ba;
  v_uv.xy        = vec2(o ? 1.0 : 0.0, r & 6 ? (o ? 0.0 : 1.0) : 0.0); // texture coordinates
}