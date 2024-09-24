#include "smedia/sgraphic.h"
slib::Stroke::Stroke(sushort t, float w, const SColor& col) : type(t), width(w), color(col) { memset(interval, 0, 2 * sizeof(int)); }
slib::Stroke::Stroke(const Stroke& st) {
    type = st.type;
    width = st.width;
    color = st.color;
    interval[0] = st.interval[0];
    interval[1] = st.interval[1];
}
slib::Stroke::~Stroke() {}
slib::Stroke& slib::Stroke::operator=(const Stroke& st) {
    type = st.type;
    width = st.width;
    color = st.color;
    interval[0] = st.interval[0];
    interval[1] = st.interval[1];
    return *this;
}
slib::Transform2D::Transform2D() { reset(); }
slib::Transform2D::~Transform2D() {}
void slib::Transform2D::setOrigin(const Area<float>& area, ORIGIN_TYPE type) {
    switch (type) {
    case ORIGIN_TYPE::ZERO:
        origin = svec2f(0, 0);
        break;
    case ORIGIN_TYPE::TOP_LEFT:
        origin = svec2f(area.ori_x, area.ori_y);
        break;
    case ORIGIN_TYPE::BOTTOM_LEFT:
        origin = svec2f(area.ori_x, area.ori_y + area.height);
        break;
    case ORIGIN_TYPE::TOP_RIGHT:
        origin = svec2f(area.ori_x + area.width, area.ori_y + area.height);
        break;
    case ORIGIN_TYPE::BOTTOM_RIGHT:
        origin = svec2f(area.ori_x + area.width, area.ori_y);
        break;
    case ORIGIN_TYPE::CENTER:
        origin = svec2f(area.ori_x + area.width / 2.0f, area.ori_y + area.height / 2.0f);
        break;
    default:
        origin = svec2f(0, 0);
        break;
    }
}
void slib::Transform2D::transform(smath::Vector<smath::Vector2D<float>>& vertex) {
    smat2f smat(1, tan(shear[0]), tan(shear[1]), 1);
    smat2f rmat(cos(rotation), sin(rotation), -sin(rotation), cos(rotation));
#pragma omp parallel for
    sfor(vertex) {
        // Translation : _origin => (0, 0)
        $_ -= origin;
        // Reflection
        if (reflect[0]) $_[0] *= -1;
        if (reflect[1]) $_[1] *= -1;
        // Rotation (CW)
        $_ = rmat * $_;
        // Shearing
        $_ = smat * $_;
        // Scaling
        $_[0] *= scale[0];
        $_[1] *= scale[1];        
        // Final translation
        $_ += shift + origin;
    }
}
void slib::Transform2D::reset() {
    origin = svec2f(0.f, 0.f);
    scale = svec2f(1.f, 1.f);
    shift = svec2f(0.f, 0.f);
    shear = svec2f(0.f, 0.f);
    rotation = 0.f;
    reflect[0] = false;
    reflect[1] = false;
}
