#ifndef SLIB_SHTML_H
#define SLIB_SHTML_H

#include "sutil/sxml.h"
//#include "smedia/spaint.h"
#include "smedia/sfigure.h"

namespace slib {
	class SLIB_DLL SXmlNode;
	class SLIB_DLL SXml;	
	namespace sxml {
		constexpr sushort DECLARATION_NODE = 0x0100;
		constexpr sushort INFORMATION_NODE = 0x0110;
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

		extern SLIB_DLL SObjPtr toPlistObj(const SXmlNode& node);
		extern SLIB_DLL ArrayIterator<char> readXmlNode(slib::ArrayIterator<char> current, const ArrayIterator<char>& end, SXmlNode& node);
		extern SLIB_DLL void writeXmlNode(slib::IOStream& ostream, const slib::SXmlNode& node, bool formed, int l);

		extern String encode(const char* s);
		extern String decode(const char* s);

	}
        
//	using sxmlelement = Pointer<SXmlNode>;
	class SLIB_DLL SXmlNode : public Node<SXmlNode> {
		friend SXml;
		typedef Node<SXmlNode> xmlnode;

	public:
		sushort type;
		String tag;
		stringarray content;
		sattribute attribute;

	public:
		static SXmlNode plistElement(const SObjPtr& obj);
		static SXmlNode svgElement(const SFigure& fig);
		//static sobj toPlistObj(const sxnode& node);

		//static void fillSVG(SDictionary& attribute, const smedia::SBrush& brush, const char *fid);
		//static void strokeSVG(SDictionary& attribute, const smedia::SStroke& stroke);
		//static void txtstyleSVG(SDictionary& attribute, text_style& tattr);
		//static sxnode svgNode(smedia::SCanvas* cnvs);
		//static sxnode svgNode(smedia::SFigure* fig);

	public:
		SXmlNode();
		SXmlNode(const char *s, const char* c = nullptr);
		SXmlNode(sushort t, const char* s, const char* c = nullptr);
		SXmlNode(const SXmlNode& node);
		SXmlNode& operator=(const SXmlNode& node);

		virtual ~SXmlNode();

		
		SXmlNode *find(const char *tag);

		Array<ArrayIterator<SXmlNode>> findAll(const char* tag);
		
		String text() const;

		void parse(const char* s);
		String toString(const char *format = nullptr) const;
	};

	class SXml {
		friend SXmlNode;

	protected:
		SXmlNode _root;

	public:
		static SXml plist(const SObjPtr& obj);
		static SXml svg(const SCanvas& cnvs);

		void _init();
		void _read(SFile& file, String& tag, SXmlNode* parent);
		//void _write(sio::SFile& file, SXmlNode* node);

	public:
		SXml();


		SXml(suint t);
		SXml(const SXml& doc);
		~SXml();


		size_t size() const;
		SXmlNode& operator[] (int i);
		SXmlNode& operator[] (const char* tag);

		ArrayIterator<SPointer<SXmlNode>> begin();
		ArrayCIterator<SPointer<SXmlNode>> begin() const;
		ArrayIterator<SPointer<SXmlNode>> end();
		ArrayCIterator<SPointer<SXmlNode>> end() const;

		void load(const char* path);
		void save(const char* path, bool formed = false);
		void clear();
	};
	extern std::ostream& operator<<(std::ostream& os, const SXmlNode& node);
}

#endif