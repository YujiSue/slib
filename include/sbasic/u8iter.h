#ifndef SLIB_U8ITER_H
#define SLIB_U8ITER_H
#include "sbasic/char.h"
namespace slib {
    /**
    * @class Utf8Iterator
    * \~english @brief Iterator for UTF-8 string (char arrays)
    * \~japanese @brief UTF-8文字列用のイテレータ
    */
    class SLIB_DLL Utf8Iterator {
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef char value_type;
        typedef std::ptrdiff_t difference_type;
        typedef char* pointer;
        typedef Char reference;
        Char _char;

    public:
        Utf8Iterator();
        Utf8Iterator(char* s, String& b);
        Utf8Iterator(const Utf8Iterator &uit);
        ~Utf8Iterator();
        Utf8Iterator& operator=(const Utf8Iterator &uit);
        Char operator *();
        char* operator ->();
        Char operator[](std::ptrdiff_t diff);
        Utf8Iterator &operator ++();
        Utf8Iterator operator ++(int) const;
        Utf8Iterator &operator --();
        Utf8Iterator operator --(int) const;
        Utf8Iterator &operator +=(std::ptrdiff_t diff);
        Utf8Iterator &operator -=(std::ptrdiff_t diff);
        Utf8Iterator operator +(std::ptrdiff_t diff) const;
        Utf8Iterator operator -(std::ptrdiff_t diff) const;
        difference_type operator-(Utf8Iterator it) const;
        char* ptr();
        String& str();
        size_t index() const;
        size_t length() const;
        void swap(Utf8Iterator uit1, Utf8Iterator uit2);
        bool operator ==(const Utf8Iterator &uit) const;
        bool operator !=(const Utf8Iterator &uit) const;
        bool operator <(const Utf8Iterator &uit) const;
        bool operator <=(const Utf8Iterator &uit) const;
        bool operator >(const Utf8Iterator &uit) const;
        bool operator >=(const Utf8Iterator &uit) const;
    };
    
    class SLIB_DLL Utf8CIterator {
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef const char value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const char* pointer;
        typedef Char reference;
        const char* first;
    protected:
        Char _char;
    public:
        Utf8CIterator();
        Utf8CIterator(const char* s, const String& b);
        Utf8CIterator(const Utf8CIterator &uit);
        ~Utf8CIterator();
        Utf8CIterator& operator=(const Utf8CIterator &uit);
        const Char operator *() const;
        const char* operator ->() const;
        const Char operator[](std::ptrdiff_t diff);
        Utf8CIterator &operator ++();
        Utf8CIterator operator ++(int) const;
        Utf8CIterator &operator --();
        Utf8CIterator operator --(int) const;
        Utf8CIterator &operator +=(std::ptrdiff_t diff);
        Utf8CIterator &operator -=(std::ptrdiff_t diff);
        Utf8CIterator operator +(std::ptrdiff_t diff) const;
        Utf8CIterator operator -(std::ptrdiff_t diff) const;
        difference_type operator-(Utf8CIterator it) const;
        const char* ptr();
        const String& str();
        size_t index() const;
        size_t length() const;
        void swap(Utf8CIterator uit1, Utf8CIterator uit2);
        bool operator ==(const Utf8CIterator &uit) const;
        bool operator !=(const Utf8CIterator &uit) const;
        bool operator <(const Utf8CIterator &uit) const;
        bool operator <=(const Utf8CIterator &uit) const;
        bool operator >(const Utf8CIterator &uit) const;
        bool operator >=(const Utf8CIterator &uit) const;
    };
}
#endif