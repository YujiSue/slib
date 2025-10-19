#ifndef SLIB_SOBJPTR_H
#define SLIB_SOBJPTR_H

#include "sbasic/array.h"
#include "sbasic/map.h"
#include "sbasic/string.h"
#include "sbasic/region.h"
#include "smath/fraction.h"
#include "smath/complex.h"
#include "smath/numeric.h"
#include "sobj/sobject.h"
#include "sobj/svoid.h"

namespace slib {
    class SLIB_DLL SVoid;
    class SLIB_DLL SNumber;
    class SLIB_DLL SString;
    class SLIB_DLL STime;
    class SLIB_DLL SDate;
    class SLIB_DLL SData;
    class SLIB_DLL SArray;
    class SLIB_DLL SDictionary;
    class SLIB_DLL SText;
    class SLIB_DLL SFile;
    class SLIB_DLL STable;
    template<class Ret, class... Args>
    class SFunction;

    class SLIB_DLL SImage;
    class SLIB_DLL SFigure;
    class SLIB_DLL SCanvas;

    class SLIB_DLL SIterator;
    class SLIB_DLL SCIterator;
    class SLIB_DLL SObjPtr;
    /**
    * @class SElement
    * \~english @brief Element object for container class
    */
    class SLIB_DLL SElement {
        friend SIterator;
        friend SCIterator;
        friend SObjPtr;
        enum class SLIB_DLL E_CLASS : subyte{
            OBJECT = 1,
            CHAR = 2,
            PAIR = 3,
        };
        union SLIB_DLL Element {
            SObjPtr* obj;
            Char ch;
            MapData<String, SObjPtr>* pair;

            Element() { obj = nullptr; }
            ~Element() {}
        };
        E_CLASS eclass;
        Element element;

    public:
        /**
        * Constructor/Destructor
        */
        SElement();
        SElement(SObjPtr* obj);
        SElement(ArrayIterator<SObjPtr> it);
        SElement(ArrayCIterator<SObjPtr> it);
        SElement(Utf8Iterator it);
        SElement(Utf8CIterator it);
        SElement(MapIterator<String, SObjPtr> it);
        SElement(MapCIterator<String, SObjPtr> it);
        SElement(const SElement& elem);
        ~SElement();

        /*
        * Assign
        */
        SElement& operator=(const SElement& elem);
        SElement& operator=(const SObjPtr& obj);

        /*
        * Arithmetic / Operator
        */

        SObjPtr operator+(const char *s);
        SObjPtr &operator+=(const char* s);

        /*
        * Getter
        */
        size_t size() const;
        size_t length() const;
        
        String& key();
        const String& key() const;
        SObjPtr& value();
        const SObjPtr& value() const;

        String toString(const char* format = nullptr) const;

        /*
        * Setter
        */
        void resize(const size_t sz);

        /*
        * Access
        */
        SElement operator[](const int i);
        const SElement operator[](const int i) const;
        SElement operator[](const char* s);
        const SElement operator[](const char* s) const;
        SElement operator[](const SElement &e);
        const SElement operator[](const SElement& e) const;
        SIterator begin();
        SCIterator begin() const;
        SIterator end();
        SCIterator end() const;

        /*
        * Editor
        */
        SObjPtr& add(const SObjPtr& obj);
        SObjPtr& append(const SObjPtr obj);
        SObjPtr& append(const SArray& array);
        SObjPtr& set(const String& key, const SObjPtr& obj);

        
        /*
        * Checker
        */
        bool isNull() const;
        bool isObj() const;
        bool isChar() const;
        bool isPair() const;

        bool isNum() const;
        bool isStr() const;
        bool isDate() const;
        bool isData() const;
        bool isArray() const;
        bool isDict() const;
        bool isText() const;
        bool isFile() const;
        bool isFunc() const;

        bool isImg() const;
        bool isFig() const;
        bool isCnvs() const;

        bool empty() const;
        bool hasKey(const char* key) const;
        stringarray split(const char* s) const;
        bool beginWith(const char* que) const;
        bool endWith(const char* que) const;
        bool include(const SElement& elem) const;
        bool include(const SObjPtr& obj) const;
        bool match(const char* s) const;
        bool match(const Regex& reg) const;
        size_t indexOf(const SObjPtr& obj) const;
        size_t find(const char* s) const;

