#ifndef SLIB_SANNOTATION_H
#define SLIB_SANNOTATION_H

#include "sbasic/array.h"

namespace slib {
    template<typename T, class Data>
    struct annot_dat {
		Range<T> site;
        Data data;
        
		annot_dat();
		annot_dat(const Range<T> &s, const Data &d);
		annot_dat(const annot_dat& annot);
		~annot_dat();
        
		annot_dat& operator=(const annot_dat& annot);
		bool operator<(const annot_dat& annot) const;
		bool operator==(const annot_dat& annot) const;
    };

	template<typename T, class Data>
	class SAnnotation {
	protected:
		Array<annot_dat<T, Data>> _annotation;

	public:
		SAnnotation();
		~SAnnotation();

		void insert(const Range<T> &site, const Data& dat);
		void append(const Range<T>& site, const Data& dat);
		void overwrite(const Range<T>& site, const Data& dat);
		void cancel(const Range<T>& site, const Data& dat);

		void expand(Range<T> site, sint len);
		void erase(Range<T> site);
		Range<sarr_iter<annot_dat<T, Data>>> operator[](const T& site);
		Range<sarr_iter<annot_dat<T, Data>>> operator[](const Range<T>& site);
		Range<sarr_iter<annot_dat<T, Data>>> at(const T& site);
		Range<sarr_iter<annot_dat<T, Data>>> at(const Range<T>& site);
		bool isAnnotated(const T& site) const;
		bool isAnnotated(const Range<T>& site) const;
	};
    
    /*============================================================*/
	template<typename T, class Data>
	annot_dat<T, Data>::annot_dat() {}
	template<typename T, class Data>
	annot_dat<T, Data>::annot_dat(const Range<T>& s, const Data& d) : site(s), data(d) {}
	template<typename T, class Data>
	annot_dat<T, Data>::annot_dat(const annot_dat<T, Data>& annot) : site(annot.site), data(annot.data) {}
	template<typename T, class Data>
	annot_dat<T, Data>::~annot_dat() {}

	template<typename T, class Data>
	annot_dat<T, Data>& annot_dat<T, Data>::operator=(const annot_dat<T, Data>& annot) {
		site = annot.site; data = annot.data; return *this;
	}
	template<typename T, class Data>
	bool annot_dat<T, Data>::operator<(const annot_dat<T, Data>& annot) const { return site < annot.site; }
	template<typename T, class Data>
	bool annot_dat<T, Data>::operator==(const annot_dat<T, Data>& annot) const { return site == annot.site && data == annot.data; }
	template<typename T, class Data>
	SAnnotation<T, Data>::SAnnotation() {}
	template<typename T, class Data>
	SAnnotation<T, Data>::~SAnnotation() {}
	template<typename T, class Data>
	void SAnnotation<T, Data>::insert(const Range<T>& site, const Data& dat) {
		if (_annotation.empty()) _annotation.add(annot_dat<T, Data>(site, dat));
		else {
			if (site < _annotation.first().site) _annotation.insert(0, annot_dat<T, Data>(site, dat));
			else {
				sforeach(_annotation) {
					if (E_.site < site) {
						_annotation.insert(it+1, annot_dat<T, Data>(site, dat));
					}
				}
			}	
		}
	}
	template<typename T, class Data>
	void SAnnotation<T, Data>::append(const Range<T>& site, const Data& dat) {
		if (_annotation.empty()) _annotation.add(annot_dat<T, Data>(site, dat));
		else {
			if (site.end < _annotation.first().site.begin) _annotation.insert(0, annot_dat<T, Data>(site, dat));
			else if (_annotation.last().site.end < site.begin) _annotation.add(annot_dat<T, Data>(site, dat));
			else {
				sforeach(_annotation) {
					if (E_.site.include(site)) {
						auto tmp = E_.data;
						E_.site.begin = site.end + 1;

					}
					else if (E_.site.overlap(site)) {
											}
					else {
						if (_annotation.begin() < it) {

						}
					}
				}
			}
		}
	}
	template<typename T, class Data>
	void SAnnotation<T, Data>::overwrite(const Range<T>& site, const Data& dat) {

	}
	template<typename T, class Data>
	void SAnnotation<T, Data>::cancel(const Range<T>& site, const Data& dat) {

	}
	template<typename T, class Data>
	Range<sarr_iter<annot_dat<T, Data>>> SAnnotation<T, Data>::operator[](const Site& site) { return at(site); }
	template<typename T, class Data>
	Range<sarr_citer<annot_dat<T, Data>>> SAnnotation<T, Data>::operator[](const Site& site) const { return at(site); }
	template<typename T, class Data>
	Range<sarr_iter<annot_dat<T, Data>>> SAnnotation<T, Data>::at(const Site& site) {
		auto it = _annotation.begin(), end = _annotation.end();
		if (site < E_.site) return Range<sarr_iter<annot_dat<Site, Data>>>(it, it);
		if ((end - 1)->site < site) {
			if ((end - 1)->site.overlap(site)) return Range<sarr_iter<annot_dat<Site, Data>>>(end-1, end-1);
			else return Range<sarr_iter<annot_dat<Site, Data>>>(end, end);
		}



		


	}
	template<typename T, class Data>
	Range<sarr_citer<annot_dat<T, Data>>> SAnnotation<T, Data>::at(const Site& site) const {

	}
	template<typename T, class Data>
	bool SAnnotation<T, Data>::isAnnotated(const Site& site) const {
		auto pos = at(site);
		sforin(pit, pos.begin, pos.end) {
			if (pit->overlap(site)) return true;
		}
		return false;
	}
}

#endif
