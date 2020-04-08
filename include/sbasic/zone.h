#ifndef SLIB_ZONE_H
#define SLIB_ZONE_H

#include "smath/la.h"

namespace slib {
    #define szone Zone<sint>
    #define szonef Zone<float>
    #define szoned Zone<double>
    
    template<typename T>
    struct Zone {
        T ori_x, ori_y, ori_z, width, height, depth;
        
        Zone();
        Zone(const T &x, const T &y, const T &z, const T &w, const T &h, const T &d);
        Zone(const Zone &z);
        ~Zone();
        
        Zone &operator =(const Zone &z);
        smath::SVector3D<T> center() const;
        T volume() const;
        bool include(const T &x, const T &y, const T &z) const;
        bool include(const Zone &z) const;
        bool overlap(const Zone &z) const;
        void merge(const Zone &z);
        Zone conjunction(const Zone &z);
        bool operator < (const Zone &z) const;
        bool operator == (const Zone &z) const;
        bool operator != (const Zone &z) const;
    };
    
    /*============================================================*/
    template<typename T>
    Zone<T>::Zone()
    : ori_x(initVal<T>()), ori_y(initVal<T>()), ori_z(initVal<T>()),
    width(initVal<T>()), height(initVal<T>()), depth(initVal<T>()) {}
    template<typename T>
    Zone<T>::Zone(const T &x, const T &y, const T &z, const T &w, const T &h, const T &d)
    : ori_x(x), ori_y(y), ori_z(z), width(w), height(h), depth(d) {}
    template<typename T>
    Zone<T>::Zone(const Zone &z)
    : ori_x(z.ori_x), ori_y(z.ori_y), ori_z(z.ori_z), width(z.width), height(z.height), depth(z.depth) {}
    template<typename T>
    Zone<T>::~Zone() {}
    template<typename T>
    Zone<T> &Zone<T>::operator=(const Zone<T> &z) {
        ori_x = z.ori_x; ori_y = z.ori_y; ori_z = z.ori_z;
        width = z.width; height = z.height; depth = z.depth;
        return *this;
    }
    template<typename T>
    smath::svec3d<T> Zone<T>::center() const {
        return smath::svec3d<T>(ori_x+width/2, ori_y+height/2, ori_z+depth/2);
    }
    template<typename T>
    T Zone<T>::volume() const { return width*height*depth; }
    template<typename T>
    bool Zone<T>::include(const T &x, const T &y, const T &z) const {
        return ori_x <= x && x <= (ori_x+width) &&
        ori_y <= y && y <= (ori_y+height) &&
        ori_z <= z && z <= (ori_z+depth);
    }
    template<typename T>
    bool Zone<T>::include(const Zone<T> &z) const {
        return ori_x <= z.ori_x && (z.ori_x + z.width) <= (ori_x + width) &&
        ori_y <= z.ori_y && (z.ori_y + z.height) <= (ori_y + height) &&
        ori_z <= z.ori_z && (z.ori_z + z.depth) <= (ori_z + depth);
    }
    template<typename T>
    bool Zone<T>::overlap(const Zone<T> &z) const {
        return ori_x <= (z.ori_x + z.width) && z.ori_x <= (ori_x + width) &&
        ori_y <= (z.ori_y + z.height) && z.ori_y <= (ori_y + height) &&
        ori_z <= (z.ori_z + z.depth) && z.ori_z <= (ori_z + depth);
    }
    template<typename T>
    void Zone<T>::merge(const Zone<T> &z) {
        T end_x = ori_x+width<z.ori_x+z.width?z.ori_x+z.width:ori_x+width;
        if(z.ori_x < ori_x) ori_x = z.ori_x;
        width = end_x-ori_x;
        T end_y = ori_y+height<z.ori_y+z.height?z.ori_y+z.height:ori_y+height;
        if(z.ori_y < ori_y) ori_y = z.ori_y;
        height = end_y-ori_y;
        T end_z = ori_z+depth<z.ori_z+z.depth?z.ori_z+z.depth:ori_z+depth;
        if(z.ori_z < ori_z) ori_z = z.ori_z;
        depth = end_z-ori_z;
    }
    template<typename T>
    Zone<T> Zone<T>::conjunction(const Zone<T> &z) {
        if (include(z)) return z;
        else if (overlap(z)) {
            T x1_ = ori_x<z.ori_x?z.ori_x:ori_x,
            y1_ = ori_y<z.ori_y?z.ori_y:ori_y,
            z1_ = ori_y<z.ori_z?z.ori_z:ori_z,
            x2_ = ori_x+width<z.ori_x+z.width?ori_x+width:z.ori_x+z.width,
            y2_ = ori_y+height<z.ori_y+z.height?ori_y+height:z.ori_y+z.height,
            z2_ = ori_z+depth<z.ori_z+z.depth?ori_z+depth:z.ori_z+z.depth;
            return Zone(x1_, y1_, z1_, x2_-x1_, y2_-y1_, z2_-z1_);
        }
        else return Zone();
    }
    template<typename T>
    bool Zone<T>::operator < (const Zone<T> &z) const {
        if(ori_x != z.ori_x) return ori_x < z.ori_x;
        if(ori_y != z.ori_y) return ori_y < z.ori_y;
        if(ori_z != z.ori_z) return ori_z < z.ori_z;
        if(width != z.width) return width < z.width;
        if(height != z.height) return height < z.height;
        if(depth != z.depth) return depth < z.depth;
    }
    template<typename T>
    bool Zone<T>::operator == (const Zone<T> &z) const {
        return ori_x == z.ori_x && ori_y == z.ori_y && ori_z == z.ori_z &&
        width == z.width && height == z.height && depth == z.depth;
    }
    template<typename T>
    bool Zone<T>::operator != (const Zone<T> &z) const { return (*this) != z; }
}
#endif