        /*
        * Cast
        */
        SObjPtr& object();
        const SObjPtr& object() const; 
        Char& character();
        const Char& character() const;
        MapData<String, SObjPtr>& pair();
        const MapData<String, SObjPtr>& pair() const;


        SNumber& number();
        const SNumber& number() const;
        SString& string();
        const SString& string() const;
        SDate& date();
        const SDate& date() const;
        SData& data();
        const SData& data() const;
        SArray& array();
        const SArray& array() const;
        SDictionary& dict();
        const SDictionary& dict() const;


        SFigure& figure();
        const SFigure& figure() const;
        SCanvas& canvas();
        const SCanvas& canvas() const;

        template<class Cls>
        Cls& as() {
            if (instanceOf<SVoid>(element.obj)) return any().ref<Cls>();
            else if (instanceOf<Cls>(element.obj)) return *dynamic_cast<Cls*>(element.obj);
            else {
                //throw CastException(castErrorText(getClass(), ));
            }
        }
        template<class Cls>
        const Cls& as() const {
            if (instanceOf<SVoid>(element.obj)) return any().ref<Cls>();
            else if (instanceOf<Cls>(element.obj)) return *dynamic_cast<const Cls*>(element.obj);
            else {
                //Cls cls; throw CastException(castErrorText(getClass(), cls.getClass()));
            }
        }

        template<class Ret, class... Args>
        SFunction<Ret, Args...>& function();

        sbyte byteValue() const;
        subyte ubyteValue() const;
        sshort shortValue() const;
        sushort ushortValue() const;
        int intValue() const;
        unsigned uintValue() const;
        sinteger integer() const;
        size_t sizeValue() const;
        float floatValue() const;
        double doubleValue() const;

        operator bool()const;
        operator sbyte() const;
        operator subyte() const;
        operator sshort() const;
        operator sushort() const;
        operator int() const;
        operator unsigned() const;
        operator long long() const;
        operator size_t() const;
        operator float() const;
        operator double() const;

        operator const char*()const;


        operator SObjPtr& ();
        operator const SObjPtr& () const;
        operator Char& ();
        operator const Char& () const;
        operator MapData<String, SObjPtr>& ();
        operator const MapData<String, SObjPtr>& () const;

        operator const SNumber& () const;
        operator SNumber& ();
        operator const SString& () const; 
        operator SString& ();
        operator const SDate& () const;
        operator SDate& ();
        operator const SData& () const;
        operator SData& ();
        operator const SArray& () const;
        operator SArray& ();
        operator const SDictionary& () const;
        operator SDictionary& ();

        /*
        * Compare
        */
        bool operator < (const SElement& elem) const;
        bool operator < (const SObjPtr& obj) const;
        bool operator <= (const SElement& elem) const;
        bool operator <= (const SObjPtr& obj) const;
        bool operator > (const SElement& elem) const;
        bool operator > (const SObjPtr& obj) const;
        bool operator >= (const SElement& elem) const;
        bool operator >= (const SObjPtr& obj) const;
        bool operator == (const char *s) const;
        bool operator == (const int i) const;
        bool operator == (const SElement& elem) const;
        bool operator == (const SObjPtr& obj) const; 
        bool operator != (const SElement& elem) const;
        bool operator != (const SObjPtr& obj) const;
    };
    
    /**
    * @SObjPtr
    * \~english @brief Basic object pointer
    */
    class SLIB_DLL SObjPtr {
        friend SElement;
        friend SIterator;
        friend SCIterator;
    public:
        SObject *instance;

    private:
        void copy(const slib::SObjPtr& obj);
        void move(slib::SObjPtr& obj);
        void assign(const slib::SObjPtr& obj);
        void share();
        void release();

    public:
        static SObjPtr toObj(const char* s);

