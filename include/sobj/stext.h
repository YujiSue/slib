#ifndef SLIB_STEXT_H
#define SLIB_STEXT_H
#include "sbasic/parray.h"
#include "sobj/sstring.h"
#include "sobj/sdict.h"
#include "smedia/sgraphic.h"

namespace slib {
	namespace stxt {
		extern SLIB_DLL String wrap(const String& str, const size_t row, const size_t offset, const char* breaker = nullptr, bool forced = false);
	}
	class SLIB_DLL SDictionary;
	class SLIB_DLL SText;
	class SLIB_DLL TextAttribute : public Range<int> {
	public:
		String font;
		float size;
		suint style;
		Color color, background;

	public:
		TextAttribute();
		TextAttribute(const SDictionary &dict);
		TextAttribute(const TextAttribute &attr);
		~TextAttribute();
		TextAttribute& operator=(const TextAttribute& attr);
	};

	class SLIB_DLL TextAnnotation {
		friend SText;
		
	private:
		PArray<TextAttribute> _attributes;

	public:
		TextAnnotation();
		virtual ~TextAnnotation();

		size_t count() const;
		bool annotated(const int pos) const;
		bool annotated(const srange& rng) const;

		PArray<TextAttribute> attributes(const srange& rng) const;
		TextAttribute& operator[](const int i);
		const TextAttribute& operator[](const int i) const;
		PArrayIterator<TextAttribute> begin();
		PArrayCIterator<TextAttribute> begin() const;
		PArrayIterator<TextAttribute> end();
		PArrayCIterator<TextAttribute> end() const;
		
		void merge(const srange& rng, const TextAttribute&c);		
		void erase(const srange& rng);

		void insert(const int pos, const size_t sz);
		void remove(const int pos, const size_t sz);
		void shift(const int sz);

		void clear();

	};
	class SLIB_DLL SText : public SObject {
	private:
		String _string;
		PArray<Pair<Range<int>, TextAttribute>> _attributes;

	public:
		SText();
		SText(const char *str, const SDictionary& a = {});
		SText(const String& str, const SDictionary& a = {});
		SText(const SString& str, const SDictionary& a = {});
		SText(const SText& text);
		~SText();

		SText& operator = (const char* s);
		SText& operator = (String&& s);
		SText& operator = (const String& s);
		SText& operator = (const SString& s);
		SText& operator = (const SText& txt);
		SText& operator += (const SText& txt);
		SText operator + (const SText& txt);

		SText& append(const SText& txt);

		void insert(size_t idx, const char* s);
		void insert(size_t idx, SText& txt);

		void removeAt(size_t idx);
		void remove(size_t off, size_t len);
		void remove(srange range);

		SText subtext(size_t off, size_t len);
		SText subtext(srange range);

		void clip(size_t off, size_t len = -1);
		void clip(srange range);

		void resize(const size_t sz);
		
		bool annotated(const int pos) const;
		bool annotated(const srange& rng) const;

		PArrayCIterator<Pair<Range<int>, TextAttribute>> attribute(const int pos) const;
		Range<PArrayCIterator<Pair<Range<int>, TextAttribute>>> attribute(const srange& rng) const;

		void setStyle(const srange& rng, const suint st);
		void setFont(const srange& rng, const char* f);
		void setSize(const srange& rng, const size_t sz);
		void setColor(const srange& rng, const Color &col);
		void setBackground(const srange& rng, const Color& col);
		void setAttribute(const srange& rng, const TextAttribute& attr);
		void removeAttribute(const srange& rng);
		void clearAttribute();

		const char* cstr() const;
		String& string();
		const String& string() const;

		operator const char* () const;

