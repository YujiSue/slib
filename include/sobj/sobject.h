#ifndef SLIB_SOBJECT_H
#define SLIB_SOBJECT_H

#include "sbasic.h"

namespace slib {
	using namespace smath;
	typedef enum {
		NULL_OBJ = 0x0000,
		VOID_OBJ = 0x0001,
		NUMBER_OBJ = 0x0002,
		STRING_OBJ = 0x0003,
		DATE_OBJ = 0x0004,
		DATA_OBJ = 0x0005,
		ARRAY_OBJ = 0x0006,
		DICT_OBJ = 0x0007,
		FUNC_OBJ = 0x0008,
		FILE_OBJ = 0x0009,
		TEXT_OBJ = 0x000A,
		COLUMN_OBJ = 0x000B,
		TABLE_OBJ = 0x000C,
		CHAR_OBJ = 0x000D,
		PAIR_OBJ = 0x000E,
		NODE_OBJ = 0x000F,

		DOC_OBJ = 0x0011,
		
		DB_OBJ = 0x0013,

		COLOR_OBJ = 0x0020,
		PIXEL_OBJ = 0x0021,
		IMAGE_OBJ = 0x0022,
		FIGURE_OBJ = 0x0023,
		CANVAS_OBJ = 0x0024,

		SOUND_OBJ = 0x0031,
        
		MOVIE_OBJ = 0x0041,

        MODEL_OBJ = 0x0051,
        SPACE_OBJ = 0x0052,

        SGUI_OBJ = 0x0080,


    } OBJ_TYPE;
    
    #define sobj slib::SObjPtr
    #define scobj slib::SClsPtr
    
    class SLIB_DLL SObjPtr;
    template<class Cls, OBJ_TYPE T=NULL_OBJ>
    class SLIB_DLL SClsPtr;
    
    class SLIB_DLL SObject;
    class SLIB_DLL SNumber;
	using snum = SClsPtr<SNumber, NUMBER_OBJ>;
    class SLIB_DLL SChar;
	using schar = SClsPtr<SChar, CHAR_OBJ>;
    class SLIB_DLL SString;
	using ss = SClsPtr<SString, STRING_OBJ>;
    class SLIB_DLL SText;
	using stxt = SClsPtr<SText, TEXT_OBJ>;
    class SLIB_DLL SDate;
	using sdt = SClsPtr<SDate, DATE_OBJ>;
    class SLIB_DLL SData;
	using sdat = SClsPtr<SData, DATA_OBJ>;
    class SLIB_DLL SArray;
	using sarray = SClsPtr<SArray, ARRAY_OBJ>;
    class SLIB_DLL SPair;
	using spair = SClsPtr<SPair, PAIR_OBJ>;
    class SLIB_DLL SDictionary;
	using sdict = slib::SClsPtr<slib::SDictionary, DICT_OBJ>;
	template<class Return, class... Args>
	class SFunction;
	template<class Return, class... Args>
	using sfunc = SClsPtr<SFunction<Return, Args...>, FUNC_OBJ>;
    class SLIB_DLL SColumn;
	using scolumn = SClsPtr<SColumn, COLUMN_OBJ>;
    class SLIB_DLL STable;
	using stable = SClsPtr<STable, TABLE_OBJ>;
	class SLIB_DLL SDataBase;
	using sdb = SClsPtr<SDataBase, DB_OBJ>;

    namespace sio {
        class SLIB_DLL SFile;
		using sfile = SClsPtr<SFile, FILE_OBJ>;
    }
    namespace smedia {
		class SLIB_DLL SColor;
		using scolor = SClsPtr<SColor, COLOR_OBJ>;
		class SLIB_DLL SImage;
		using simg = SClsPtr<SImage, IMAGE_OBJ>;
        class SLIB_DLL SFigure;
		using sfig = SClsPtr<SFigure, FIGURE_OBJ>;
        class SLIB_DLL SCanvas;
		using scnvs = SClsPtr<SCanvas, CANVAS_OBJ>;
        //class SLIB_DLL SSound;
		//using ssnd = SClsPtr<SSound, SOUND_OBJ>;
        class SLIB_DLL SMovie;
		using smov = SClsPtr<SMovie, MOVIE_OBJ>;
    };
    namespace sworld {
        class SLIB_DLL SModel;
        class SLIB_DLL SSpace;
    };
    