    public:
        /*
        * Constructor/Destructor
        */
        SObjPtr();
        SObjPtr(const bool n);
        SObjPtr(const sbyte n);
        SObjPtr(const subyte n);
        SObjPtr(const sshort n);
        SObjPtr(const sushort n);
        SObjPtr(const int n);
        SObjPtr(const unsigned n);
        SObjPtr(const size_t ui);
#ifndef LINUX_OS
        SObjPtr(const long i);
#ifndef MAC_OS
        SObjPtr(const unsigned long ui);
#endif
#endif
        SObjPtr(const long long i);
#ifdef MAC_OS
        SObjPtr(const unsigned long long ui);
#endif
#ifdef LINUX_OS
        SObjPtr(const sinteger i);
#endif
        SObjPtr(const float n);
        SObjPtr(const double n);

        SObjPtr(const Fraction<sint>& n);
        SObjPtr(const Complex<float>& n);
        SObjPtr(const SNumber& n);
        SObjPtr(SNumber* n);

        SObjPtr(const char* s);
        SObjPtr(const String& s);
        SObjPtr(const SString& s);
        SObjPtr(SString* s);


        SObjPtr(const STime& t);
        SObjPtr(const SDate& d);

        SObjPtr(const SData& d);

        SObjPtr(const stringarray& array);
        SObjPtr(std::initializer_list<SObjPtr> li);
        SObjPtr(const SArray &a);
        SObjPtr(SArray* a);
        SObjPtr(const sattribute& attr);
        SObjPtr(std::initializer_list<Pair<String, SObjPtr>> li);
        SObjPtr(const SDictionary& d);
        SObjPtr(SDictionary* d);
        template<class Ret, class... Args>
        SObjPtr(const SFunction<Ret, Args...> &func) {
            instance = new SFunction<Ret, Args...>(func);
        }

        SObjPtr(const SFile& f);
        SObjPtr(SFile *f);

        SObjPtr(const SFigure& f);
        SObjPtr(const SCanvas& c);

        SObjPtr(const STable& t);

        SObjPtr(const ArrayIterator<SObjPtr> &ait);
        SObjPtr(const Char ch);
        SObjPtr(const MapIterator<String, SObjPtr> &mit);

        SObjPtr(const SElement& elem);
        SObjPtr(const SObjPtr& obj);


        ~SObjPtr();

        /*
        * Assign
        */
        SObjPtr& operator=(const SObjPtr& obj);
        
        /*
        * Arithmetic / Operator
        */

        SObjPtr& operator++();
        SObjPtr& operator--();
        SObjPtr operator-() const;
        SObjPtr operator+(const char* s) const;
        SObjPtr operator+(const slib::String& s) const;
        SObjPtr operator+(const int i) const;
        SObjPtr operator+(const SElement& e) const;
        SObjPtr operator+(const SObjPtr obj) const;
        SObjPtr operator-(const int i) const;
        SObjPtr operator-(const SObjPtr obj) const;
        SObjPtr operator*(const int i) const;
        SObjPtr operator*(const SObjPtr obj) const;
        SObjPtr operator/(const int i) const;
        SObjPtr operator/(const SObjPtr obj) const;
        SObjPtr operator%(const int i) const;
        SObjPtr operator%(const SObjPtr obj) const;

        SObjPtr& operator+=(const char* s);
        SObjPtr& operator+=(const slib::String& s);
        SObjPtr& operator+=(const SObjPtr obj);

        /*
        * Getter
        */
        size_t size() const;
        size_t length() const;
        suint type() const;


        Array<String> split(const char* sep, const bool trim = true, const bool ignore_quot = false, const bool dequote = true) const;
        Map<String, String> parse(const char* sep, const char* delim, const bool trim = true, const bool ignore_quot = false, bool dequote = true) const;
        

        stringarray keyset(bool sort = false) const;

        String getClass() const;
        SObjPtr clone() const;

        
        /*
        * Setter
        */
        void resize(const size_t sz);

        /*
        * Access
        */
        SElement operator[](const int i);
        const SElement operator[](const int i) const;
        SElement operator[](const char* s);
        const SElement operator[](const char* s) const;
        SElement operator[](const String& s);
        const SElement operator[](const String& s) const;
        SElement operator[](const SElement& obj);
        const SElement operator[](const SElement& obj) const;
        SElement operator[](const SObjPtr obj);
        const SElement operator[](const SObjPtr obj) const;
        SIterator begin();
        SCIterator begin() const;
        SIterator end();
        SCIterator end() const;

