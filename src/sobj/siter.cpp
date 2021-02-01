#include "sobj/sobject.h"
#include "sobj/sstring.h"
#include "sobj/sdict.h"

using namespace slib;

slib::SIterator::SIterator(slib::SArrayIterator<slib::SObjPtr> ait) {
	_type = ARRAY_OBJ;
    new(&_it._ait) SArrayIterator<sobj>(ait);
}
slib::SIterator::SIterator(slib::SUtf8Iterator uit) {
	_type = STRING_OBJ;
    new(&_it._uit) SUtf8Iterator(uit);
    _obj = SChar(*uit);
}
slib::SIterator::SIterator(slib::SMapIterator<String, slib::SObjPtr> mit) {
	_type = DICT_OBJ;
    new(&_it._mit) SMapIterator<String, slib::SObjPtr>(mit);
    _obj = SPair(*mit);
}
slib::SIterator::SIterator(const slib::SIterator &it) : _type(it._type), _obj(it._obj) {
    switch (_type) {
        case STRING_OBJ:
            _it._uit = it._it._uit;
            break;
        case ARRAY_OBJ:
            _it._ait = it._it._ait;
            break;
		case ROW_OBJ:
			_it._ait = it._it._ait;
			break;
        case DICT_OBJ:
            _it._mit = it._it._mit;
            break;
        default:
            break;
    }
}
slib::SIterator::~SIterator() {
    switch (_type) {
        case STRING_OBJ:
            _it._uit.~SUtf8Iterator();
            break;
        case ARRAY_OBJ:
            _it._ait.~SArrayIterator<sobj>();
            break;
		case ROW_OBJ:
			_it._ait.~SArrayIterator<sobj>();
			break;
        case DICT_OBJ:
            _it._mit.~SMapIterator<String, sobj>();
            break;
        default:
            break;
    }
}