    #define snull sobj()
    #define kv(X,Y) slib::kvpair<slib::String, slib::SObjPtr>((X), V(Y))

    class SLIB_DLL SObject {
    protected:
#ifndef _MANAGED
        ::std::atomic<int> _scope;
#else 
        int _scope;
#endif
        
    public:
        SObject();
        virtual ~SObject();
        
        void addScope();
        void removeScope();
        bool scoped() const;
        virtual String getClass() const = 0;
        virtual String toString() const = 0;
        virtual SObject *clone() const = 0;
    };
    
    class SLIB_DLL SIterator;
    class SLIB_DLL SCIterator;
    
	class SLIB_DLL SObjPtr {
	protected:
		OBJ_TYPE _type;
		SObject* _ptr;

	public:
		SObjPtr();
		SObjPtr(OBJ_TYPE ot);
		SObjPtr(bool b);
		SObjPtr(sbyte i);
		SObjPtr(subyte ui);
		SObjPtr(sshort i);
		SObjPtr(sushort ui);
		SObjPtr(int i);
		SObjPtr(unsigned int ui);
		SObjPtr(size_t ui);
#ifdef WIN64_OS
		SObjPtr(long i);
#ifndef MAC_OS
		SObjPtr(unsigned long ui);
#endif
#endif
		SObjPtr(long long i);
#ifdef MAC_OS
		SObjPtr(unsigned long long i);
#endif
#ifdef LINUX_OS
		SObjPtr(sinteger i);
#endif
		SObjPtr(float f);
		SObjPtr(double d);
		SObjPtr(const smath::Fraction<sint>& frac);
		SObjPtr(const smath::Complex<float>& comp);
		SObjPtr(const char* s);
		SObjPtr(const ::std::string& str);
		SObjPtr(String&& str);
		SObjPtr(const String& str);
		SObjPtr(::std::initializer_list<SObjPtr> li);
		SObjPtr(::std::initializer_list<::std::pair<String, SObjPtr>> li);
		SObjPtr(::std::initializer_list<kvpair<String, SObjPtr>> li);
		SObjPtr(const stringarray& array);
		SObjPtr(const sattribute& atrr);
		SObjPtr(const SNumber& num);
		SObjPtr(const SChar& ch);
		SObjPtr(const SString& str);
		SObjPtr(const SDate& date);
		SObjPtr(const SData& dat);
		SObjPtr(const SArray& array);
		SObjPtr(const SPair& pair);
		SObjPtr(const SDictionary& dict);
		SObjPtr(const SText& txt);
		template<class Return, class... Args>
		SObjPtr(const std::function<Return(Args...)>& func) : _type(FUNC_OBJ), _ptr(new SFunction<Return(Args...)>(func)) {}
		template<class Return, class... Args>
		SObjPtr(const SFunction<Return(Args...)>& func) : _type(FUNC_OBJ), _ptr(new SFunction<Return(Args...)>(func)) {}
		SObjPtr(const SColumn& col);
		SObjPtr(const STable& tbl);
		SObjPtr(const SDataBase& db);

		SObjPtr(const sio::SFile& file);
		SObjPtr(const smedia::SColor& col);
		SObjPtr(const smedia::SImage& img);
		SObjPtr(const smedia::SFigure& fig);
		SObjPtr(const smedia::SCanvas& cnvs);
		//SObjPtr(const smedia::SSound &snd);
		SObjPtr(const smedia::SMovie& mov);
		SObjPtr(SObject* so);
		template<class Cls>
		SObjPtr(OBJ_TYPE t, Cls* cls) : _type(t), _ptr(dynamic_cast<SObject*>(cls)) {}
		SObjPtr(SObjPtr&& obj);
		SObjPtr(const SObjPtr& obj);
		template<class Cls, OBJ_TYPE T>
		SObjPtr(SClsPtr<Cls, T>&& obj) : _type(T), _ptr(obj._ptr) { obj.discard(); }
		template<class Cls, OBJ_TYPE T>
		SObjPtr(const SClsPtr<Cls, T>& obj) : _type(T), _ptr(obj._ptr) { share(); }
		virtual ~SObjPtr();
		static SObjPtr toSObj(const String& s);
		static SObjPtr import(sobj info);
		SObjPtr& operator = (SObjPtr&& obj);
		SObjPtr& operator = (const SObjPtr& obj);
		template<class Cls, OBJ_TYPE T>
		SObjPtr& operator = (SClsPtr<Cls, T>&& obj);
		template<class Cls, OBJ_TYPE T>
		SObjPtr& operator = (const SClsPtr<Cls, T>& obj);