        /*
       * Editor
       */
        SObjPtr& add(const SObjPtr& obj);
        SObjPtr& append(const SObjPtr& obj);
        SObjPtr& append(const SArray& array);
        SObjPtr& insert(const int idx, const SObjPtr& obj);
        //SObjPtr& insert(const Pair<> idx, const SObjPtr& obj);
        SObjPtr& set(const String &key, const SObjPtr& obj);
        SObjPtr& remove(const size_t off, const size_t len);
        SObjPtr& erase(const SObjPtr& obj);
        void clear();

        /*
        * Checker
        */
        bool empty() const;
        
        bool hasKey(const char* key) const;

        bool isNull() const;
        bool isNum() const;
        bool isStr() const;
        bool isDate() const;
        bool isData() const;
        bool isArray() const;
        bool isDict() const;
        bool isText() const;
        bool isFile() const;
        bool isTable() const;
        bool isFunc() const;
        

        bool isImg() const;
        bool isCnvs() const;
        bool isFig() const;

        
        bool beginWith(const char* que) const;
        bool endWith(const char* que) const;
        bool include(const SElement& elem) const;
        bool include(const SObjPtr& obj) const;
        bool match(const slib::Regex &reg) const;
        bool equal(const slib::Regex& reg) const;
        bool match(const char* s) const;
        size_t indexOf(const SObjPtr& obj) const;
        size_t find(const char* s) const;



        
        /*
        * IO
        */

        void open(const char* path);
        void load(const char* path);
        void save(const char* path);


        sbyte byteValue() const;
        subyte ubyteValue() const;
        sshort shortValue() const;
        sushort ushortValue() const;
        int intValue() const;
        unsigned uintValue() const;
        sinteger integer() const;
        size_t sizeValue() const;
        float floatValue() const;
        double doubleValue() const;
        String toString(const char* format = nullptr) const;
        SVoid& any();
        const SVoid& any() const;
        SNumber& number();
        const SNumber& number() const;
        SString& string();
        const SString& string() const;
        SDate& date();
        const SDate& date() const;
        SData& data();
        const SData& data() const;
        SArray& array();
        const SArray& array() const;
        SDictionary& dict();
        const SDictionary& dict() const;
        SText& text();
        const SText& text() const;
        SFile& file();
        const SFile& file() const;
        STable& table();
        const STable& table() const;
        template<class Ret, class... Args>
        SFunction<Ret, Args...>& function() {
            if (instance) return *dynamic_cast<SFunction<Ret, Args...>*>(instance);
            throw CastException("");
        }


        SFigure& figure();
        const SFigure& figure() const;
        SCanvas& canvas();
        const SCanvas& canvas() const;
        SImage& image();
        const SImage& image() const;







        operator bool() const;
        operator sbyte() const;
        operator subyte() const;
        operator sshort() const;
        operator sushort() const;
        operator int() const;
        operator unsigned() const;
        operator long long() const;
        operator size_t() const;
        operator float() const;
        operator double() const;

        operator const char* () const;
        //operator String() const;
        operator SString& ();
        operator SArray& ();
        operator SDictionary& ();
        operator STable& ();
        operator const SString&() const;
        operator const SArray& () const;
        operator const SDictionary& () const;
        operator const STable& () const;




        template<class Cls>
        Cls& as() {
            if (instanceOf<SVoid>(instance)) return any().ref<Cls>();
            else if (instanceOf<Cls>(instance)) return *dynamic_cast<Cls*>(instance);
            else {
                //Cls cls; throw CastException(castErrorText(getClass(), cls.getClass()));
            }
        }
        template<class Cls>
        const Cls& as() const {
            if (instanceOf<SVoid>(instance)) return any().ref<Cls>();
            else if (instanceOf<Cls>(instance)) return *dynamic_cast<const Cls*>(instance);
            else {
                //Cls cls; throw CastException(castErrorText(getClass(), cls.getClass()));
            }
        }
        template<class Ret, class... Args >
        Ret operator()(Args... args) {
            if (isFunc()) return function<Ret, Args...>()(args...);
            else throw CastException("");
        }