slib::SIterator& slib::SIterator::operator=(const slib::SIterator &it) {
    _type = it._type; _obj = it._obj;
    switch (_type) {
        case STRING_OBJ:
            _it._uit = it._it._uit;
            break;
        case ARRAY_OBJ:
            _it._ait = it._it._ait;
            break;
		case ROW_OBJ:
			_it._ait = it._it._ait;
			break;
		case DICT_OBJ:
            _it._mit = it._it._mit;
            break;
        default:
            break;
    }
    return *this;
}
slib::SObjPtr &slib::SIterator::operator *() {
    switch (_type) {
        case STRING_OBJ:
            _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            return *_it._ait;
            break;
		case ROW_OBJ:
			return *_it._ait;
			break;
		case DICT_OBJ:
            _obj.pair() = *_it._mit;
            break;
        default:
            break;
    }
    return _obj;
}
slib::SObjPtr *slib::SIterator::operator ->() {
    switch (_type) {
        case STRING_OBJ:
            _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            return &(*_it._ait);
            break;
        case DICT_OBJ:
            _obj.pair() = *_it._mit;
            break;
        default:
            break;
    }
    return &_obj;
}
slib::SObjPtr &slib::SIterator::operator [](std::ptrdiff_t diff) {
    switch (_type) {
        case STRING_OBJ:
            _it._uit += diff; _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            _it._ait += diff; return *_it._ait;
            break;
        case DICT_OBJ:
            _it._mit += diff; _obj.pair() = *_it._mit;
            break;
        default:
            break;
    }
    return _obj;
}
slib::SIterator &slib::SIterator::operator ++() {
    switch (_type) {
        case STRING_OBJ:
            ++_it._uit; _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            ++_it._ait;
            break;
        case DICT_OBJ:
            ++_it._mit;
			_obj.pair() = *_it._mit;
            break;
        default:
            break;
    }
    return *this;
}
slib::SIterator slib::SIterator::operator ++(int) { auto sit = *this; ++sit; return sit; }
slib::SIterator &slib::SIterator::operator --() {
    switch (_type) {
        case STRING_OBJ:
            --_it._uit; _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            --_it._ait;
            break;
        case DICT_OBJ:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "Map iterator", "operator--");
            break;
        default:
            break;
    }
    return *this;
}
slib::SIterator slib::SIterator::operator --(int) { auto sit = *this; --sit; return sit; }
slib::SIterator &slib::SIterator::operator+=(std::ptrdiff_t diff) {
    switch (_type) {
        case STRING_OBJ:
            _it._uit+=diff; _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            _it._ait+=diff;
            break;
        case DICT_OBJ:
            _it._mit+=diff; _obj.pair() = *_it._mit;
            break;
        default:
            break;
    }
    return *this;
}
slib::SIterator &slib::SIterator::operator-=(std::ptrdiff_t diff) {
    switch (_type) {
        case STRING_OBJ:
            _it._uit-=diff; _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            _it._ait-=diff;
            break;
        case DICT_OBJ:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "Map iterator", "operator-=");
            break;
        default:
            break;
    }
    return *this;
}
slib::SIterator slib::SIterator::operator +(std::ptrdiff_t diff) { auto sit = *this; sit+=diff; return sit; }
slib::SIterator slib::SIterator::operator -(std::ptrdiff_t diff) { auto sit = *this; sit-=diff; return sit; }
sinteger slib::SIterator::operator-(SIterator it) {
    switch (_type) {
        case STRING_OBJ:
            return _it._uit-it._it._uit;
            break;
        case ARRAY_OBJ:
            return _it._ait-it._it._ait;
            break;
        case DICT_OBJ:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "Map iterator", "operator-");
            break;
        default:
            break;
    }
    return 0;
}
void slib::SIterator::swap(SIterator sit1, SIterator sit2) {
    switch (sit1._type) {
        case STRING_OBJ:
        {
            auto utmp = sit1._it._uit; sit1._it._uit = sit2._it._uit; sit2._it._uit = utmp;
            break;
        }
        case ARRAY_OBJ:
        {
            auto atmp = sit1._it._ait; sit1._it._ait = sit2._it._ait; sit2._it._ait = atmp;
            break;
        }
        case DICT_OBJ:
        {
            auto mtmp = sit1._it._mit; sit1._it._mit = sit2._it._mit; sit2._it._mit = mtmp;
            break;
        }
        default:
            break;
    }
}
bool slib::SIterator::operator ==(const slib::SIterator &sit) const {
    if (_type == sit._type) {
        switch (_type) {
            case STRING_OBJ:
                return _it._uit==sit._it._uit;
                break;
            case ARRAY_OBJ:
                return _it._ait==sit._it._ait;
                break;
            case DICT_OBJ:
                return _it._mit==sit._it._mit;
                break;
            default:
                break;
        }
    }
    return false;
}
bool slib::SIterator::operator !=(const slib::SIterator &sit) const { return !(*this==sit); }
bool slib::SIterator::operator <(const slib::SIterator &sit) const {
    switch (_type) {
        case STRING_OBJ:
            return _it._uit < sit._it._uit;
            break;
        case ARRAY_OBJ:
            return _it._ait < sit._it._ait;
            break;
        case DICT_OBJ:
            return _it._mit < sit._it._mit;
            break;
        default:
            break;
    }
    return false;
}
bool slib::SIterator::operator <=(const slib::SIterator &sit) const {
    switch (_type) {
        case STRING_OBJ:
            return _it._uit <= sit._it._uit;
            break;
        case ARRAY_OBJ:
            return _it._ait <= sit._it._ait;
            break;
        case DICT_OBJ:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "Map iterator", "operator<=");
            break;
        default:
            break;
    }
    return false;
}
bool slib::SIterator::operator >(const slib::SIterator &sit) const {
    switch (_type) {
        case STRING_OBJ:
            return _it._uit > sit._it._uit;
            break;
        case ARRAY_OBJ:
            return _it._ait > sit._it._ait;
            break;
        case DICT_OBJ:
            throw sit._it._mit < _it._mit;
            break;
        default:
            break;
    }
    return false;
}
bool slib::SIterator::operator >=(const slib::SIterator &sit) const {
    switch (_type) {
        case STRING_OBJ:
            return _it._uit >= sit._it._uit;
            break;
        case ARRAY_OBJ:
            return _it._ait >= sit._it._ait;
            break;
        case DICT_OBJ:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "Map iterator", "operator>=");
            break;
        default:
            break;
    }
    return false;
}

