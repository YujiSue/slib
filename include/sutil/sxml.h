#ifndef SLIB_SXML_H
#define SLIB_SXML_H

#include "sbasic/sbasic.h"
#include "sutil/sdocument.h"
#include "smedia/spaint.h"
#include "smedia/sfigure.h"

namespace slib {
    
	namespace xml {
		constexpr sushort DEFINITION_NODE = 0x0100;
		constexpr sushort DOCTYPE_NODE = 0x0200;
		constexpr sushort DOCTYPE_PUB_NODE = 0x0210;
		constexpr sushort DOCTYPE_SYS_NODE = 0x0220;
		constexpr sushort CDATA_NODE = 0x0400;
		constexpr sushort COMMENT_NODE = 0x0800;
		constexpr sushort ENTITY_NODE = 0x1000;


		constexpr sushort SINGLE_TAG = 0x0001;
		constexpr sushort START_TAG = 0x0002;
		constexpr sushort CLOSE_TAG = 0x0004;
		constexpr sushort PAIRED_TAG = 0x0006;
		constexpr sushort HIDDEN_TAG = 0x0008;

		constexpr sushort XML = 0x0001;
		constexpr sushort HTML = 0x1000;
		constexpr sushort XHTML = 0x1001;
		constexpr sushort PLIST = 0x0011;
		constexpr sushort SVG = 0x0021;
		//constexpr sushort OOML = 0x0041;
		//constexpr sushort SOML = 0x0081;

		extern inline void encodeXML(String& str);
		extern inline void decodeXML(String& str);

	}
        
	class SLIB_DLL SXmlNode;
	class SLIB_DLL SXmlDoc;
        
	using sxnode = SClsPtr<SXmlNode, NODE_OBJ>;
	class SLIB_DLL SXmlNode : public SNode<SXmlNode, NODE_OBJ> {
		friend SXmlDoc;

	public:
		sushort type;
		String tag, content;
		SDictionary attribute;

	public:
		static sxnode plistNode(const sobj& obj);
		static sobj toPlistObj(const sxnode& node);

		static void fillSVG(SDictionary& attribute, const smedia::SBrush& brush, const char *fid);
		static void strokeSVG(SDictionary& attribute, const smedia::SStroke& stroke);
		static void txtstyleSVG(SDictionary& attribute, text_style& tattr);
		static sxnode svgNode(smedia::SCanvas* cnvs);
		static sxnode svgNode(smedia::SFigure* fig);
		

	public:
		SXmlNode();
		SXmlNode(suint t, const char* s, const char *c = nullptr);
		virtual ~SXmlNode();

		void parseTag(const char* s);
		void parse(const char* s);
		String toString() const;
	};

	using sxml = SClsPtr<SXmlDoc, DOC_OBJ>;
	class SXmlDoc : public SDocument<SXmlNode> {
		friend SXmlNode;

	protected:
		sxnode _definition, _doctype, _style, _entity;

	public:
		void _init();
		void _read(sio::SFile& file, String& tag, SXmlNode* parent);
		//void _write(sio::SFile& file, SXmlNode* node);

	public:
		SXmlDoc();
		SXmlDoc(suint t);
		SXmlDoc(const SXmlDoc& doc);
		~SXmlDoc();

		sxnode definition() const;
		//sxnode style() const;
		sxnode doctype() const;
		sxnode entity() const;

		void addToEntity(sxnode node);

		void load(const char* path);
		void save(const char* path);
		void clear();
	};

	/*
        #define sxml sptr<SXmlNode>
        
        class SLIB_DLL SXmlNode : public Node<SXmlNode> {
            friend SXmlDoc;
        public:
            static constexpr sushort DEFINITION = 0x0100;
            static constexpr sushort DOCTYPE = 0x0200;
            static constexpr sushort DOCTYPE_PUBLIC = 0x0210;
            static constexpr sushort DOCTYPE_SYSTEM = 0x0220;
            static constexpr sushort CDATA = 0x0400;
            static constexpr sushort COMMENT = 0x0800;
            
            static constexpr sushort SINGLE = 0x0001;
            static constexpr sushort PAIRED = 0x0002;
            static constexpr sushort CLOSE = 0x0004;
            static constexpr sushort HIDDEN = 0x0008;
            
            static String escapeXML(const char *s);
            static String decodeXML(const char *s);
            
        public:
            sushort type;
            String tag, content;
            sattribute attribute;
            
		public:
            void _parseTag(const char *s);
            
        public:
            SXmlNode();
            SXmlNode(const char *s);
            SXmlNode(int t, const char *s = nullptr, const char *v = nullptr);
            
            virtual ~SXmlNode();
            
            static sxml plistNode(const sobj &obj);
            static void fillSVG(sattribute &attribute, const smedia::SBrush &brush, intarray *path);
            static void strokeSVG(sattribute &attribute, const smedia::SStroke &stroke);
            static void txtstyleSVG(sattribute &attribute, const text_style &tattr);
            static sxml svgNode(smedia::SCanvas *cnvs);
            static sxml svgNode(smedia::SFigure *fig);
            static sobj toPlistObj(const sxml &node);
                        
            void parse(const char *s);
            String toString() const;
        };
        class SLIB_DLL SXmlDoc {
            friend SXmlNode;
        public:
            static constexpr sushort XML = 0x1000;
            static constexpr sushort PLIST = 0x1100;
            static constexpr sushort SVG = 0x1200;
            static constexpr sushort OOML = 0x1400;
            
            static constexpr sushort HTML = 0x4000;
            static constexpr sushort XHTML = 0x5000;
            
        protected:
            sushort _type;
            sxml _definition, _doctype, _entity;
            Array<sxml> _nodes;
            
        public:
            void _init();
            void _read(SFile &file, String &tag, SXmlNode *parent);
            void _write(SFile &file, SXmlNode *node);
            
        public:
            SXmlDoc();
            SXmlDoc(int t);
            SXmlDoc(const SXmlDoc &doc);
            ~SXmlDoc();
            
            sushort type() const;
            
            sxml definition() const;
            //sxml style() const;
            sxml doctype() const;
            sxml entity() const;
            const Array<sxml> &others() const;
            void addComment(sxml xml);
            void addCData(sxml xml);
            void setEntity(sxml xml);
            void addToEntity(sxml xml);
            
            void load(const char *path);
            void save(const char *path);
            void clear();
        };
		*/

}

#endif