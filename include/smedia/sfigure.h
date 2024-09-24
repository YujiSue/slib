#ifndef SLIB_SFIGURE_H
#define SLIB_SFIGURE_H
#include "sbasic/node.h"
#include "smath/geom.h"
#include "sobj/stext.h"
#include "sio/sfile.h"
#include "smedia/sgraphic.h"
#include "smedia/simage.h"
namespace slib {
    class SLIB_DLL SFigure;
    class SLIB_DLL FigureElement {
        friend SFigure;
    public:
        String figid;
        Transform2D transform;
        Area<float> boundary;
        Stroke stroke;
        SColor brush;
        SDictionary attribute;

    public:
        FigureElement();
        virtual ~FigureElement();

        virtual size_t num() const;
        virtual void setVertex(int idx, const smath::Vector2D<float>& pt);
        virtual void expand(const smath::Vector2D<float>& vec);
        virtual void translate(const smath::Vector2D<float>& vec);
        virtual void rotate(const float rot);
        virtual void shear(const float shear, smath::AXIS ax);
        virtual void reflect(smath::AXIS ax);
        virtual void updateBoundary();

        virtual String getClass() const;
    };
    class SLIB_DLL SPoint : public sgeom::Point<float, 2>, public FigureElement {
        friend SFigure;
    public:
        SPoint();
        SPoint(const float x, const float y, const SColor& col = scolor::BLACK);
        SPoint(const sgeom::Point<float, 2>& pt, const SColor& col = scolor::BLACK);
        SPoint(const SPoint& pt);
        ~SPoint();
        
        size_t num() const;

        void expand(const smath::Vector2D<float>& vec);
        void translate(const smath::Vector2D<float>& vec);
        void rotate(const float rot);
        void shear(const float shear, smath::AXIS ax);
        void reflect(smath::AXIS ax);
        void updateBoundary();

        String getClass() const;
    };
    class SLIB_DLL SLine : public sgeom::Line<float, 2>, public FigureElement {
        friend SFigure;
    public:
        SLine();
        SLine(const float x1, const float y1, const float x2, const float y2, const Stroke& strk = Stroke());
        SLine(const smath::Vector2D<float>& p1, const smath::Vector2D<float>& p2);
        SLine(const smath::Vector2D<float>& p1, const smath::Vector2D<float>& p2, const Stroke& strk);
        SLine(const SLine& line);
        ~SLine();
        
        size_t num() const;

        void expand(const smath::Vector2D<float>& vec);
        void translate(const smath::Vector2D<float>& vec);
        void rotate(const float rot);
        void shear(const float shear, smath::AXIS ax);
        void reflect(smath::AXIS ax);
        void updateBoundary();

        String getClass() const;

    };
    class SLIB_DLL SCurve : public sgeom::Curve<float, 2>, public FigureElement {
        friend SFigure;
    public:
        SCurve();
        SCurve(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4, const Stroke& strk = Stroke());
        SCurve(const smath::Vector2D<float>& p1, const smath::Vector2D<float>& p2, const smath::Vector2D<float>& p3, const smath::Vector2D<float>& p4, const Stroke& strk = Stroke());
        SCurve(const SCurve& curve);
        ~SCurve();

        size_t num() const;

        void expand(const smath::Vector2D<float>& vec);
        void translate(const smath::Vector2D<float>& vec);
        void rotate(const float rot);
        void shear(const float shear, smath::AXIS ax);
        void reflect(smath::AXIS ax);
        void updateBoundary();

        String getClass() const;
    };
    class SLIB_DLL SPath : public sgeom::Path<float, 2>, public FigureElement {
        friend SFigure;
    public:
        SPath();
        SPath(const sgeom::Path<float, 2> &path);
        SPath(const SPath& path);
        ~SPath();

        size_t num() const;

        void expand(const smath::Vector2D<float>& vec);
        void translate(const smath::Vector2D<float>& vec);
        void rotate(const float rot);
        void shear(const float shear, smath::AXIS ax);
        void reflect(smath::AXIS ax);
        void updateBoundary();

        String getClass() const;
    };
    class SLIB_DLL SRectangle : public sgeom::Rectangle<float>, public FigureElement {
        friend SFigure;
    public:
        SRectangle();
        SRectangle(const float p1, const float p2, const float p3, const float p4);
        SRectangle(const Area<float>& area);
        SRectangle(const sgeom::Rectangle<float>& rect);
        SRectangle(const SRectangle& rect);
        ~SRectangle();

        size_t num() const;

        void expand(const smath::Vector2D<float>& vec);
        void translate(const smath::Vector2D<float>& vec);
        void rotate(const float rot);
        void shear(const float shear, smath::AXIS ax);
        void reflect(smath::AXIS ax);
        void updateBoundary();

        String getClass() const;
    };
    class SLIB_DLL SEllipse : public sgeom::Ellipse<float>, public FigureElement {
    public:
        SEllipse();
        SEllipse(const float p1, const float p2, const float p3, const float p4);
        SEllipse(const Area<float>& area);
        SEllipse(const SEllipse& elps);
        ~SEllipse();