slib::SCIterator::SCIterator(SArrayCIterator<SObjPtr> ait) {
    _type =ARRAY_OBJ;
    new(&_it._ait) SArrayCIterator<slib::SObjPtr>(ait);
}
slib::SCIterator::SCIterator(SUtf8CIterator uit) {
    _type =STRING_OBJ;
    new(&_it._uit) SUtf8CIterator(uit);
    _obj = SChar(*_it._uit);
}
slib::SCIterator::SCIterator(SMapCIterator<String, SObjPtr> mit) {
    _type =DICT_OBJ;
    new(&_it._mit) SMapCIterator<String, slib::SObjPtr>(mit);
    _obj = SPair(*_it._mit);
}
slib::SCIterator::SCIterator(const slib::SCIterator &it) : _type(it._type), _obj(it._obj) {
    switch (_type) {
        case STRING_OBJ:
            _it._uit = it._it._uit;
            break;
        case ARRAY_OBJ:
            _it._ait = it._it._ait;
            break;
        case DICT_OBJ:
            _it._mit = it._it._mit;
            break;
        default:
            break;
    }
}
slib::SCIterator::~SCIterator() {
    switch (_type) {
        case STRING_OBJ:
            _it._uit.~SUtf8CIterator();
            break;
        case ARRAY_OBJ:
            _it._ait.~SArrayCIterator<slib::SObjPtr>();
            break;
        case DICT_OBJ:
            _it._mit.~SMapCIterator<String, slib::SObjPtr>();
            break;
        default:
            break;
    }
}

