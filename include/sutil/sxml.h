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
		SXmlNode(suint t, const char* s, const char* c = nullptr, std::initializer_list<kvpair<String, SObjPtr>> a = {});
		virtual ~SXmlNode();

		void parseTag(const char* s);
		void parse(const char* s);
		String toString(bool formed) const;
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

		void setEntity(sxnode node);
		void addToEntity(sxnode node);

		void load(const char* path);
		void save(const char* path);
		void clear();
	};

}

#endif