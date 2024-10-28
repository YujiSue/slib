#ifndef SLIB_SXML_H
#define SLIB_SXML_H
#include "sbasic/node.h"
#include "sio/stream.h"
#include "smedia/sfigure.h"
namespace slib {
	class SLIB_DLL SXmlNode;
	class SLIB_DLL SXml;	
	namespace sxml {
		constexpr sushort ROOT_NODE = 0x0000;

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

		extern SLIB_DLL SPointer<SXmlNode> declaration(const char* s, const Map<String, String>& attr = {});
		extern SLIB_DLL SPointer<SXmlNode> doctype();
		extern SLIB_DLL SPointer<SXmlNode> leaf(const char* s, const Map<String, String>& attr = {}, const char* c = nullptr);
		extern SLIB_DLL SPointer<SXmlNode> node(const sushort t, const char* s = nullptr, const Map<String, String>& attr = {}, const char* c = nullptr);
		extern SLIB_DLL SPointer<SXmlNode> comment(const char* s);

		extern SLIB_DLL SPointer<SXmlNode> plistNode(SObjPtr obj);
		extern SLIB_DLL SObjPtr toPlistObj(const SXmlNode& node);

		extern SLIB_DLL void setSvgTextStyle(sattribute& attribute, const TextAttribute& attr);
		extern SLIB_DLL void setSvgStroke(sattribute &attribute, const Stroke& strk);
		extern SLIB_DLL void setSvgBrush(sattribute& attribute, const SColor& brush, const String &fid);
		extern SLIB_DLL void setSvgTransform(sattribute& attribute, const Transform2D& transform);
		extern SLIB_DLL SPointer<SXmlNode> svgDeclaration(const slib::SCanvas& cnvs);
		extern SLIB_DLL SPointer<SXmlNode> svgNode(const SCanvas& cnvs);
		extern SLIB_DLL SPointer<SXmlNode> svgNode(const SFigure &fig);
		extern SLIB_DLL SFigure toSVGObj(const SXmlNode& node);

		extern SLIB_DLL ArrayIterator<char> readXmlNode(slib::ArrayIterator<char> current, const ArrayIterator<char>& end, SXmlNode& node);
		extern SLIB_DLL void writeXmlNode(slib::IOStream& ostream, const slib::SXmlNode& node, bool formed, int l);

		extern SLIB_DLL String encode(const char* s);
		extern SLIB_DLL String decode(const char* s);

	}
	class SLIB_DLL SXmlNode : public SNode<SXmlNode> {
		typedef SPointer<SXmlNode> SXmlNodeElement;
	public:
		sushort type;
		String tag;
		stringarray content;
		Map<String, String> attribute;

	public:
		SXmlNode();
		SXmlNode(const char* s, const Map<String, String>& attr = {}, const char* c = nullptr);
		SXmlNode(const sushort t, const char* s = nullptr, const Map<String, String>& attr = {}, const char* c = nullptr);
		SXmlNode(const SXmlNode& node);
		virtual ~SXmlNode();
		SXmlNode& operator=(const SXmlNode& node);

		void add(const SXmlNode& node);
		void insert(const int idx, const SXmlNode& node);
		void insertBefore(const SXmlNode& node, SXmlNode& dest);
		void insertAfter(const SXmlNode& node, SXmlNode& dest);
		bool match(const char* tag, const sattribute& attr = {}) const;
		PArrayIterator<SXmlNode> find(const char* tag, const sattribute& attr = {});
		PArrayCIterator<SXmlNode> find(const char* tag, const sattribute& attr = {}) const;
		Array<PArrayIterator<SXmlNode>> findAll(const char* tag, const sattribute& attr = {});
		Array<PArrayCIterator<SXmlNode>> findAll(const char* tag, const sattribute& attr = {}) const;
		SXmlNode &search(const char *tag, const sattribute& attr = {});
		const SXmlNode &search(const char *tag, const sattribute& attr = {}) const;
		PArray<SXmlNode> searchAll(const char *tag, const sattribute& attr = {}) const;
		SXmlNode &operator[](const char *tag);
		const SXmlNode &operator[](const char *tag) const;

		String text() const;
		void parse(const char* s);
		String toString(const char* format = nullptr) const;
	};

	class SXml : public SXmlNode {
	public:
		SXml();
		SXml(SObjPtr obj);
		SXml(const SXml& doc);
		~SXml();
		void load(const char* path);
		void save(const char* path, bool formed = false);
	};
	extern std::ostream& operator<<(std::ostream& os, const SXmlNode& node);
}

#endif