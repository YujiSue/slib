#ifndef SLIB_UTF8_H
#define SLIB_UTF8_H

#include "sbasic/char.h"

namespace slib {
    class String;
    
    #define su8_iter SUtf8Iterator
    #define su8_citer SUtf8CIterator
    
    class SLIB_DLL SUtf8Iterator {
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef Char value_type;
        typedef std::ptrdiff_t difference_type;
        typedef Char* pointer;
        typedef Char& reference;
        
    protected:
        Char _char;
        
    public:
        SUtf8Iterator();
        SUtf8Iterator(String *s, const char *c);
        SUtf8Iterator(const SUtf8Iterator &uit);
        ~SUtf8Iterator();
        
        SUtf8Iterator& operator=(const SUtf8Iterator &uit);
        reference operator *();
        pointer operator ->();
        reference operator[](std::ptrdiff_t diff);
        SUtf8Iterator &operator ++();
        SUtf8Iterator operator ++(int) const;
        SUtf8Iterator &operator --();
        SUtf8Iterator operator --(int) const;
        SUtf8Iterator &operator +=(std::ptrdiff_t diff);
        SUtf8Iterator &operator -=(std::ptrdiff_t diff);
        SUtf8Iterator operator +(std::ptrdiff_t diff) const;
        SUtf8Iterator operator -(std::ptrdiff_t diff) const;
        int operator-(SUtf8Iterator it) const;
        void swap(SUtf8Iterator uit1, SUtf8Iterator uit2);
        bool operator ==(const SUtf8Iterator &uit) const;
        bool operator !=(const SUtf8Iterator &uit) const;
        bool operator <(const SUtf8Iterator &uit) const;
        bool operator <=(const SUtf8Iterator &uit) const;
        bool operator >(const SUtf8Iterator &uit) const;
        bool operator >=(const SUtf8Iterator &uit) const;
    };
    
    class SLIB_DLL SUtf8CIterator {
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef const Char value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const Char* pointer;
        typedef const Char& reference;
        
    protected:
        Char _char;
        
    public:
        SUtf8CIterator();
        SUtf8CIterator(const String *s, const char *c);
        SUtf8CIterator(const SUtf8CIterator &uit);
        ~SUtf8CIterator();
        
        SUtf8CIterator& operator=(const SUtf8CIterator &uit);
        reference operator *() const;
        pointer operator ->() const;
        reference operator[](std::ptrdiff_t diff);
        SUtf8CIterator &operator ++();
        SUtf8CIterator operator ++(int) const;
        SUtf8CIterator &operator --();
        SUtf8CIterator operator --(int) const;
        SUtf8CIterator &operator +=(std::ptrdiff_t diff);
        SUtf8CIterator &operator -=(std::ptrdiff_t diff);
        SUtf8CIterator operator +(std::ptrdiff_t diff) const;
        SUtf8CIterator operator -(std::ptrdiff_t diff) const;
        int operator-(SUtf8CIterator it) const;
        void swap(SUtf8CIterator uit1, SUtf8CIterator uit2);
        bool operator ==(const SUtf8CIterator &uit) const;
        bool operator !=(const SUtf8CIterator &uit) const;
        bool operator <(const SUtf8CIterator &uit) const;
        bool operator <=(const SUtf8CIterator &uit) const;
        bool operator >(const SUtf8CIterator &uit) const;
        bool operator >=(const SUtf8CIterator &uit) const;
    };
}

#endif