		String getClass() const;
		String toString(const char* format) const;
		SObject* clone() const;
	};
	/*
	class SLIB_DLL SText : public SObject {
	private:
		String _string;
		Array<STextAttribute> _attribute;

	public:
		SText();
		SText(const char* s);
		SText(const char* s, SDictionary&& a);
		SText(sobj obj);
		SText(const SText& text);
		~SText();

		SText& operator = (const char* s);
		SText& operator = (String&& s);
		SText& operator = (const String& s);
		SText& operator = (const SString& s);
		SText& operator = (const SText& txt);
		SText& operator += (const SText& txt);
		SText operator + (const SText& txt);

		const Array<STextAttribute>& attributes() const { return _attribute; }
		Array<STextAttribute>& attributes() { return _attribute; }
		bool hasAttribute(size_t pos) const {
			if (_attribute.empty() || pos < _attribute[0].begin || _attribute[-1].end < pos) return false;
			sforeach(_attribute) { if (E_.include(pos)) return true; }
			return false;
		}
		bool hasAttribute(sranges rng) const {
			if (_attribute.empty() || rng.end < _attribute[0].begin || _attribute[-1].end < rng.begin) return false;
			sforeach(_attribute) { if (E_.overlap(rng)) return true; }
			return false;
		}
		//STextAttribute& attribute(size_t pos) const;
		//Range<sarr_citer<STextAttribute>> attribute(sranges rng) const;
		//Range<sarr_iter<STextAttribute>> attribute(sranges rng);
		void addAttribute(const STextAttribute& attr) {
			if (_attribute.empty() || _attribute[-1].end < attr.begin) _attribute.add(attr);
			else if (attr.end < _attribute[0].begin) _attribute.insert(0, attr);
			else {
				sforeach(_attribute) {
					if (attr.end < E_.begin) break;
					

				}
			}
		}
		template<class... Args>
		void addAttribute(Args... args) { addAttribute(STextAttribute(args...)); }
		void updateAttribute(const STextAttribute& attr) { 
			//if (!hasAttribute(range)) return;
		}
		void addStyle(sranges range, sushort type) { 

		}
		void removeStyle(sranges range, sushort type) { 
			if (!hasAttribute(range)) return;

		}
		void setBold(sranges range) { addStyle(range, sstyle::BOLD); }
		void resetBold(sranges range) { removeStyle(range, sstyle::BOLD); }
		void setItalic(sranges range) { addStyle(range, sstyle::ITALIC); }
		void setUnderline(sranges range) { addStyle(range, sstyle::UNDERLINE); }
		void setSuper(srange range) { addStyle(range, sstyle::SUPERSCRIPT); }
		void setSub(srange range) { addStyle(range, sstyle::SUBSCRIPT); }
		void setFont(srange range, const char* font);
		void setSize(srange range, float size);
		void setColor(srange range, const smedia::SColor& col);
		void setBGColor(srange range, const smedia::SColor& col);
		void resize(size_t s) {
			if (!s) clear();
			else {
				if (s < _string.size()) {
					auto as = _attribute.size();
					srforeach(_attribute) {
						if (s < E_.begin) --as;
						else if (s < E_.end) E_.end = s - 1;
						else if (E_.end < s) break;
					}
					_attribute.resize(as);
				}
				_string.resize(s);
			}
		}
		void resize(size_t s, const char& c) {
			auto l = _string.length();
			resize(s);
			if (l < s) { 
				auto p = _string.ptr(l);
				sforin(c, l, s) { *p = c; ++p; }
			}
		}
		void clear() {
			_string.clear();
			_attribute.clear();
		}

		void add(const char& c);
		void append(const char* s);
		void append(const std::string& s);
		void append(const String& s);
		void append(const SString& s);
		void append(const SText& t);

		void insert(size_t idx, const char* s);
		void insert(size_t idx, const std::string& s);
		void insert(size_t idx, const String& s);
		void insert(size_t idx, const SString& s);

		void removeAt(size_t idx);
		void remove(size_t off, size_t len = -1);
		void remove(const srange& rng);

		void replace(size_t off, size_t len, const char* alt);
		void replace(const srange& rng, const char* alt);
		void replace(const char* ori, const char* alt);
		void replace(const Regex& rgx, const char* alt);
		void rearrange(const Regex& rgx, const CArray<sint>& order);

		void clip(size_t off, size_t len = -1);
		void clip(const srange& rng);

		SText subtext(size_t off, size_t len);
		SText subtext(srange range);

		void load(const char* path);
		void save(const char* path);

		
		String getClass() const { return "text"; }
		String toString() const;
		String toString(const char *form) const;
		SObject* clone() const;
	};
	*/
/*
	struct SLIB_DLL text_style {
		suint type;
        String font, ruby;
        float size, weight;
        Color color, background;
        
		text_style(suint t = sstyle::PLAIN, const char *f = "Arial", float s = 10.0, Color c = "black", Color b = "clear");
		text_style(SDictionary &dict);
		text_style(const text_style&style);
        ~text_style();
		text_style& operator=(const text_style& s);

		sobj toObj() const;
    };
	#define text_attribute std::pair<srange, text_style>
	*/




    
#define SBoldText(X) SText((X), { D_("type", slib::sstyle::BOLD) })
#define SItalicText(X) SText((X), { D_("type", slib::sstyle::ITALIC) })
#define SColorText(C, X) SText((X), { D_("color", Color(C).rgba()) })
    
}
#endif
