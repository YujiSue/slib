#ifndef __SCANVAS_H__
#define __SCANVAS_H__

#include "sutil/sutil.h"
#include "sobj/stext.h"
#include "sobj/snode.h"
#include "sutil/sxml.h"
#include "sio/sfile.h"
#include "smedia/sgraphic.h"
#include "smedia/spaint.h"
#include "smedia/simage.h"

namespace slib {
    using namespace smath;
    using namespace sio;
    
    namespace smedia {
        class SLIB_DLL SFigure;
        class SLIB_DLL SCanvas;
        
        class SLIB_DLL SPoint2D;
        class SLIB_DLL SLine2D;
        class SLIB_DLL SCurve2D;
        class SLIB_DLL SPath2D;
        class SLIB_DLL SRectangle;
        class SLIB_DLL SEllipse;
        class SLIB_DLL SArc;
        class SLIB_DLL SPolygon;
        class SLIB_DLL SPicture;
        class SLIB_DLL SCalligraphy;
        
        #define sfig scobj<slib::smedia::SFigure, FIGURE_OBJ>
        #define spt scobj<slib::smedia::SPoint2D, FIGURE_OBJ>
        #define sline scobj<slib::smedia::SLine2D, FIGURE_OBJ>
        #define scurve scobj<slib::smedia::SCurve2D, FIGURE_OBJ>
        #define spath scobj<slib::smedia::SPath2D, FIGURE_OBJ>
        #define srect scobj<slib::smedia::SRectangle, FIGURE_OBJ>
        #define sellipse scobj<slib::smedia::SEllipse, FIGURE_OBJ>
        #define sarc scobj<slib::smedia::SArc, FIGURE_OBJ>
        #define spolygon scobj<slib::smedia::SPolygon, FIGURE_OBJ>
        #define spict scobj<slib::smedia::SPicture, FIGURE_OBJ>
        #define scalligraphy scobj<slib::smedia::SCalligraphy, FIGURE_OBJ>
        
        class SLIB_DLL SFigure : public SNode<SFigure, FIGURE_OBJ> {
        
        protected:
            sushort _type;
            String _id;
            strans2d _trans;
            SPaint _paint;
            sareaf _boundary;
            v2f _origin;
            v2fvec _vertex;
            
        protected:
            void _makeBoundary(v2f &point);
            void _resetBoundary();
            void _updateBoundary();
            
        public:
            SFigure();
            SFigure(int t);
            SFigure(int t, const SDictionary &dic);
            SFigure(const SFigure &fig);
            virtual ~SFigure();
            
            sushort type() const;
            const String &name() const;
            strans2d transformer() const;
            SPaint &painter();
            const SPaint &painter() const;
            const SBrush &brush() const;
            const SStroke &stroke() const;
            sareaf boundary() const;
            size_t vnum() const;
			v2fvec &vertex();
            const v2fvec &vertex() const;
            
            void setName(const char *s);
            void setOrigin(smath::sgeom::ORIGIN ori);
            void setOrigin(v2f ori);
            void setScale(v2f v);
            void setTranslate(v2f v);
            void setSkew(v2f v);
            void setRot(float f);
            void setReflect(uint8_t i);
            void setTransform(const STransform2D &trans);
            void setStrokeType(sushort t);
            void setStrokeWidth(float w);
            void setStrokeColor(const SColor &c);
            void setStrokeInterval(const intarray &a);
            void setStroke(const SStroke &s);
            void setFillColor(const SColor &c);
            void setFillColor(const SGradient &g);
            void setBrush(const SBrush &f);
            void setPaint(const SPaint &p);
            void setAttribute(const SDictionary &dic);
            
            void expand(v2f v);
            void shift(v2f v);
            void shear(v2f v);
            void rotate(float f);
            void reflect(uint8_t i);
            void transform();
            
            void addVertex(v2f v);
            void setVertex(size_t idx, v2f v);
            void insertVertex(size_t idx, v2f v);
            void removeVertex(size_t idx);
            void clearVertex();
            