        /*
        * Compare
        */
        bool operator < (const int n) const;
        bool operator < (const size_t n) const;
        bool operator < (const float n) const;
        bool operator < (const double n) const;
        bool operator < (const SElement& elem) const;
        bool operator < (const SObjPtr& obj) const;
        bool operator <= (const SElement& elem) const;
        bool operator <= (const SObjPtr& obj) const;
        bool operator > (const int n) const;
        bool operator > (const size_t n) const;
        bool operator > (const float n) const;
        bool operator > (const double n) const;
        bool operator > (const SElement& elem) const;
        bool operator > (const SObjPtr& obj) const; 
        bool operator >= (const SElement& elem) const;
        bool operator >= (const SObjPtr& obj) const;
        bool operator == (const char* s) const;
        bool operator == (const int n) const;
        bool operator == (const size_t n) const;
        bool operator == (const float n) const;
        bool operator == (const double n) const;
        bool operator == (const SElement& elem) const;
        bool operator == (const SObjPtr& obj) const;
        bool operator != (const SElement& elem) const;
        bool operator != (const SObjPtr& obj) const;
    };
    extern SLIB_DLL bool operator < (const int n, const SObjPtr& obj);
    extern SLIB_DLL bool operator < (const float n, const SObjPtr& obj);
    extern SLIB_DLL bool operator < (const double n, const SObjPtr& obj);
    extern SLIB_DLL bool operator > (const int n, const SObjPtr& obj);
    extern SLIB_DLL bool operator > (const float n, const SObjPtr& obj);
    extern SLIB_DLL bool operator > (const double n, const SObjPtr& obj);
    extern SLIB_DLL bool operator == (const int n, const SObjPtr& obj);
    extern SLIB_DLL bool operator == (const float n, const SObjPtr& obj);
    extern SLIB_DLL bool operator == (const double n, const SObjPtr& obj);
    extern SLIB_DLL SObjPtr operator+(const char* s, const SElement& e);
    extern SLIB_DLL SObjPtr operator+(const char* s, const SObjPtr& obj);

    namespace smath {
        extern SLIB_DLL SObjPtr abs(const SObjPtr& obj);
    }

    enum class SLIB_DLL ITER_TYPE : subyte {
        ARRAY = 1,
        UTF8 = 2,
        MAP = 3,
    };
    class SLIB_DLL SIterator {
    public:
        union SLIB_DLL Iter {
            ArrayIterator<SObjPtr> arrit;
            Utf8Iterator u8it;
            MapIterator<String, SObjPtr> mapit;

            Iter() { arrit = ArrayIterator<SObjPtr>(); }
            ~Iter() {}
        };
        void copyIter(Iter& dest, const Iter& src, ITER_TYPE type) {
            switch (type) {
            case ITER_TYPE::ARRAY:
                dest.arrit = src.arrit;
                break;
            case ITER_TYPE::UTF8:
                dest.u8it = src.u8it;
                break;
            case ITER_TYPE::MAP:
                dest.mapit = src.mapit;
                break;
            }
        }
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef SElement value_type;
        typedef std::ptrdiff_t difference_type;
        typedef Iter* pointer;
        typedef SElement& reference;

    private:
        ITER_TYPE itype;
        Iter iter;
        SElement elem;

    public:
        SIterator(ArrayIterator<SObjPtr> ait);
        SIterator(Utf8Iterator uit);
        SIterator(MapIterator<String, SObjPtr> mit);