		SObjPtr& operator += (const char* s);
		SObjPtr& operator += (const ::std::string& s);
		SObjPtr& operator += (const String& s);
		SObjPtr& operator += (const SString& s);
		SObjPtr& operator += (const SObjPtr& obj);
		template<typename T>
		SObjPtr& operator += (const T& val) {
			if (isNum()) number() += val;
			else if (isStr()) string() += val;
		}
		SObjPtr& operator -= (const SObjPtr& obj);
		template<typename T>
		SObjPtr& operator -= (const T& val) {
			if (isNum()) number() -= val;
			return *this;
		}
		SObjPtr& operator *= (int i);
		SObjPtr& operator *= (size_t s);
		SObjPtr& operator *= (const SObjPtr& obj);
		template<typename T>
		SObjPtr& operator *= (const T& val) {
			if (isNum()) number() *= val;
			return *this;
		}
		SObjPtr& operator /= (const SObjPtr& obj);
		template<typename T>
		SObjPtr& operator /= (const T& val) {
			if (isNum()) number() /= val;
			return *this;
		}
		SObjPtr& operator %= (const SObjPtr& obj);
		template<typename T>
		SObjPtr& operator %= (const T& val) {
			if (isNum()) number() %= val;
			return *this;
		}
		SObjPtr& operator++();
		SObjPtr& operator--();
		SObjPtr operator-() const;

		SObjPtr operator+(const char* s) const;
		SObjPtr operator+(const ::std::string& s) const;
		SObjPtr operator+(const String& s) const;
		SObjPtr operator+(const SString& s) const;
		SObjPtr operator+(const SObjPtr& obj) const;
		template<typename T>
		SObjPtr operator+(T val) const {
			if (isNum()) return number() + val;
			throw SException(ERR_INFO, SLIB_CAST_ERROR, T(_ptr), CAST_TEXT(, T(number)));
		}
		SObjPtr operator-(const SObjPtr& obj) const;
		template<typename T>
		SObjPtr operator-(T val) const {
			if (isNum()) return number() - val;
			throw SException(ERR_INFO, SLIB_CAST_ERROR);
		}
		SObjPtr operator*(int i) const;
		SObjPtr operator*(size_t s) const;
		template<typename T>
		SObjPtr operator*(T val) const {
			if (isNum()) return number() * val;
			throw SException(ERR_INFO, SLIB_CAST_ERROR);
		}
		SObjPtr operator*(const SObjPtr& obj) const;
		template<typename T>
		SObjPtr operator/(T val) const {
			if (isNum()) return number() / val;
			throw SException(ERR_INFO, SLIB_CAST_ERROR);
		}
		SObjPtr operator/(const SObjPtr& obj) const;
		template<typename T>
		SObjPtr operator%(T val) const {
			if (isNum()) return number() % val;
			throw SException(ERR_INFO, SLIB_CAST_ERROR);
		}
		SObjPtr operator%(const SObjPtr& obj) const;

		SObject* ptr();
		const SObject* ptr() const;
		SObject* operator->() const;
		SObject& operator*();
		const SObject& operator*() const;

		void share();
		void release();
		void copyTo(SObjPtr& ptr) const;
		void moveTo(SObjPtr& ptr);
		void clone(const slib::SObjPtr& obj);
		void discard();
		void swap(SObjPtr& ptr);