            void addFigure(SFigure *fig);
            void addFigure(sfig &&fig);
            void addFigure(sfig &fig);
            template<class Cls>
            void addFigure(scobj<Cls, FIGURE_OBJ> &&fig) {
                if (_children.empty()) _boundary = fig->boundary();
                else _boundary.merge(fig->boundary());
                SNode<SFigure, FIGURE_OBJ>::add(dynamic_cast<SFigure *>(fig.ptr()));
                fig.discard(); _updateBoundary();
            }
            template<class Cls>
            void addFigure(scobj<Cls, FIGURE_OBJ> &fig) {
                if (_children.empty()) _boundary = fig->boundary();
                else _boundary.merge(fig->boundary());
                fig.share(); SNode<SFigure, FIGURE_OBJ>::add(dynamic_cast<SFigure *>(fig.ptr()));
                _updateBoundary();
            }
            
            virtual slib::smath::v2f center() const;
            virtual double length() const;
            virtual double area() const;
            
            virtual bool include(v2f v) const;
            virtual bool cross(sfig fig) const;
            
            virtual String getClass() const;
            virtual String toString() const;
            virtual SObject *clone() const;
        };
        
#define scnvs scobj<slib::smedia::SCanvas, CANVAS_OBJ>
        
        class SLIB_DLL SCanvas : public SFigure {
        protected:
            SColor _background;
            sarea _frame;
            
        private:
            void loadSVG(const char *path);
            void loadCNVS(const char *path);
            void saveSVG(const char *path);
            void saveCNVS(const char *path);
            
        public:
            SCanvas();
            SCanvas(size_t w, size_t h, const char *name = "", SColor col = "white");
            SCanvas(const SCanvas &canvas);
            ~SCanvas();
            
            void load(const char *path);
            void save(const char *path);
            
            size_t width() const;
            size_t height() const;
            sarea frame() const;
            const SColor &background() const;
            void resize(size_t w, size_t h);
            void setBackGround(const SColor &col);
            void setFrame(sarea area);
            
            void setPaint(const SPaint &paint);
            void drawPoint(v2f pos);
            void drawLine(v2f init, v2f end);
            void drawPath();
            void drawRect(float x, float y, float w, float h);
            void drawRect(sareaf area);
            void drawPolygon(const v2fvec &v);
            void drawEllipse(float x, float y, float r);
            void drawEllipse(float x, float y, float w, float h);
            void drawEllipse(sareaf area);
            void drawArc();
            void drawPict(float x, float y, const char *s);
            void drawText(float x, float y, const char *s);
            
            void addPoint(spt pt);
            void addLine(sline ln);
            void addPath(spath path);
            void addRect(srect rect);
            void addPolygon(spolygon poly);
            void addEllipse(sellipse elps);
            void addArc(sarc arc);
            void addPict(spict pic);
            void addText(scalligraphy txt);
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
        
        class SLIB_DLL SPoint2D : public SFigure {
        protected:
            sushort _shape;
            float _size;
            
        public:
            SPoint2D();
            SPoint2D(v2f v, sushort s = sshape::CIRCLE, float f = 1.0f, const SPaint &paint = SPaint());
            SPoint2D(v2f v, const SDictionary &dic);
            SPoint2D(const SPoint2D &p);
            ~SPoint2D();
            
            sushort shape() const;
            float size() const;
            
            void setShape(sushort s);
            void setSize(float f);
            
            slib::smath::v2f center() const;
            double length() const;
            double area() const;
            
            bool include(v2f v) const;
            bool cross(sfig fig) const;
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
        
        class SLIB_DLL SLine2D : public SFigure {
        public:
            SLine2D();
            SLine2D(v2f p1, v2f p2, const SStroke &stroke = SStroke());
            SLine2D(v2f p1, v2f p2, const SDictionary &dic);
            SLine2D(const SLine2D &l);
            ~SLine2D();
            
            void setStraightLine();
            void setDotLine(intarray &i);
            void setBrokenLine(intarray &i);
            
            slib::smath::v2f center() const;
            double length() const;
            double area() const;
            
            bool include(v2f v) const;
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
        class SLIB_DLL SCurve2D : public SFigure {
            /*
             BEZIER_2
             BEZIER_3
             B_SPLINE
             */
            