slib::SCIterator& slib::SCIterator::operator=(const SCIterator &it) {
    _type = it._type; _obj = it._obj;
    switch (_type) {
        case STRING_OBJ:
            _it._uit = it._it._uit;
            break;
        case ARRAY_OBJ:
            _it._ait = it._it._ait;
            break;
        case DICT_OBJ:
            _it._mit = it._it._mit;
            break;
        default:
            break;
    }
    return *this;
}
const slib::SObjPtr &slib::SCIterator::operator *() {
    switch (_type) {
        case STRING_OBJ:
            _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            return *_it._ait;
            break;
        case DICT_OBJ:
            _obj.pair() = *_it._mit;
            break;
        default:
            break;
    }
    return _obj;
}
const slib::SObjPtr *slib::SCIterator::operator ->() {
    switch (_type) {
        case STRING_OBJ:
            _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            return &(*_it._ait);
            break;
        case DICT_OBJ:
            _obj.pair() = *_it._mit;
            break;
        default:
            break;
    }
    return &_obj;
}
const slib::SObjPtr &slib::SCIterator::operator [](std::ptrdiff_t diff) {
    switch (_type) {
        case STRING_OBJ:
            _it._uit += diff; _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            _it._ait += diff; return *_it._ait;
            break;
        case DICT_OBJ:
            _it._mit += diff; _obj.pair() = *_it._mit;
            break;
        default:
            break;
    }
    return _obj;
}
slib::SCIterator &slib::SCIterator::operator ++() {
    switch (_type) {
        case STRING_OBJ:
            ++_it._uit; _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            ++_it._ait;
            break;
        case DICT_OBJ:
            ++_it._mit; _obj.pair() = *_it._mit;
            break;
        default:
            break;
    }
    return *this;
}
slib::SCIterator slib::SCIterator::operator ++(int) { auto sit = *this; ++sit; return sit; }
slib::SCIterator &slib::SCIterator::operator --() {
    switch (_type) {
        case STRING_OBJ:
            --_it._uit; _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            --_it._ait;
            break;
        case DICT_OBJ:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "Map iterator", "operator--");
            break;
        default:
            break;
    }
    return *this;
}
slib::SCIterator slib::SCIterator::operator --(int) { auto sit = *this; --sit; return sit; }
slib::SCIterator &slib::SCIterator::operator+=(std::ptrdiff_t diff) {
    switch (_type) {
        case STRING_OBJ:
            _it._uit+=diff; _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            _it._ait+=diff;
            break;
        case DICT_OBJ:
            _it._mit+=diff; _obj.pair() = *_it._mit;
            break;
        default:
            break;
    }
    return *this;
}
slib::SCIterator &slib::SCIterator::operator-=(std::ptrdiff_t diff) {
    switch (_type) {
        case STRING_OBJ:
            _it._uit-=diff; _obj.character() = *_it._uit;
            break;
        case ARRAY_OBJ:
            _it._ait-=diff;
            break;
        case DICT_OBJ:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "Map iterator", "operator-=");
            break;
        default:
            break;
    }
    return *this;
}
slib::SCIterator slib::SCIterator::operator +(std::ptrdiff_t diff) { auto sit = *this; sit+=diff; return sit; }
slib::SCIterator slib::SCIterator::operator -(std::ptrdiff_t diff) { auto sit = *this; sit-=diff; return sit; }
sinteger slib::SCIterator::operator-(SCIterator it) {
    switch (_type) {
        case STRING_OBJ:
            return _it._uit-it._it._uit;
            break;
        case ARRAY_OBJ:
            return _it._ait-it._it._ait;
            break;
        case DICT_OBJ:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "Map iterator", "operator-");
            break;
        default:
            break;
    }
    return 0;
}
void slib::SCIterator::swap(slib::SCIterator sit1, slib::SCIterator sit2) {
    switch (sit1._type) {
        case STRING_OBJ:
        {
            auto utmp = sit1._it._uit; sit1._it._uit = sit2._it._uit; sit2._it._uit = utmp;
            break;
        }
        case ARRAY_OBJ:
        {
            auto atmp = sit1._it._ait; sit1._it._ait = sit2._it._ait; sit2._it._ait = atmp;
            break;
        }
        case DICT_OBJ:
        {
            auto mtmp = sit1._it._mit; sit1._it._mit = sit2._it._mit; sit2._it._mit = mtmp;
            break;
        }
        default:
            break;
    }
}
bool slib::SCIterator::operator ==(const slib::SCIterator &sit) const {
    if (_type == sit._type) {
        switch (_type) {
            case STRING_OBJ:
                return _it._uit==sit._it._uit;
                break;
            case ARRAY_OBJ:
                return _it._ait==sit._it._ait;
                break;
            case DICT_OBJ:
                return _it._mit==sit._it._mit;
                break;
            default:
                break;
        }
    }
    return false;
}
bool slib::SCIterator::operator !=(const slib::SCIterator &sit) const { return !(*this==sit); }
bool slib::SCIterator::operator <(const slib::SCIterator &sit) const {
    switch (_type) {
        case STRING_OBJ:
            return _it._uit < sit._it._uit;
            break;
        case ARRAY_OBJ:
            return _it._ait < sit._it._ait;
            break;
        case DICT_OBJ:
			return _it._mit < sit._it._mit;
            break;
        default:
            break;
    }
    return false;
}
bool slib::SCIterator::operator <=(const slib::SCIterator &sit) const {
    switch (_type) {
        case STRING_OBJ:
            return _it._uit <= sit._it._uit;
            break;
        case ARRAY_OBJ:
            return _it._ait <= sit._it._ait;
            break;
        case DICT_OBJ:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "Map iterator", "operator<=");
            break;
        default:
            break;
    }
    return false;
}
bool slib::SCIterator::operator >(const slib::SCIterator &sit) const {
    switch (_type) {
        case STRING_OBJ:
            return _it._uit > sit._it._uit;
            break;
        case ARRAY_OBJ:
            return _it._ait > sit._it._ait;
            break;
        case DICT_OBJ:
			return sit._it._mit < _it._mit;
            break;
        default:
            break;
    }
    return false;
}
bool slib::SCIterator::operator >=(const slib::SCIterator &sit) const {
    switch (_type) {
        case STRING_OBJ:
            return _it._uit >= sit._it._uit;
            break;
        case ARRAY_OBJ:
            return _it._ait >= sit._it._ait;
            break;
        case DICT_OBJ:
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "Map iterator", "operator>=");
            break;
        default:
            break;
    }
    return false;
}

