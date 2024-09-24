#include "sobj/sobjptr.h"
slib::SIterator::SIterator(slib::ArrayIterator<slib::SObjPtr> ait) {
    itype = ITER_TYPE::ARRAY; iter.arrit = ait;
}
slib::SIterator::SIterator(slib::Utf8Iterator uit) {
    itype = ITER_TYPE::UTF8; iter.u8it = uit;
}
slib::SIterator::SIterator(slib::MapIterator<slib::String, slib::SObjPtr> mit) {
    itype = ITER_TYPE::MAP; iter.mapit = mit;
}

slib::SIterator::SIterator(const slib::SIterator &it) {
    itype = it.itype;
    SIterator::copyIter(iter, it.iter, itype);
    //memcpy(&iter, &it.iter, sizeof(Iter));
}
slib::SIterator::~SIterator() {}
slib::SIterator& slib::SIterator::operator=(const slib::SIterator &it) { 
    itype = it.itype; 
    SIterator::copyIter(iter, it.iter, itype);
    //memcpy(&iter, &it.iter, sizeof(Iter)); 
    return *this;
}
slib::SElement& slib::SIterator::operator *() { 
    switch (itype) {
    case ITER_TYPE::ARRAY:
        elem.eclass = SElement::E_CLASS::OBJECT;
        elem.element.obj = iter.arrit.ptr();
        break;
    case ITER_TYPE::UTF8:
        elem.eclass = SElement::E_CLASS::CHAR;
        elem.element.ch = *iter.u8it;
        break;
    case ITER_TYPE::MAP:
        elem.eclass = SElement::E_CLASS::PAIR;
        elem.element.pair = iter.mapit.ptr();
        break;
    default:
        break;
    }
    return elem;
}
slib::SIterator::Iter* slib::SIterator::operator ->() { return &iter; }
slib::SElement& slib::SIterator::operator [](std::ptrdiff_t diff) {
    switch (itype) {
    case ITER_TYPE::ARRAY:
    {
        iter.arrit += diff;
        elem.eclass = SElement::E_CLASS::OBJECT;
        elem.element.obj = iter.arrit.ptr();
        break;
    }
    case ITER_TYPE::UTF8:
    {
        iter.u8it += diff;
        elem.eclass = SElement::E_CLASS::CHAR;
        elem.element.ch = *iter.u8it;
        break;
    }
    case ITER_TYPE::MAP:
    {
        iter.mapit += diff;
        elem.eclass = SElement::E_CLASS::PAIR;
        elem.element.pair = iter.mapit.ptr();
        break;
    }
    default:
        break;
    }
    return elem;
}
slib::SIterator& slib::SIterator::operator ++() {
    switch (itype) {
    case ITER_TYPE::ARRAY:
        ++iter.arrit;
        break;
    case ITER_TYPE::UTF8:
        ++iter.u8it;
        break;
    case ITER_TYPE::MAP:
        ++iter.mapit;
    default:
        break;
    }
    return *this;
}
slib::SIterator slib::SIterator::operator ++(int) { auto sit = *this; ++sit; return sit; }
slib::SIterator& slib::SIterator::operator --() {
    switch (itype) {
    case ITER_TYPE::ARRAY:
        --iter.arrit;
        break;
    case ITER_TYPE::UTF8:
        --iter.u8it;
        break;
    default:
        break;
    }
    return *this;
}
slib::SIterator slib::SIterator::operator --(int) { auto sit = *this; --sit; return sit; }
slib::SIterator &slib::SIterator::operator+=(std::ptrdiff_t diff) {
    switch (itype) {
    case ITER_TYPE::ARRAY:
        iter.arrit += diff;
        break;
    case ITER_TYPE::UTF8:
        iter.u8it += diff;
        break;
    case ITER_TYPE::MAP:
        iter.mapit += diff;
    default:
        break;
    }
    return *this;
}
slib::SIterator &slib::SIterator::operator-=(std::ptrdiff_t diff) {
    switch (itype) {
    case ITER_TYPE::ARRAY:
        iter.arrit -= diff;
        break;
    case ITER_TYPE::UTF8:
        iter.u8it -= diff;
        break;
    default:
        break;
    }
    return *this;
}
slib::SIterator slib::SIterator::operator +(std::ptrdiff_t diff) { auto sit = *this; sit+=diff; return sit; }
slib::SIterator slib::SIterator::operator -(std::ptrdiff_t diff) { auto sit = *this; sit-=diff; return sit; }
std::ptrdiff_t slib::SIterator::operator-(SIterator it) {
    if (itype != it.itype) throw Exception();
    switch (itype) {
    case ITER_TYPE::ARRAY:
        return iter.arrit - it.iter.arrit;
    case ITER_TYPE::UTF8:
        return iter.u8it - it.iter.u8it;
    default:
        return 0;
    }
}
void slib::SIterator::swap(SIterator it1, SIterator it2) {
    if (it1.itype != it2.itype) throw Exception();
    switch (it1.itype) {
    case ITER_TYPE::ARRAY:
        it1.iter.arrit.swap(it1.iter.arrit, it2.iter.arrit);
        break;
    case ITER_TYPE::UTF8:
        it1.iter.u8it.swap(it1.iter.u8it, it2.iter.u8it);
        break;
    case ITER_TYPE::MAP:
        it1.iter.mapit.swap(it1.iter.mapit, it2.iter.mapit);
        break;
    default:
        break;
    }
}
bool slib::SIterator::operator ==(const slib::SIterator &it) const {
    if (itype != it.itype) return false;
    switch (itype) {
    case ITER_TYPE::ARRAY:
        return iter.arrit == it.iter.arrit;
    case ITER_TYPE::UTF8:
        return iter.u8it == it.iter.u8it;
    case ITER_TYPE::MAP:
        return iter.mapit == it.iter.mapit;
    default:
        return false;
    }
}
bool slib::SIterator::operator !=(const slib::SIterator &it) const { return !(*this==it); }
bool slib::SIterator::operator <(const slib::SIterator &it) const {
    if (itype != it.itype) return false;
    switch (itype) {
    case ITER_TYPE::ARRAY:
        return iter.arrit < it.iter.arrit;
    case ITER_TYPE::UTF8:
        return iter.u8it < it.iter.u8it;
    case ITER_TYPE::MAP:
        return iter.mapit < it.iter.mapit;
    default:
        return false;
    }
}
bool slib::SIterator::operator <=(const slib::SIterator& it) const { return (*this) < it || (*this) == it; }
bool slib::SIterator::operator >(const slib::SIterator& it) const { return it < (*this); }
bool slib::SIterator::operator >=(const slib::SIterator& it) const { return it < (*this) || (*this) == it; }