        SIterator(const SIterator& it);
        ~SIterator();
        SIterator& operator=(const SIterator& it);
        reference operator *();
        pointer operator ->();
        reference operator [](std::ptrdiff_t diff);
        SIterator& operator ++();
        SIterator operator ++(int);
        SIterator& operator --();
        SIterator operator --(int);
        SIterator& operator +=(std::ptrdiff_t diff);
        SIterator& operator -=(std::ptrdiff_t diff);
        SIterator operator +(::std::ptrdiff_t diff);
        SIterator operator -(::std::ptrdiff_t diff);
        std::ptrdiff_t operator -(SIterator it);
        void swap(SIterator sit1, SIterator sit2);
        bool operator ==(const SIterator& sit) const;
        bool operator !=(const SIterator& sit) const;
        bool operator <(const SIterator& sit) const;
        bool operator <=(const SIterator& sit) const;
        bool operator >(const SIterator& sit) const;
        bool operator >=(const SIterator& sit) const;
    };
    class SLIB_DLL SCIterator {
    public:
        union SLIB_DLL CIter {
            ArrayCIterator<SObjPtr> arrit;
            Utf8CIterator u8it;
            MapCIterator<String, SObjPtr> mapit;
            CIter() { arrit = ArrayCIterator<SObjPtr>(); }
            ~CIter() {}
        };
        void copyIter(CIter& dest, const CIter& src, ITER_TYPE type) {
            switch (type) {
            case ITER_TYPE::ARRAY:
                dest.arrit = src.arrit;
                break;
            case ITER_TYPE::UTF8:
                dest.u8it = src.u8it;
                break;
            case ITER_TYPE::MAP:
                dest.mapit = src.mapit;
                break;
            }
        }
    public:
        typedef ::std::random_access_iterator_tag iterator_category;
        typedef const SElement value_type;
        typedef ::std::ptrdiff_t difference_type;
        typedef CIter* pointer;
        typedef SElement& reference;
    private:
        ITER_TYPE itype;
        CIter iter;
        SElement elem;

    public:
        SCIterator(ArrayCIterator<SObjPtr> ait);
        SCIterator(Utf8CIterator uit);
        SCIterator(MapCIterator<String, SObjPtr> mit);
        SCIterator(const SCIterator& it);
        ~SCIterator();
        SCIterator& operator=(const SCIterator& it);
        reference operator *();
        pointer operator ->();
        reference& operator [](::std::ptrdiff_t diff);
        SCIterator& operator ++();
        SCIterator operator ++(int);
        SCIterator& operator --();
        SCIterator operator --(int);
        SCIterator& operator +=(::std::ptrdiff_t diff);
        SCIterator& operator -=(::std::ptrdiff_t diff);
        SCIterator operator +(::std::ptrdiff_t diff);
        SCIterator operator -(::std::ptrdiff_t diff);
        std::ptrdiff_t operator -(SCIterator it);
        void swap(SCIterator sit1, SCIterator sit2);
        bool operator ==(const SCIterator& sit) const;
        bool operator !=(const SCIterator& sit) const;
        bool operator <(const SCIterator& sit) const;
        bool operator <=(const SCIterator& sit) const;
        bool operator >(const SCIterator& sit) const;
        bool operator >=(const SCIterator& sit) const;
    };

    template<class Ret, class... Args>
    SFunction<Ret, Args...>& SElement::function() {
        if (isObj()) return *dynamic_cast<SFunction<Ret, Args...>*>(element.obj->instance);
        else throw CastException("");
    }

    extern SLIB_DLL String toString(const SNumber& obj, const char* format = nullptr);
    extern SLIB_DLL String toString(const SString& obj, const char* format = nullptr);
    extern SLIB_DLL String toString(const SDate& obj, const char* format = nullptr);
    extern SLIB_DLL String toString(const SData& obj, const char* format = nullptr);
    extern SLIB_DLL String toString(const SArray& obj, const char* format = nullptr);
    extern SLIB_DLL String toString(const SDictionary& obj, const char* format = nullptr);

    extern SLIB_DLL String toString(const SElement& obj, const char* format = nullptr);
    extern SLIB_DLL String toString(const SObjPtr& obj, const char* format = nullptr);
    extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::SElement& elem);
    extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::SObjPtr& obj);

}

extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::SNumber& obj);
extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::SString& obj);
extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::SDate& obj);
extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::SData& obj);
extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::SArray& obj);
extern SLIB_DLL std::ostream& operator<<(std::ostream& os, const slib::SDictionary& obj);


using siter = slib::SIterator;
using sciter = slib::SCIterator;
using sobj = slib::SObjPtr;
#define snull slib::SObjPtr()

#endif