        size_t num() const;

        void expand(const smath::Vector2D<float>& vec);
        void translate(const smath::Vector2D<float>& vec);
        void rotate(const float rot);
        void shear(const float shear, smath::AXIS ax);
        void reflect(smath::AXIS ax);
        void updateBoundary();

        String getClass() const;
    };
    class SLIB_DLL SArc : public sgeom::Arc<float>, public FigureElement {
    public:
        SArc();
        SArc(const SArc& arc);
        ~SArc();

        size_t num() const;

        void expand(const smath::Vector2D<float>& vec);
        void translate(const smath::Vector2D<float>& vec);
        void rotate(const float rot);
        void shear(const float shear, smath::AXIS ax);
        void reflect(smath::AXIS ax);
        void updateBoundary();

        String getClass() const;
    };
    class SLIB_DLL SPolygon : public sgeom::Polygon<float>, public FigureElement {
    public:
        SPolygon();
        SPolygon(const size_t sz);
        SPolygon(std::initializer_list<smath::Vector2D<float>> li);
        SPolygon(const SPolygon& poly);
        ~SPolygon();

        size_t num() const;
        void setVertex(int idx, const smath::Vector2D<float>& pt);
        void expand(const smath::Vector2D<float>& vec);
        void translate(const smath::Vector2D<float>& vec);
        void rotate(const float rot);
        void shear(const float shear, smath::AXIS ax);
        void reflect(smath::AXIS ax);
        void updateBoundary();

        String getClass() const;
    };
    class SLIB_DLL SPicture : public sgeom::Rectangle<float>, public FigureElement {
        friend SFigure;
    protected:
        SImage _image;
    public:
        SPicture();
        
        ~SPicture();
        
        bool loaded() const;
        String url() const;
        SImage& image();
        const SImage& image() const;

        size_t num() const;

        void expand(const smath::Vector2D<float>& vec);
        void translate(const smath::Vector2D<float>& vec);
        void rotate(const float rot);
        void shear(const float shear, smath::AXIS ax);
        void reflect(smath::AXIS ax);
        void updateBoundary();

        String getClass() const;
    };
    class SLIB_DLL SCaption : public sgeom::Rectangle<float>, public FigureElement {
        friend SFigure;
    protected:
        String _text;
        TextAttribute _attribute;
    public:
        SCaption();
        SCaption(const float x, const float y, const char* s, const TextAttribute &attr = TextAttribute());
        SCaption(const SCaption& c);
        ~SCaption();

        const char* text() const;
        const String& string() const;
        const TextAttribute& attribute() const;
        void setText(const char* s);
        void setFont(const char* font, float size);
        void setStyle(suint type);
        void setColor(const Color& col);
        smath::Vector2D<float> position() const;

        size_t num() const;

        void expand(const smath::Vector2D<float>& vec);
        void translate(const smath::Vector2D<float>& vec);
        void rotate(const float rot);
        void shear(const float shear, smath::AXIS ax);
        void reflect(smath::AXIS ax);
        void updateBoundary();

        String getClass() const;
    };
    
    class SLIB_DLL SFigure : public SNode<SFigure>, public SObject {
    protected:
        SPointer<FigureElement> _element;

    protected:
        void _update();
        void _setParent(SFigure* parent);

    public:
        SFigure();
        SFigure(const SPoint& pt);
        SFigure(const SLine& ln);
        SFigure(const SCurve& crv);
        SFigure(const SPath& path);
        SFigure(const SRectangle& rect);
        SFigure(const SEllipse& el);
        SFigure(const SArc& arc);
        SFigure(const SPolygon& poly);
        SFigure(const SPicture& pict);
        SFigure(const SCaption& cap);
        SFigure(FigureElement *felem);
        SFigure(const SFigure & fig);
        ~SFigure();

        const String& figId() const;
        const Area<float>& boundary() const;
        const Transform2D& transform() const;
        const Stroke &stroke() const;
        const SColor &color() const;
        const SDictionary& attribute() const;

        void setId(const char* s);
        void setStroke(const Stroke& strk);
        void setStrokeStyle(const sint style);
        void setStrokeWidth(const float width);
        void setStrokeColor(const SColor& col);
        void setFillColor(const SColor& col);

        //void setScale(const smath::Vector2D<float>& vec);
        //void setShift(const smath::Vector2D<float>& vec);
        void setAttribute(const char* s, const SObjPtr& obj);

        virtual void setVertex(int idx, const smath::Vector2D<float>& pt);
        virtual void expand(const smath::Vector2D<float>& vec);
        virtual void translate(const smath::Vector2D<float>& vec);
        virtual void rotate(const float rot);
        virtual void shear(const float shear, smath::AXIS ax);
        virtual void reflect(smath::AXIS ax);
        bool isAbstract() const;
        bool isGroup() const;
        bool isPoint() const;
        bool isLine() const;
        bool isCurve() const;
        bool isPath() const;
        bool isRect() const;
        bool isEllipse() const;
        bool isArc() const;
        bool isPolygon() const;
        bool isPict() const;
        bool isCaption() const;

