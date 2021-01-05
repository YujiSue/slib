#ifndef SLIB_STEXT_H
#define SLIB_STEXT_H

#include "sbasic/range.h"
#include "sobj/sstring.h"
#include "smedia/sgraphic.h"

namespace slib {
	const String PLAIN_TXT_TAG = ESC + "[0m";
	const String BOLD_TXT_TAG = ESC + "[1m";
	const String ITALIC_TXT_TAG = ESC + "[3m";
	const String UNDERLINE_TXT_TAG = ESC + "[4m";
	const String BLACK_TXT_TAG = ESC + "[30m";
	const String RED_TXT_TAG = ESC + "[31m";
	const String GREEN_TXT_TAG = ESC + "[32m";
	const String YELLOW_TXT_TAG = ESC + "[33m";
	const String BLUE_TXT_TAG = ESC + "[34m";
	const String MAGENTA_TXT_TAG = ESC + "[35m";
	const String CYAN_TXT_TAG = ESC + "[36m";	
	const String WHITE_TXT_TAG = ESC + "[37m";
	const String DEFAULT_COLOR_TXT_TAG = ESC + "[39m";
	const String BLACK_TXTBG_TAG = ESC + "[40m";
	const String RED_TXTBG_TAG = ESC + "[41m";
	const String GREEN_TXTBG_TAG = ESC + "[42m";
	const String YELLOW_TXTBG_TAG = ESC + "[43m";
	const String BLUE_TXTBG_TAG = ESC + "[44m";
	const String MAGENTA_TXTBG_TAG = ESC + "[45m";
	const String CYAN_TXTBG_TAG = ESC + "[46m";
	const String WHITE_TXTBG_TAG = ESC + "[47m";
	const String DEFAULT_COLOR_TXTBG_TAG = ESC + "[47m";
	const String DEFAULT_TXT_TAG = ESC + "[0m" + ESC + "[39m" + ESC + "[49m";

    class SLIB_DLL SText;
	/*
	class SLIB_DLL STextAttribute : public Range<size_t>, public SObject {
	public:
		suint type;
		String font;
		float size;
		smedia::SColor color, background;

		STextAttribute();
		STextAttribute(const STextAttribute &attr);
		~STextAttribute();
		STextAttribute& operator=(const STextAttribute& attr);
	};
	
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

	struct SLIB_DLL text_style {
		suint type;
        String font, ruby;
        float size, weight;
        smedia::SColor color, background;
        
		text_style(suint t = sstyle::PLAIN, const char *f = "Arial", float s = 10.0, smedia::SColor c = "black", smedia::SColor b = "clear");
		text_style(SDictionary &dict);
		text_style(const text_style&style);
        ~text_style();
		text_style& operator=(const text_style& s);

		sobj toObj() const;
    };
	using text_attribute = std::pair<srange, text_style>;

    class SLIB_DLL SText : public SObject {
        
    private:
        String _string;
        Array<text_attribute> _attribute;
        
    public:
        SText();
		SText(const char* s);
		SText(const char* s, SDictionary &&a);
		SText(sobj obj);
        SText(const SText &text);
        ~SText();
        
        SText &operator = (const char *s);
        SText &operator = (String &&s);
        SText &operator = (const String &s);
        SText &operator = (const SString &s);
        SText &operator = (const SText &txt);
        SText &operator += (const SText &txt);
        SText operator + (const SText &txt);
        
        //srange aindexAt(size_t idx) const;
        
        SText &append(const SText &txt);
        
        void insert(size_t idx, const char *s);
        void insert(size_t idx, SText &txt);
        
        void removeAt(size_t idx);
        void remove(size_t off, size_t len);
        void remove(srange range);
        
        SText subtext(size_t off, size_t len);
        SText subtext(srange range);
        
        void clip(size_t off, size_t len = -1);
        void clip(srange range);
        
        
        void resize(size_t size);
        
        void load(const char *path);
        void save(const char *path);
        
        const Array<text_attribute> &attributes() const;
		Array<text_attribute>& attributes();
        Array<text_attribute> attribute(srange range) const;
        bool isAttributed(srange range) const;
        
        void setStyle(srange range, const text_style &style);
        void setType(srange range, uint16_t type);
        void setFont(srange range, const char *font);
        void setSize(srange range, const float &size);
        void setColor(srange range, const smedia::SColor &col);
        void setBGColor(srange range, const smedia::SColor &col);
        void setAttribute(srange range, sobj attr);
        
        const char *cstr() const;
		String& string();
        const String &string() const;
        
        String getClass() const;
        String toString() const;
        SObject *clone() const;
    };
    
#define SBoldText(X) SText((X), { kv("type", slib::sstyle::BOLD) })
#define SItalicText(X) SText((X), { kv("type", slib::sstyle::ITALIC) })
#define SColorText(C, X) SText((X), { kv("color", (C)) })
    
}
#endif