        public:
            SCurve2D();
            SCurve2D(const v2fvec &v, const SPaint &p = SPaint());
            SCurve2D(const v2fvec &v, const SDictionary &dic);
            SCurve2D(const SCurve2D &c);
            ~SCurve2D();
            
            //slib::smath::v2f center() const;
            //double length() const;
            //double area() const;
            
            //bool include(float x, float y) const;
            //bool cross(sfig fig) const;
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
        
        class SLIB_DLL SPath2D : public SFigure {
        protected:
            bool _closed;
            
        public:
            SPath2D();
            SPath2D(const Array<sfig> &array, const SStroke &s = SStroke());
            SPath2D(const Array<sfig> &array, const SDictionary &dic);
            SPath2D(const SPath2D &p);
            ~SPath2D();
            
            bool isClosed() const;
            void setClosed(bool b);
            
            //slib::smath::v2f center() const;
            //double length() const;
            //double area() const;
            
            //bool include(float x, float y) const;
            //bool cross(sfig fig) const;
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
        
        class SLIB_DLL SRectangle : public SFigure {
        public:
            SRectangle();
            SRectangle(float x, float y, float w, float h, const SPaint &paint = SPaint());
            SRectangle(float x, float y, float w, float h, const SDictionary &dic);
            SRectangle(const SRectangle &sr);
            ~SRectangle();
            
            float width() const;
            float height() const;
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
        
        class SLIB_DLL SEllipse : public SRectangle {
        public:
            SEllipse();
            SEllipse(float x, float y, float rad, const SPaint &paint = SPaint());
            SEllipse(float x, float y, float rad, const SDictionary &dic);
            SEllipse(float x, float y, float hrad, float vrad, const SPaint &paint = SPaint());
            SEllipse(float x, float y, float hrad, float vrad, const SDictionary &dic);
            SEllipse(const SRectangle &rect);
            SEllipse(const SEllipse &elps);
            ~SEllipse();
            
            svec2d<v2f> focus() const;
            double length() const;
            double area() const;
            bool include(v2f v) const;
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
        
        class SLIB_DLL SArc : public SEllipse {
        protected:
            v2f _phase;
            smath::ROTATION _dir;
        public:
            SArc();
            SArc(float x, float y, float rad, float init, float end,
                 smath::ROTATION r, const SPaint &paint = SPaint());
            SArc(float x, float y, float rad, float init, float end,
                 smath::ROTATION r, const SDictionary &dic);
            SArc(float x, float y, float hrad, float vrad, float init, float end,
                 smath::ROTATION r, const SPaint &paint = SPaint());
            SArc(float x, float y, float hrad, float vrad, float init, float end,
                 smath::ROTATION r, const SDictionary &dic);
            SArc(const SArc &arc);
            ~SArc();
            
            v2f phase() const;
            smath::ROTATION direction() const;
            void setPhase(v2f ph, smath::ROTATION r = CCW);
            
            //double length() const;
            //double area() const;
            //bool include(v2f v) const;
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
        
        class SLIB_DLL SPolygon : public SFigure {
        public:
            SPolygon();
            SPolygon(size_t n);
            SPolygon(const v2fvec &v, const SPaint &paint = SPaint());
            SPolygon(const v2fvec &v, const SDictionary &dic);
            SPolygon(const SPolygon &poly);
            ~SPolygon();
            
            slib::smath::v2f center() const;
            double length() const;
            double area() const;
            //bool include(v2f v) const;
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;;
        };
        
        class SLIB_DLL SPicture : public SRectangle {
        protected:
            SImage _image;
            
        public:
            SPicture();
            SPicture(float w, float h, const char *s);
            SPicture(const SPicture &pict);
            ~SPicture();
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
        
        class SLIB_DLL SCalligraphy : public SFigure {
        protected:
            String _text;
            STextStyle _attribute;
            //spath _path;
            
        public:
            SCalligraphy();
            SCalligraphy(float x, float y, const char *s);
            SCalligraphy(const SCalligraphy &c);
            ~SCalligraphy();
            
            const char *text() const;
            const STextStyle &style() const;
            
            void setText(const char *s);
            void setFont(const char *font, float size);
            void setStyle(sushort type, const char *font = "Arial", float size = 10.0,
                          SColor col = "black", SColor bg = "clear");
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
        };
    }
}

#endif
