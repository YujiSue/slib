#ifndef SLIB_CV_ADAPTOR_H
#define SLIB_CV_ADAPTOR_H

#include "smedia/simage.h"
#include "smedia/sfigure.h"
#include "opencv/cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

namespace slib {
    namespace smedia {
		extern inline int toCVType(sushort type) {
			if (type == scolor::GRAY8) return CV_8UC1;
			else if (type == scolor::GRAY16) return CV_16U;
			else if (type == scolor::RGB) return CV_8UC3;
			else if (type == scolor::RGBA) return CV_8UC4;
			return CV_8UC1;
		}
		extern inline sushort toSType(int type) {
			if (type == CV_8UC1) return scolor::GRAY8;
			else if (type == CV_16U) return scolor::GRAY16;
			else if (type == CV_8UC3) return scolor::RGB;
			else if (type == CV_8UC4) return scolor::RGBA;
			return scolor::GRAY8;
		}
		template<typename T>
		extern inline smath::Vector2D<T> toSVec(const cv::Point_<T>& p) { return smath::Vector2D<T>(p.x, p.y); }
		template<typename T>
		extern inline cv::Point_<T> toCVPoint(const smath::Vector2D<T>& v) { return cv::Point_<T>(v.x, v.y); }
		template<typename T>
		extern inline cv::Point toCVSize(const Area<T>& a) { return cv::Size(a.width, a.height); }
		template<typename T>
		extern inline cv::Rect toCVRect(const Area<T>& a) {
			return cv::Rect(a.ori_x, a.ori_y, a.width, a.height);
		}
		extern inline cv::Scalar toCVColor(const SColor& col) {
			return cv::Scalar(col[0], col[1], col[2], col[3]);
		}
		template<typename T>
		extern inline cv::InputArrayOfArrays toCVArray(smath::Vector<smath::Vector2D<T>>& vertex) {
			std::vector<cv::Point_<T>> vec(vertex.size());
			sfor2(vec, vertex) $_1 = toCVPoint<T>($_2);
			return cv::InputArrayOfArrays(vec);
		}
		extern inline void toSImg(cv::Mat& mat, SImage& img) {
			

			img.clear();
			img.setType(toSType(mat.type()));
			img.resize(mat.cols, mat.rows);
			img.copy(mat.ptr(0), img.size());
		}
		extern inline cv::Mat toCVMat(SImage& img) {
			if (img.isLink()) return cv::imread(img.file().path().cstr());
			else {
				img.addScope();
				return cv::Mat(img.height(), img.width(), toCVType(img.type()), img.ptr());
			}
		}
		extern inline cv::Mat toCVMat(SCanvas& cnvs) {
			cv::Mat mat(cnvs.height(), cnvs.width(), CV_8UC4);
			mat = toCVColor(cnvs.background());
			sforeach(cnvs.root()) {
				if (E_.type() && sshape::POINT) {
					

				}
				else if (E_.type() == sshape::LINE) {
					cv::line(mat, toCVPoint(E_->vertex()[0]), toCVPoint(E_->vertex()[1]), toCVColor(E_->stroke().color), E_->stroke().width);
				}
				else if (E_.type() == sshape::RECTANGLE) {
					cv::rectangle(mat, toCVRect(E_->boundary()), toCVColor(E_->brush().color), -1);
					cv::rectangle(mat, toCVRect(E_->boundary()), toCVColor(E_->stroke().color), E_->stroke().width);
				}
				else if (E_.type() == sshape::ELLIPSE || E_.type() == sshape::CIRCLE) {
					cv::ellipse(mat, toCVPoint(E_->center()), toCVSize(E_->boundary()), 0.0, 0.0, 360.0, toCVColor(E_->brush().color), -1);
					cv::ellipse(mat, toCVPoint(E_->center()), toCVSize(E_->boundary()), 0.0, 0.0, 360.0, toCVColor(E_->stroke().color), E_->stroke().width);
				}
				else if (E_.type() == sshape::POLYGON) {
					auto cvpts = toCVArray(E_->vertex());
					cv::fillPoly(mat, cvpts, toCVColor(E_->brush().color));
					cv::polylines(mat, cvpts, true, toCVColor(E_->stroke().color), E_->stroke().width);
				}

			}
			return mat;
		}
		extern inline void displayCV(SImage& img) {
			cv::Mat mat = toCVMat(img);
			cv::namedWindow("Image");
			cv::imshow("Image", mat);
			while (true) {
				if(cv::waitKeyEx(10) == 27) break;
			}
		}
		extern inline void displayCV(SCanvas& cnvs) {
			cv::Mat mat = toCVMat(cnvs);
			cv::namedWindow("Canvas");
			cv::imshow("Canvas", mat);
			while (true) {
				if (cv::waitKeyEx(10) == 27) break;
			}
		}
		extern inline void displayCV(sobj& obj) {
			if (obj.isImg()) displayCV(obj.image());
			else if (obj.isCnvs()) displayCV(obj.canvas());
		}
    }
}
#endif
