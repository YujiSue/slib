#include "smedia/sgraphic.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::smedia;

inline void update(mat2f &&m, v2fvec &points, Area<float> &boundary) {
    sforeach(points) E_ = m * E_;
    v2f corner[4]; v4f corner_;
    corner[0] = v2f(boundary.ori_x, boundary.ori_y);
    corner[1] = v2f(boundary.ori_x, boundary.ori_y+boundary.height);
    corner[2] = v2f(boundary.ori_x+boundary.width, boundary.ori_y+boundary.height);
    corner[3] = v2f(boundary.ori_x+boundary.width, boundary.ori_y);
    sforin(i, 0, 4) {
        corner[i] = corner[i]*m;
        if (corner[i].x < boundary.ori_x) corner_.x = corner[i].x;
        if (corner[i].y < boundary.ori_y) corner_.y = corner[i].y;
        if (boundary.ori_x+boundary.width < corner[i].x) corner_.z = corner[i].x;
        if (boundary.ori_y+boundary.height < corner[i].y) corner_.w = corner[i].y;
    }
    boundary.ori_x = corner_.x; boundary.width = corner_.z-corner_.x;
    boundary.ori_y = corner_.y; boundary.height = corner_.w-corner_.y;
}
inline void update2(const mat3f &m, v2fvec &points, Area<float> &boundary) {
    sforeach(points) {
        v3f v_(E_.x, E_.y, 1); v_ = m * v_;
        E_.x = v_.x; E_.y = v_.y;
    }
    v3f corner[4]; v4f corner_;
    corner[0] = v3f(boundary.ori_x, boundary.ori_y, 1.0f);
    corner[1] = v3f(boundary.ori_x, boundary.ori_y+boundary.height, 1.0f);
    corner[2] = v3f(boundary.ori_x+boundary.width, boundary.ori_y+boundary.height, 1.0f);
    corner[3] = v3f(boundary.ori_x+boundary.width, boundary.ori_y, 1.0f);
    sforin(i, 0, 4) {
        corner[i] = corner[i]*m;
        if (corner[i].x < boundary.ori_x) corner_.x = corner[i].x;
        if (corner[i].y < boundary.ori_y) corner_.y = corner[i].y;
        if (boundary.ori_x+boundary.width < corner[i].x) corner_.z = corner[i].x;
        if (boundary.ori_y+boundary.height < corner[i].y) corner_.w = corner[i].y;
    }
    boundary.ori_x = corner_.x; boundary.width = corner_.z-corner_.x;
    boundary.ori_y = corner_.y; boundary.height = corner_.w-corner_.y;
}
void STransform2D::scaling(v2f v, v2f ori, v2fvec &points, Area<float> &boundary) {
    shift(-ori, points, boundary);
    update(mat2f(v.x, 0, 0, v.y), points, boundary);
    shift(ori, points, boundary);
}
void STransform2D::shift(v2f v, v2fvec &points, Area<float> &boundary) {
    sforeach(points) E_ = E_+=v;
    boundary.ori_x += v.x; boundary.ori_y += v.y;
}
void STransform2D::shear(v2f v, v2f ori, v2fvec &points, Area<float> &boundary) {
    shift(-ori, points, boundary);
    update(mat2f(1, tan(v.x), tan(v.y), 1), points, boundary);
    shift(ori, points, boundary);
}
void STransform2D::rotate(float f, v2f ori, v2fvec &points, Area<float> &boundary) {
    shift(-ori, points, boundary);
    update(mat2f(cos(f), sin(f), -sin(f), cos(f)), points, boundary);
    shift(ori, points, boundary);
}
void STransform2D::reflect(uint8_t axis, v2f ori, v2fvec &points, Area<float> &boundary) {
    shift(-ori, points, boundary);
    update(mat2f(axis&smath::HORIZONTAL?-1:1, 0, 0, axis&smath::VERTICAL?-1:1), points, boundary);
    shift(ori, points, boundary);
}
void STransform2D::trans(mat3f affine, v2f ori, v2fvec &points, Area<float> &boundary) {
    shift(-ori, points, boundary);
    update2(affine, points, boundary);
    shift(ori, points, boundary);
}

STransform2D::STransform2D()
    : scale(v2f(1.0f, 1.0f)), rotation(0.0f), reflection(0) {}
STransform2D::STransform2D(const STransform2D &trans)
    : scale(trans.scale), translate(trans.translate),
    skew(trans.skew), rotation(trans.rotation), reflection(trans.reflection) {}
STransform2D::~STransform2D() {}

mat3f STransform2D::transMatrix() const {
    return mat3f((float)(scale.x*(reflection&HORIZONTAL?-1.0f:1.0f)*cos(rotation)),
                 (float)(sin(rotation)*tan(skew.x)),
                 translate.x,
                 (float)(-sin(rotation)*tan(skew.y)),
                 (float)(scale.y*(reflection&VERTICAL?-1.0f:1.0f)*cos(rotation)),
                 translate.y,
                 0.0f, 0.0f, 1.0f);
}
void STransform2D::clear() {
    scale = v2f(1.0f, 1.0f);
    translate = v2f(0.0f, 0.0f);
    skew = v2f(0.0f, 0.0f);
    rotation = 0.0f;
    reflection = 0;
}
