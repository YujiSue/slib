#ifndef SLIB_CV_ADAPTOR_H
#define SLIB_CV_ADAPTOR_H

#include "smedia/simage.h"
#include "opencv/cv.h"
#include "opencv2/opencv.hpp"

namespace slib {
    namespace smedia {
		extern inline int s2cvtype(sushort type) {
			if (type == GRAY8) return CV_8UC1;
			else if (type == GRAY16) return CV_16U;
			else if (type == RGB24) return CV_8UC3;
			else if (type == RGB32 || type == RGBA) return CV_8UC4;
			return CV_8UC1;
		}
		extern inline sushort cv2stype(int type) {
			if (type == CV_8UC1) return GRAY8;
			else if (type == CV_16U) return GRAY16;
			else if (type == CV_8UC3) return RGB24;
			else if (type == CV_8UC4) return RGBA;
			return GRAY8;
		}
		extern inline void cv2simg(cv::Mat& mat, SImage& img) {
			img.clear();
			img.setType(cv2stype(mat.type()));
			img.resize(mat.cols, mat.rows);
			img.copy(mat.ptr(0), img.size());
		}
		extern inline cv::Mat s2cvimg(SImage& img) {
			if (img.isLink()) return cv::imread(img.file().path().cstr());
			else return cv::Mat(img.height(), img.width(), s2cvtype(img.type()), img.ptr());
		}
		extern inline v2i p2vi(cv::Point& p) {
			return v2i(p.x, p.y);
		}
		extern inline v2f p2vf(cv::Point& p) {
			return v2f(p.x, p.y);
		}
		extern inline v2d p2vd(cv::Point& p) {
			return v2d(p.x, p.y);
		}
		template<typename N>
		extern inline cv::Point v2cvp(const SVector2D<N>& v) {
			return cv::Point(v.x, v.y);
		}
		/*        
        extern inline cv::Mat adjustGray16(simage *img) {
            int64_t totalpx = img->height()*img->width();
            cv::Mat mat(img->height(), img->width(), CV_8U);
            int16_t max = 0, min = 0xFFFF;
            uint16_t *imgpxs = (uint16_t *)img->ptr();
            uint8_t *matpx = &mat.data[0];
            for (int i = 0; i < totalpx; ++i) {
                *matpx = 255.0*((*imgpxs)-min)/(max-min);
                ++imgpxs; ++matpx;
            }
            return mat;
        }
        extern inline cv::Mat adjustGray16(simage *img, int min, int max) {
            int64_t totalpx = img->height()*img->width();
            cv::Mat mat(img->height(), img->width(), CV_8U);
            uint16_t *imgpxs = (uint16_t *)img->ptr();
            uint8_t *matpx = &mat.data[0];
            for (int i = 0; i < totalpx; ++i) {
                if(*imgpxs < min) *matpx = 0;
                else if(max < *imgpxs) *matpx = 255;
                else *matpx = 255.0*((*imgpxs)-min)/(max-min);
                ++imgpxs; ++matpx;
            }
            return mat;
        }
		*/
    }
}
#endif
