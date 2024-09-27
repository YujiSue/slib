#ifndef SLIB_SEARCH_H
#define SLIB_SEARCH_H
#include "sbasic/ptr.h"
#include "sbasic/farray.h"
#include "sbasic/string.h"
namespace slib {
    enum class MATCH {
        EXACT = 0x00,
        START = 0x01,
        END = 0x02,
        PARTIAL = 0x04
    };
    extern SLIB_DLL Regex makeRegexQuery(const char* que, MATCH m, bool ignore = true);

    template<typename T, class CONTAINER>
    inline void _bisearch(slib::Range<slib::ArrayIterator<T>>& range, CONTAINER& array, const T& val, std::function<bool(const T&, const T&)> comparer) {
        if (range.end == range.begin + 1) return;
        if ((int)(range.end - range.begin) < 16) {
            if (comparer(*range.begin, val)) {
                range.end = range.begin;
                --range.begin;
            }
            else if (comparer(val, *(range.end - 1))) {
                range.begin = range.end - 1;
            }
            else {
                sforin(it, range.begin, range.end) {
                    if (comparer($_, val)) {
                        range.begin = it - 1;
                        range.end = it;
                        break;
                    }
                }
            }
            return;
        }
        else {
            slib::ArrayIterator<T> med = range.begin + (int)(range.end - range.begin) / 2;
            if (comparer(*med, val)) range.end = med;
            else range.begin = med;
            _bisearch(range, array, val, comparer);
        }
    }

    template<typename T, class CONTAINER>
    ArrayIterator<T> bisearch(CONTAINER& array, const T& val, std::function<bool(const T&, const T&)> comparer) {
        if (array.empty()) throw NullException(nullErrorText("Array content"));
        if (comparer(*array.begin(), val)) return array.begin();
        else if (comparer(val, *(array.end() - 1))) return array.end();
        else {
            Range<ArrayIterator<T>> range(array.begin(), array.end());
            _bisearch(range, array, val, comparer);
            return range.end;
        }
    }

    template<class MATCH, size_t S>
    class PMA {
        typedef PMA<MATCH, S>* pma_ptr;
    public:
        Array<MATCH> matches;
        FArray<pma_ptr, S> children;
    public:
        PMA() { 
            children.reset(nullptr);
        }
        ~PMA() { clear(); }
        void add(const MATCH &m) { matches.add(m); }
        void clear() {
            children.reset(nullptr);
            matches.clear();
        }
    };
    template<typename MATCH, size_t S>
    class Trie {
    protected:
        Array<Pointer<PMA<MATCH, S>>> _pmas;
        ArrayIterator<Pointer<PMA<MATCH, S>>> _end;
        size_t _qlength;
    public:
        Trie() {
            _pmas.add(new PMA<MATCH, S>());
            _pmas[0]->children[0] = (PMA<MATCH, S>*)_pmas[0];
            _end = _pmas.end();
            _qlength = 1;
        }
        Trie(const Trie<MATCH, S> &trie) {}
        virtual ~Trie() {}
        Trie<MATCH, S>& operator=(const Trie<MATCH, S>& trie) {
            return *this;
        }
        void setData(subyte* data, const size_t sz, std::function<int(subyte *)> &assign, std::function<MATCH(int)>& matching) {
            if (sz < _qlength) throw Exception();
            auto len = sz - _qlength + 1;
            sforin(i, 0, len) {
                auto now = root();
                auto p = data;
                sforin(j, 0, _qlength) {
                    auto idx = assign(p);
                    if (!(now->children[idx])) {
                        auto nxt = newPMA();
                        now->children[idx] = nxt;
                    }
                    now = now->children[idx];
                    ++p;
                }
                now->add(matching(i));
                ++data;
            }
        }
        void complete() {
            std::queue<PMA<MATCH, S>*> stack;
            auto root = (PMA<MATCH, S>*)_pmas[0];
            sforin(i, 1, S) {
                if (!(root->children[i])) root->children[i] = root;
                else {
                    root->children[i]->children[0] = root;
                    stack.push(root->children[i]);
                }
            }
            while (!stack.empty()) {
                auto now = stack.front();
                stack.pop();
                sforin(i, 1, S) {
                    if (now->children[i]) {
                        auto next = now->children[0];
                        while (!(next->children[i])) next = next->children[0];
                        now->children[i]->children[0] = next->children[i];
                        if (next->children[i]->matches.size())
                            now->children[i]->matches.append(next->children[i]->matches);
                        stack.push(now->children[i]);
                    }
                }
            }
        }
        void setQuerySize(const size_t q) { _qlength = q; }
        PMA<MATCH, S>* root() { return (PMA<MATCH, S>*)_pmas[0]; }
        PMA<MATCH, S>* newPMA() {
            PMA<MATCH, S>* p;
            if (_end < _pmas.end()) {
                p = (PMA<MATCH, S>*)(*_end);
                ++_end;
            }
            else {
                _pmas.add(new PMA<MATCH, S>());
                p = (PMA<MATCH, S>*)_pmas[-1];
                _end = _pmas.end();
            }
            return p;
        }
        void reserve(size_t sz) { 
            auto current = _end - _pmas.begin();
            _pmas.reserve(sz); 
            _end = _pmas.begin() + current;
        }
        void clear() {
            sforin(it, _pmas.begin(), _end) $_->clear();
            _pmas[0]->children[0] = (PMA<MATCH, S>*)_pmas[0];
            _end = _pmas.begin() + 1;
        }
        bool operator<(const Trie<MATCH, S>& trie) const { return false; }
        bool operator==(const Trie<MATCH, S>& trie) const { return false; }
    };
}

#endif