		SObjPtr& operator[](int idx);
		const SObjPtr& operator[](int idx) const;
		SObjPtr& operator[](const char* key);
		const SObjPtr& operator[](const char* key) const;
		SObjPtr& operator[](const ::std::string& key);
		const SObjPtr& operator[](const ::std::string& key) const;
		SObjPtr& operator[](const String& key);
		const SObjPtr& operator[](const String& key) const;
		SObjPtr& operator[](const SString& key);
		const SObjPtr& operator[](const SString& key) const;
		SObjPtr& operator[](const SObjPtr& obj);
		const SObjPtr& operator[](const SObjPtr& obj) const;
		SObjPtr& at(int idx);
		const SObjPtr& at(int idx) const;
		SObjPtr& at(const char* key);
		const SObjPtr& at(const char* key) const;
		SObjPtr& at(const ::std::string& key);
		const SObjPtr& at(const ::std::string& key) const;
		SObjPtr& at(const String& key);
		const SObjPtr& at(const String& key) const;
		SObjPtr& at(const SString& key);
		const SObjPtr& at(const SString& key) const;
		SObjPtr& at(const SObjPtr& obj);
		const SObjPtr& at(const SObjPtr& obj) const;
		
		SObjPtr getValue(int idx) const;


		SIterator begin();
		SCIterator begin() const;
		SIterator end();
		SCIterator end() const;

		void load(sobj info);
        void save(sobj info);
        
        suint type() const;
        size_t size() const;
		void resize(size_t s, sobj obj = snull);
        size_t length() const;
        bool empty() const;
        String name() const;
        String key() const;
        const SObjPtr &value() const;
        bool hasKey(const char *key) const;
        stringarray keyset() const;
		bool beginWith(const char* que) const;
		bool endWith(const char* que) const;
		String substring(size_t offset, size_t len = -1) const;
		String substring(srange range) const;
        SObjPtr subset(size_t offset, size_t len = -1) const;
        SObjPtr subset(srange range) const;
		stringarray split(const char *sep, bool trim = true) const;
		stringarray splitline(bool trim = true) const;
		String replace(const char* ori, const char* alt) const;
		sattribute parse(const char *sep, const char *part, bool trim = true) const;
		String fill(size_t size, char fill = ' ', bool head = false);
		SObjPtr &sort(std::function<bool(const SObjPtr & o1, const SObjPtr & o2)> Comparer = sortAsc<SObjPtr>);
		SObjPtr& sortBy(const char *key, slib::ORDER order = ASC);
		void convert(int t);
        void transform(int t);
        
        void add(SObjPtr &&ptr);
        void add(const SObjPtr &ptr);
        void insert(int idx, const SObjPtr &ptr);
		void insert(const SObjPtr& ptr);
        void insert(const kvpair<String, SObjPtr> &pair);
        void set(int idx, const SObjPtr &ptr);
        void set(const char *key, const SObjPtr &ptr);
		void set(const SObjPtr &obj, const SObjPtr& ptr);
        void removeAt(int idx);
        void remove(const char *key);
        void remove(const size_t &off, const size_t &len);
        void remove(const srange &range);
        void clear();
        
        bool contain(const char *que) const;
        bool contain(const String &que) const;
        bool contain(const SObjPtr &ptr) const;
        bool match(const Regex &rgx, size_t offset = 0) const;
        bool equal(const Regex &rgx) const;
        size_t find(const char *que) const;
        size_t find(const SObjPtr &ptr) const;
        size_t rfind(const char *que) const;
        size_t rfind(const SObjPtr &ptr) const;
        sizearray search(const Regex &rgx, size_t offset = 0) const;
        String toString() const;
        
