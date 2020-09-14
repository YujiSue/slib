#include "smedia/sgraphic.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::smedia;

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
void STransform2D::expand(v2f& s, v2fvec& vertex, v2f ori) {
	ori *= -1.0;
	shift(ori, vertex);
	mat2f mat(s.x, 0, 0, s.y);
	sforeach(vertex) E_ = mat * E_;
	ori *= -1.0;
    shift(ori, vertex);
}
void STransform2D::shift(v2f& t, v2fvec& vertex) {
	sforeach(vertex) E_ += t;
}
void STransform2D::shear(v2f& s, v2fvec& vertex, v2f ori) {
	ori *= -1.0;
	shift(ori, vertex);
	mat2f mat(1, tan(s.x), tan(s.y), 1);
	sforeach(vertex) E_ = mat * E_;
	ori *= -1.0;
	shift(ori, vertex);
}
void STransform2D::rotate(float rot, v2fvec& vertex, v2f ori) {
	ori *= -1.0;
	shift(ori, vertex);
	mat2f mat(cos(rot), sin(rot), -sin(rot), cos(rot));
	sforeach(vertex) E_ = mat * E_;
	ori *= -1.0;
	shift(ori, vertex);
}
void STransform2D::reflect(subyte ref, v2fvec& vertex, v2f ori) {
	ori *= -1.0;
	shift(ori, vertex);
	mat2f mat((ref == smath::HORIZONTAL ? -1.f : 1.f), 0.f, 0.f, (ref == smath::VERTICAL ? -1.f : 1.f));
	sforeach(vertex) E_ = mat * E_;
	ori *= -1.0;
	shift(ori, vertex);
}
STransform2D::STransform2D() { clear(); }
STransform2D::STransform2D(const STransform2D &trans)
    : scale(trans.scale), translate(trans.translate),
    skew(trans.skew), rotation(trans.rotation), reflection(trans.reflection) {}
STransform2D::~STransform2D() {}

void STransform2D::clear() {
    scale = v2f(1.0f, 1.0f);
    translate = v2f(0.0f, 0.0f);
    skew = v2f(0.0f, 0.0f);
    rotation = 0.0f;
    reflection = 0;
}
