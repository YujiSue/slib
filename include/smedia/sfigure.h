#ifndef SLIB_SFIGURE_H
#define SLIB_SFIGURE_H

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
		class SLIB_DLL SCircle;
		class SLIB_DLL SEllipse;
        class SLIB_DLL SArc;
        class SLIB_DLL SPolygon;
        class SLIB_DLL SPicture;
        class SLIB_DLL SCalligraphy;
        
        #define sfig scobj<slib::smedia::SFigure, FIGURE_OBJ>
        #define spoint2d scobj<slib::smedia::SPoint2D, FIGURE_OBJ>
        #define sline2d scobj<slib::smedia::SLine2D, FIGURE_OBJ>
        #define scurve2d scobj<slib::smedia::SCurve2D, FIGURE_OBJ>
        #define spath2d scobj<slib::smedia::SPath2D, FIGURE_OBJ>
        #define srect scobj<slib::smedia::SRectangle, FIGURE_OBJ>
		#define sellipse scobj<slib::smedia::SEllipse, FIGURE_OBJ>
        #define scirc scobj<slib::smedia::SCircle, FIGURE_OBJ>
        #define sarc scobj<slib::smedia::SArc, FIGURE_OBJ>
        #define spolygon scobj<slib::smedia::SPolygon, FIGURE_OBJ>
        #define spict scobj<slib::smedia::SPicture, FIGURE_OBJ>
        #define scalligraphy scobj<slib::smedia::SCalligraphy, FIGURE_OBJ>
        
        class SLIB_DLL SFigure : public SNode<SFigure, FIGURE_OBJ> {
		protected:
			sushort _type;
			sareaf _boundary;
			sgeom::ORIGIN _origin;
			v2fvec _vertex;
			STransform2D _trans;
			SPaint _paint;
			SDictionary _attribute;

        protected:
            void _makeBoundary(v2f pt);
            void _resetBoundary();
            void _updateBoundary();

        public:
            SFigure();
            SFigure(sushort t);
            SFigure(sushort t, const SDictionary &dic);
            SFigure(const SFigure &fig);
            virtual ~SFigure();
            
            sushort type() const;
			sareaf boundary() const;
			sgeom::ORIGIN origin() const;
			size_t vcount() const;
			v2fvec& vertex();
			const v2fvec& vertex() const;
			SDictionary& attribute();
			const SDictionary& attribute() const;

			const STransform2D &transformer() const;
            const SPaint &painter() const;
			const SBrush &brush() const;
			const SStroke &stroke() const;
            
            void setOrigin(smath::sgeom::ORIGIN ori);
			void setScale(v2f v);
            void setTranslate(v2f v);
            void setSkew(v2f v);
            void setRotation(float f);
            void setReflect(subyte r);
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
            
            void expand(v2f s);
            void shift(v2f t);
            void shear(v2f s);
            void rotate(float rot);
            void reflect(subyte ref);
            void transform();
            
            void addVertex(v2f v);
            void setVertex(size_t idx, v2f v);
            void insertVertex(size_t idx, v2f v);
            void removeVertex(size_t idx);
            void clearVertex();
            
			void addFigure(sfig fig);
            void addFigure(SFigure &&fig);
            void addFigure(const SFigure &fig);
			template<class Cls>
            void addFigure(SClsPtr<Cls, FIGURE_OBJ> fig) { 
				sfig fig_(dynamic_cast<SFigure*>(fig.ptr()));
				fig_->addScope();
				SNode<SFigure, FIGURE_OBJ>::addChild(fig_);
				_resetBoundary(); _updateBoundary();
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
        
        class SLIB_DLL SCanvas : public SDocument<SFigure> {
        protected:
			v2i _size;
			sarea _frame;
			SColor _background;
			SPaint _paint;

        private:
            void loadSVG(const char *path);
            //void loadCNVS(const char *path);
            void saveSVG(const char *path);
            //void saveCNVS(const char *path);
            
        public:
            SCanvas();
            SCanvas(size_t w, size_t h, const char *name = "", const SColor & col = "white");
            SCanvas(const SCanvas &canvas);
            ~SCanvas();
            
            void load(const char *path);
            void save(const char *path);
            
            size_t width() const;
            size_t height() const;
			v2i size() const;
            sarea frame() const;
            const SColor &background() const;
            void resize(size_t w, size_t h);
            void setBackGround(const SColor &col);
            void setFrame(sarea area);
            
            void setPaint(const SPaint &paint);

			template<class... Args>
			void drawPoint(Args... args) {
				addFigure(spoint2d(args...));
			}
			template<class... Args>
			void drawLine(Args... args) {
				addFigure(sline2d(args...));
			}
			template<class... Args>
            void drawPath(Args... args) {
				addFigure(spath2d(args...));
			}
			template<class... Args>
            void drawRect(Args... args) {
				addFigure(srect(args...));
			}
			template<class... Args>
			void drawPolygon(Args... args) {
				addFigure(spolygon(args...));
			}
			template<class... Args>
			void drawEllipse(Args... args) {
				addFigure(sellipse(args...));
			}
			template<class... Args>
			void drawCircle(Args... args) {
				addFigure(scircle(args...));
			}
			template<class... Args>
			void drawArc(Args... args) {
				addFigure(sarc(args...));
			}
			template<class... Args>
			void drawPict(Args... args) {
				addFigure(spict(args...));
			}
			template<class... Args>
			void drawText(Args... args) {
				addFigure(scalligraphy(args...));
			}
			void addFigure(sfig fig);
            
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
            text_style _attribute;
            //spath _path;
            
        public:
            SCalligraphy();
            SCalligraphy(float x, float y, const char *s);
            SCalligraphy(const SCalligraphy &c);
            ~SCalligraphy();
            
            const char *text() const;
			text_style& style();
            const text_style &style() const;
            
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