        bool isNull() const;
        bool isHollow() const;
        bool isNumeric() const;
        bool isNum() const;
        bool isChar() const;
        bool isStr() const;
        bool isDate() const;
        bool isDat() const;
        bool isArray() const;
        bool isPair() const;
        bool isDict() const;
		bool isText() const;
		bool isFile() const;
        bool isFunc() const;
        bool isColumn() const;
        bool isTable() const;
		bool isDB() const;
        bool isNode() const;
		bool isColor() const;
		bool isImg() const;
        bool isFig() const;
        bool isCnvs() const;
        //bool isSound() const;
        bool isMov() const;
        template<class Cls>
        bool isClass() const {
            if (_ptr) return instanceOf<Cls>(_ptr);
			return false;
        }
        //Convert
		template<class Cls>
		inline Cls* toClassPtr() { return _ptr ? dynamic_cast<Cls*>(_ptr) : nullptr; }
		template<class Cls>
		inline const Cls* toClassPtr() const { return _ptr ? dynamic_cast<const Cls*>(_ptr) : nullptr; }
		template<class Cls>
        inline Cls &toClass() {
            if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
            if (isClass<Cls>()) return *toClassPtr<Cls>();
            throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
        template<class Cls>
        inline const Cls &toClass() const {
            if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
            if (isClass<Cls>()) return *toClassPtr<Cls>();
            throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
        
        bool boolean() const;
        sbyte byteValue() const;
        subyte ubyteValue() const;
        sshort shortValue() const;
        sushort ushortValue() const;
        int intValue() const;
        unsigned int uintValue() const;
        size_t sizeValue() const;
        long longValue() const;
        unsigned long ulongValue() const;
        long long llongValue() const;
        unsigned long long ullongValue() const;
        float floatValue() const;
        double doubleValue() const;
        sinteger integer() const;
        suinteger uinteger() const;
        sreal real() const;
        
        String str() const;
        
        SNumber &number();
        const SNumber &number() const;
        SChar &character();
        const SChar &character() const;
        SString &string();
        const SString &string() const;
        SDate &date();
        const SDate &date() const;
        SData &data();
        const SData &data() const;
        SArray &array();
        const SArray &array() const;
        SPair &pair();
        const SPair &pair() const;
        SDictionary &dict();
        const SDictionary &dict() const;
		SText& text();
		const SText& text() const;
		sio::SFile& file();
		const sio::SFile& file() const;
		template<class... Args>
        SFunction<sobj(Args...)> &func() {
            if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
            if (isFunc()) return *dynamic_cast<SFunction<sobj(Args...)> *>(_ptr);
            throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
		template<class... Args>
        const SFunction<sobj(Args...)> &func() const {
            if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
            if (isFunc()) return *dynamic_cast<SFunction<sobj(Args...)> *>(_ptr);
            throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
		template<class... Args>
		SFunction<int(Args...)>& funci() {
			if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
			if (isFunc()) return *dynamic_cast<SFunction<int(Args...)>*>(_ptr);
			throw SException(ERR_INFO, SLIB_CAST_ERROR);
		}
		template<class... Args>
		const SFunction<int(Args...)>& funci() const {
			if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
			if (isFunc()) return *dynamic_cast<SFunction<int(Args...)>*>(_ptr);
			throw SException(ERR_INFO, SLIB_CAST_ERROR);
		}
		template<class... Args>
		SFunction<void(Args...)>& funcv() {
			if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
			if (isFunc()) return *dynamic_cast<SFunction<void(Args...)>*>(_ptr);
			throw SException(ERR_INFO, SLIB_CAST_ERROR);
		}
		template<class... Args>
		const SFunction<void(Args...)>& funcv() const {
			if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
			if (isFunc()) return *dynamic_cast<SFunction<void(Args...)>*>(_ptr);
			throw SException(ERR_INFO, SLIB_CAST_ERROR);
		}
        SColumn &column();
        const SColumn &column() const;
        STable &table();
        const STable &table() const;
		SDataBase& db();
		const SDataBase& db() const;
		smedia::SColor& color();
		const smedia::SColor& color() const;
        smedia::SImage &image();
        const smedia::SImage &image() const;
        smedia::SFigure &figure();
        const smedia::SFigure &figure() const;
        smedia::SCanvas &canvas();
        const smedia::SCanvas &canvas() const;
        //smedia::SSound &sound();
        //const smedia::SSound &sound() const;
        smedia::SMovie &movie();
        const smedia::SMovie &movie() const;
        
       //
        operator SObject *() const;
        operator bool() const;
        operator sbyte() const;
        operator subyte() const;
        operator sshort() const;
        operator sushort() const;
        operator int() const;
        operator unsigned int() const;
        operator size_t() const;
#ifdef WIN64_OS
        operator long() const;
#ifndef MAC_OS
        operator unsigned long() const;
#endif
#endif
        operator long long() const;
#ifdef MAC_OS
        operator unsigned long long() const;
#endif
#ifdef LINUX_OS
		operator sinteger() const;
#endif
        operator float() const;
        operator double() const;
        operator const char *() const;
		/*
		template<class Return, class... Args>
		Return operator()(Args... args) {
			if (isFunc()) func<Return, Args...>()(args...);
		}
		*/
		template<class... Args>
		sobj operator()(Args... args) {
			if (isFunc()) return func<Args...>()(args...);
			throw SException(ERR_INFO);
		}

		sobj clone() const;
        //compare
        bool operator < (const SObjPtr &obj) const;
        template<typename T>
        bool operator < (const T &val) const {
            if (isNum()) return number() < val;
            else throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
        template<typename T>
        bool operator > (const T &val) const {
            if (isNum()) return number() > val;
            else throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
        template<typename T>
        bool operator <= (const T &val) const {
            if (isNum()) return number() <= val;
            else throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
        template<typename T>
        bool operator >= (const T &val) const {
            if (isNum()) return number() >= val;
            else throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
        bool operator == (const SNumber &n) const;
        bool operator == (const SObjPtr &obj) const;
        bool operator == (const char *s) const;
        template<typename T>
        bool operator == (const T &val) const {
            if (isNum()) return number() == val;
            else throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
        bool operator != (const SObjPtr &obj) const;
        bool operator != (const char *s) const;
        template<typename T>
        bool operator != (const T &val) const {
            if (isNum()) return number() != val;
            else throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
    };
	extern SLIB_DLL String operator+(const char* s, const SObjPtr& obj);
	extern SLIB_DLL String operator+(const ::std::string& s, const SObjPtr& obj);
	extern SLIB_DLL bool operator<(const int& i, const SObjPtr& obj);
    
    template<class Cls, OBJ_TYPE T>
    class SLIB_DLL SClsPtr : public SObjPtr {
    public:
        SClsPtr() : SObjPtr() { sobj::_type = T; }
        SClsPtr(Cls *cls) : SObjPtr(T, cls) {}
		SClsPtr(std::initializer_list<SObjPtr> li) : SObjPtr(li) {}
		SClsPtr(std::initializer_list<std::pair<String, SObjPtr>> li) : SObjPtr(li) {}
		SClsPtr(std::initializer_list<kvpair<String, SObjPtr>> li) : SObjPtr(li) {}
        template<class... Args>
        SClsPtr(Args ...args) : SObjPtr(T, new Cls(args...)) {}
        SClsPtr(SObjPtr &&obj) : SObjPtr(std::forward<SObjPtr>(obj)) {}
        SClsPtr(const SObjPtr &obj) : SObjPtr(obj) {}
        SClsPtr(SClsPtr &&obj) : SObjPtr() { _type = obj._type; _ptr = obj._ptr; obj.discard(); }
        SClsPtr(const SClsPtr &obj) : SObjPtr() { _type = obj._type; _ptr = obj._ptr; share(); }
        ~SClsPtr() {}
        
        SClsPtr &operator=(const SClsPtr<Cls, T>&obj) {
            release();_type = obj._type; _ptr = obj._ptr; share(); return *this;
        }
        Cls &operator*() {
            if (SObjPtr::_ptr) return *dynamic_cast<Cls *>(SObjPtr::_ptr);
            else throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
        const Cls &operator*() const {
            if (SObjPtr::_ptr) return *dynamic_cast<const Cls *>(SObjPtr::_ptr);
            else throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
        Cls *operator->() {
            if (SObjPtr::_ptr) return dynamic_cast<Cls *>(SObjPtr::_ptr);
            return nullptr;
        }
        const Cls *operator->() const {
            if (SObjPtr::_ptr) return dynamic_cast<const Cls *>(SObjPtr::_ptr);
            return nullptr;
        }
        operator Cls *() const {
            return isNull()?nullptr:const_cast<Cls *>(dynamic_cast<const Cls *>(SObjPtr::_ptr));
        }
    };
    
    template<class Cls, OBJ_TYPE T>
    SObjPtr &SObjPtr::operator=(SClsPtr<Cls, T> &&obj) {
        obj.moveTo(*this); return *this;
    }
    template<class Cls, OBJ_TYPE T>
    SObjPtr &SObjPtr::operator=(const SClsPtr<Cls, T> &obj) {
        obj.copyTo(*this); return *this;
    }
    namespace smath {
		extern SObjPtr absolute(const SObjPtr& obj);
	}
    #define siter SIterator
    #define sciter SCIterator
    
    class SLIB_DLL SIterator {
        friend sobj;
    public:
        typedef ::std::random_access_iterator_tag iterator_category;
        typedef sobj value_type;
        typedef ::std::ptrdiff_t difference_type;
        typedef sobj* pointer;
        typedef sobj& reference;
        
        
        union Iter {
            SArrayIterator<sobj> _ait;
            SUtf8Iterator _uit;
            SMapIterator<String, sobj> _mit;

            Iter() : _ait(nullptr) {}
            ~Iter() {}
        };
        
    private:
        OBJ_TYPE _type;
        Iter _it;
        sobj _obj;
        
    public:
        SIterator(SArrayIterator<SObjPtr> ait);
        SIterator(SUtf8Iterator uit);
        SIterator(SMapIterator<String, SObjPtr> mit);
        SIterator(const SIterator &it);
        ~SIterator();
        
        SIterator& operator=(const SIterator &it);
        reference operator *();
        pointer operator ->();
        reference operator [](::std::ptrdiff_t diff);
        SIterator &operator ++();
        SIterator operator ++(int);
        SIterator &operator --();
        SIterator operator --(int);
        SIterator &operator +=(::std::ptrdiff_t diff);
        SIterator &operator -=(::std::ptrdiff_t diff);
        SIterator operator +(::std::ptrdiff_t diff);
        SIterator operator -(::std::ptrdiff_t diff);
		sinteger operator -(SIterator it);
        void swap(SIterator sit1, SIterator sit2);
        bool operator ==(const SIterator &sit) const;
        bool operator !=(const SIterator &sit) const;
        bool operator <(const SIterator &sit) const;
        bool operator <=(const SIterator &sit) const;
        bool operator >(const SIterator &sit) const;
        bool operator >=(const SIterator &sit) const;
    };
    
    class SLIB_DLL SCIterator {
    public:
        typedef ::std::random_access_iterator_tag iterator_category;
        typedef const sobj value_type;
        typedef ::std::ptrdiff_t difference_type;
        typedef const sobj* pointer;
        typedef const sobj& reference;
        
        union CIter {
            SArrayCIterator<sobj> _ait;
            SUtf8CIterator _uit;
            SMapCIterator<String, sobj> _mit;
            
			CIter() : _ait(nullptr) {}
            ~CIter() {}
        };
        
    private:
		OBJ_TYPE _type;
		CIter _it;
        sobj _obj;
        
    public:
        SCIterator(SArrayCIterator<SObjPtr> ait);
        SCIterator(SUtf8CIterator uit);
        SCIterator(SMapCIterator<String, SObjPtr> mit);
        SCIterator(const SCIterator &it);
        ~SCIterator();
        
        SCIterator& operator=(const SCIterator &it);
        reference operator *();
        pointer operator ->();
        reference operator [](::std::ptrdiff_t diff);
        SCIterator &operator ++();
        SCIterator operator ++(int);
        SCIterator &operator --();
        SCIterator operator --(int);
        SCIterator &operator +=(::std::ptrdiff_t diff);
        SCIterator &operator -=(::std::ptrdiff_t diff);
        SCIterator operator +(::std::ptrdiff_t diff);
        SCIterator operator -(::std::ptrdiff_t diff);
        sinteger operator -(SCIterator it);
        void swap(SCIterator sit1, SCIterator sit2);
        bool operator ==(const SCIterator &sit) const;
        bool operator !=(const SCIterator &sit) const;
        bool operator <(const SCIterator &sit) const;
        bool operator <=(const SCIterator &sit) const;
        bool operator >(const SCIterator &sit) const;
        bool operator >=(const SCIterator &sit) const;
    };
}
#endif