slib::SCIterator::SCIterator(slib::ArrayCIterator<slib::SObjPtr> ait) {
    itype = ITER_TYPE::ARRAY; iter.arrit = ait;
}
slib::SCIterator::SCIterator(slib::Utf8CIterator uit) {
    itype = ITER_TYPE::UTF8; iter.u8it = uit;
}
slib::SCIterator::SCIterator(slib::MapCIterator<slib::String, slib::SObjPtr> mit) {
    itype = ITER_TYPE::MAP; iter.mapit = mit;
}

slib::SCIterator::SCIterator(const slib::SCIterator& it) {
    itype = it.itype; 
    SCIterator::copyIter(iter, it.iter, itype);
    //memcpy(&iter, &it.iter, sizeof(CIter));
}
slib::SCIterator::~SCIterator() {}
slib::SCIterator& slib::SCIterator::operator=(const slib::SCIterator& it) {
    itype = it.itype;
    SCIterator::copyIter(iter, it.iter, itype);
    //memcpy(&iter, &it.iter, sizeof(CIter)); return *this;
    return *this;
}
slib::SElement &slib::SCIterator::operator *() {
    switch (itype) {
    case ITER_TYPE::ARRAY:
        elem.eclass = SElement::E_CLASS::OBJECT;
        elem.element.obj = const_cast<SObjPtr *>(iter.arrit.ptr());
        break;
    case ITER_TYPE::UTF8:
        elem.eclass = SElement::E_CLASS::CHAR;
        elem.element.ch = iter.u8it.ptr();
        break;
    case ITER_TYPE::MAP:
        elem.eclass = SElement::E_CLASS::PAIR;
        elem.element.pair = const_cast<MapData<String, SObjPtr> *>(iter.mapit.ptr());
        break;
    default:
        break;
    }
    return elem;
}
slib::SCIterator::CIter* slib::SCIterator::operator ->() { return &iter; }
slib::SElement &slib::SCIterator::operator [](std::ptrdiff_t diff) {
    switch (itype) {
    case ITER_TYPE::ARRAY:
    {
        iter.arrit += diff;
        elem.eclass = SElement::E_CLASS::OBJECT;
        elem.element.obj = const_cast<SObjPtr*>(iter.arrit.ptr());
        break;
    }
    case ITER_TYPE::UTF8:
    {
        iter.u8it += diff;
        elem.eclass = SElement::E_CLASS::CHAR;
        elem.element.ch = iter.u8it.ptr();
        break;
    }
    case ITER_TYPE::MAP:
    {
        iter.mapit += diff;
        elem.eclass = SElement::E_CLASS::PAIR;
        elem.element.pair = const_cast<MapData<String, SObjPtr> *>(iter.mapit.ptr());
        break;
    }
    default:
        break;
    }
    return elem;
}
slib::SCIterator& slib::SCIterator::operator ++() {
    switch (itype) {
    case ITER_TYPE::ARRAY:
        ++iter.arrit;
        break;
    case ITER_TYPE::UTF8:
        ++iter.u8it;
        break;
    case ITER_TYPE::MAP:
        ++iter.mapit;
    default:
        break;
    }
    return *this;
}
slib::SCIterator slib::SCIterator::operator ++(int) { auto sit = *this; ++sit; return sit; }
slib::SCIterator& slib::SCIterator::operator --() {
    switch (itype) {
    case ITER_TYPE::ARRAY:
        --iter.arrit;
        break;
    case ITER_TYPE::UTF8:
        --iter.u8it;
        break;
    default:
        break;
    }
    return *this;
}
slib::SCIterator slib::SCIterator::operator --(int) { auto sit = *this; --sit; return sit; }
slib::SCIterator& slib::SCIterator::operator+=(std::ptrdiff_t diff) {
    switch (itype) {
    case ITER_TYPE::ARRAY:
        iter.arrit += diff;
        break;
    case ITER_TYPE::UTF8:
        iter.u8it += diff;
        break;
    case ITER_TYPE::MAP:
        iter.mapit += diff;
    default:
        break;
    }
    return *this;
}
slib::SCIterator& slib::SCIterator::operator-=(std::ptrdiff_t diff) {
    switch (itype) {
    case ITER_TYPE::ARRAY:
        iter.arrit -= diff;
        break;
    case ITER_TYPE::UTF8:
        iter.u8it -= diff;
        break;
    default:
        break;
    }
    return *this;
}
slib::SCIterator slib::SCIterator::operator +(std::ptrdiff_t diff) { auto sit = *this; sit += diff; return sit; }
slib::SCIterator slib::SCIterator::operator -(std::ptrdiff_t diff) { auto sit = *this; sit -= diff; return sit; }
std::ptrdiff_t slib::SCIterator::operator-(SCIterator it) {
    if (itype != it.itype) throw Exception();
    switch (itype) {
    case ITER_TYPE::ARRAY:
        return iter.arrit - it.iter.arrit;
    case ITER_TYPE::UTF8:
        return iter.u8it - it.iter.u8it;
    default:
        return 0;
    }
}
void slib::SCIterator::swap(SCIterator it1, SCIterator it2) {
    if (it1.itype != it2.itype) throw Exception();
    switch (it1.itype) {
    case ITER_TYPE::ARRAY:
        it1.iter.arrit.swap(it1.iter.arrit, it2.iter.arrit);
        break;
    case ITER_TYPE::UTF8:
        it1.iter.u8it.swap(it1.iter.u8it, it2.iter.u8it);
        break;
    case ITER_TYPE::MAP:
        it1.iter.mapit.swap(it1.iter.mapit, it2.iter.mapit);
        break;
    default:
        break;
    }
}
bool slib::SCIterator::operator ==(const slib::SCIterator& it) const {
    if (itype != it.itype) return false;
    switch (itype) {
    case ITER_TYPE::ARRAY:
        return iter.arrit == it.iter.arrit;
    case ITER_TYPE::UTF8:
        return iter.u8it == it.iter.u8it;
    case ITER_TYPE::MAP:
        return iter.mapit == it.iter.mapit;
    default:
        return false;
    }
}
bool slib::SCIterator::operator !=(const slib::SCIterator& it) const { return !(*this == it); }
bool slib::SCIterator::operator <(const slib::SCIterator& it) const {
    if (itype != it.itype) return false;
    switch (itype) {
    case ITER_TYPE::ARRAY:
        return iter.arrit < it.iter.arrit;
    case ITER_TYPE::UTF8:
        return iter.u8it < it.iter.u8it;
    case ITER_TYPE::MAP:
        return iter.mapit < it.iter.mapit;
    default:
        return false;
    }
}
bool slib::SCIterator::operator <=(const slib::SCIterator& it) const { return (*this) < it || (*this) == it; }
bool slib::SCIterator::operator >(const slib::SCIterator& it) const { return it < (*this); }
bool slib::SCIterator::operator >=(const slib::SCIterator& it) const { return it < (*this) || (*this) == it; }