        bool overlap(const SFigure& fig) const;
        bool include(const SFigure& fig) const;

        SFigure &draw(const SFigure& fig);

        SPoint& point();
        const SPoint& point() const;
        SLine& line();
        const SLine& line() const;
        SCurve& curve();
        const SCurve& curve() const;
        SPath& path();
        const SPath& path() const;
        SRectangle& rectangle();
        const SRectangle& rectangle() const;
        SEllipse& ellipse();
        const SEllipse& ellipse() const;
        SArc& arc();
        const SArc& arc() const;
        SPolygon& polygon();
        const SPolygon& polygon() const;
        SPicture& pict();
        const SPicture& pict() const;
        SCaption& caption();
        const SCaption& caption() const;


        operator Point2D<float>& ();
        operator const Point2D<float>& () const;
        operator SPoint& ();
        operator const SPoint& () const;



        virtual String getClass() const;
        virtual String toString(const char* format = nullptr) const;
        virtual SObject* clone() const;
    };

    class SLIB_DLL SCanvas : public SFigure {
        Area<size_t> _area;
    public:
        SCanvas();
        SCanvas(const size_t width, const size_t height);
        SCanvas(const SCanvas &cnvs);
        ~SCanvas();

        const Area<size_t>& frame() const;
        void resize(const size_t width, const size_t height);
        void load(const char* s);
        void save(const char* s);

        String getClass() const;
        String toString(const char* format = nullptr) const;
        SObject* clone() const;
    };

    /*
        
        class SLIB_DLL SFigure : public SNode<SFigure> {
		protected:
			sushort _type;
			sareaf _boundary;
			//sgeom::ORIGIN _origin;
			//v2fvec _vertex;
			Transform2D _trans;
			SPaint _paint;
			SDictionary _attribute;

        protected:
            //void _makeBoundary(v2f pt);
            void _resetBoundary();
            void _updateBoundary();

        public:
            SFigure();
            SFigure(sushort t);
            SFigure(sushort t, const SDictionary &dic);
            SFigure(const SFigure &fig);
            virtual ~SFigure();
            
			virtual sushort type() const;
			virtual sareaf boundary() const;
			//virtual sgeom::ORIGIN origin() const;
			virtual size_t vcount() const;
			//virtual v2fvec& vertex();
			//virtual const v2fvec& vertex() const;
			virtual SDictionary& attribute();
			virtual const SDictionary& attribute() const;

			//virtual const STransform2D &transformer() const;
			virtual const SPaint &painter() const;
			virtual const SBrush &brush() const;
			virtual const SStroke &stroke() const;
            
			//virtual void setOrigin(sgeom::ORIGIN ori);
			//virtual void setScale(v2f v);
			//virtual void setTranslate(v2f v);
			//virtual void setSkew(v2f v);
			virtual void setRotation(float f);
			virtual void setReflect(subyte r);
			virtual void setStrokeType(sushort t);
			virtual void setStrokeWidth(float w);
			virtual void setStrokeColor(const SColor &c);
			virtual void setStrokeInterval(const intarray &a);
			virtual void setStroke(const SStroke &s);
			virtual void setFillColor(const SColor &c);
			//virtual void setFillColor(const SGradient &g);
			virtual void setBrush(const SBrush &f);
			virtual void setPaint(const SPaint &p);
			virtual void setAttribute(const SDictionary &dic);
			
			//virtual void expand(v2f s);
            //virtual void shift(v2f t);
			//virtual void shear(v2f s);
			virtual void rotate(float rot);
			virtual void reflect(subyte ref);
			void transform();
            
            //void addVertex(v2f v);
            //void setVertex(size_t idx, v2f v);
            //void insertVertex(size_t idx, v2f v);
            void removeVertex(size_t idx);
            void clearVertex();
            
			//void addFigure(sfig fig);
            void addFigure(SFigure &&fig);
            void addFigure(const SFigure &fig);

            virtual v2f center() const;
            virtual float length() const;
            virtual float area() const;
            virtual bool include(v2f v) const;
            virtual bool cross(sfig fig) const;
            
            template<class Cls>
			bool cross(Cls c) const {
				return _boundary.overlap(c->boundary());
			}
			
            SPoint& point();
			SLine& line();
			SCurve& curve();
			SPath& path();
			SRectangle &rect();
			SPolygon& polygon();
			SEllipse& ellipse();
			SCircle& circle();
			SArc& arc();
			SPicture& pict();
			SCalligraphy& calligraph();
			SFigureGroup &group();


            virtual String getClass() const;
            virtual String toString() const;
            virtual SObject *clone() const;
        };
    */
}
#endif
