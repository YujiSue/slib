#ifndef SLIB_STEXT_H
#define SLIB_STEXT_H

#include "sobj/sstring.h"
#include "smedia/sgraphic.h"

namespace slib {
	const String PLAIN_TXT_TAG = String::ESC + "[0m";
	const String BOLD_TXT_TAG = String::ESC + "[1m";
	const String ITALIC_TXT_TAG = String::ESC + "[3m";
	const String UNDERLINE_TXT_TAG = String::ESC + "[4m";
	const String BLACK_TXT_TAG = String::ESC + "[30m";
	const String RED_TXT_TAG = String::ESC + "[31m";
	const String GREEN_TXT_TAG = String::ESC + "[32m";
	const String YELLOW_TXT_TAG = String::ESC + "[33m";
	const String BLUE_TXT_TAG = String::ESC + "[34m";
	const String MAGENTA_TXT_TAG = String::ESC + "[35m";
	const String CYAN_TXT_TAG = String::ESC + "[36m";	
	const String WHITE_TXT_TAG = String::ESC + "[37m";
	const String DEFAULT_COLOR_TXT_TAG = String::ESC + "[39m";
	const String BLACK_TXTBG_TAG = String::ESC + "[40m";
	const String RED_TXTBG_TAG = String::ESC + "[41m";
	const String GREEN_TXTBG_TAG = String::ESC + "[42m";
	const String YELLOW_TXTBG_TAG = String::ESC + "[43m";
	const String BLUE_TXTBG_TAG = String::ESC + "[44m";
	const String MAGENTA_TXTBG_TAG = String::ESC + "[45m";
	const String CYAN_TXTBG_TAG = String::ESC + "[46m";
	const String WHITE_TXTBG_TAG = String::ESC + "[47m";
	const String DEFAULT_COLOR_TXTBG_TAG = String::ESC + "[47m";
	const String DEFAULT_TXT_TAG = String::ESC + "[0m" + String::ESC + "[39m" + String::ESC + "[49m";

    class SLIB_DLL SText;

	struct text_style {
		suint type;
        String font, ruby;
        float size, weight;
        smedia::SColor color, background;
        
		text_style(int t = sstyle::PLAIN, const char *f = "Arial", float s = 10.0, smedia::SColor c = "black", smedia::SColor b = "clear");
		text_style(const sobj &obj);
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
        SText(const String &s, const sobj& a = snull);
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
        void setAttribute(srange range, const sobj &attr);
        
        const char *cstr() const;
		String& string();
        const String &string() const;
        
        String getClass() const;
        String toString() const;
        SObject *clone() const;
    };
    
#define SBoldText(X) SText((X), { kv("type", sstyle::BOLD) })
#define SItalicText(X) SText((X), { kv("type", sstyle::ITALIC) })
#define SColorText(C, X) SText((X), { kv("color", (C)) })
    
}
#endif
