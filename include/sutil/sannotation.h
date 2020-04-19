#ifndef SLIB_SANNOTATION_H
#define SLIB_SANNOTATION_H

#include "sbasic/array.h"

namespace slib {
    template<class Data>
    struct annot_dat {
		srange site;
        Data data;
        
		annot_dat();
		annot_dat(const srange &s, const Data &d);
		annot_dat(const annot_dat& annot);
		~annot_dat();
        
		annot_dat& operator=(const annot_dat& annot);
		bool operator<(const annot_dat& annot) const;
		bool operator==(const annot_dat& annot) const;
    };

	template<class Data>
	class SAnnotation {
	protected:
		Array<annot_dat<Data>> _annotation;

	public:
		SAnnotation();
		~SAnnotation();

		void insert(const srange &site, const Data& dat);
		void append(const srange& site, const Data& dat);
		void overwrite(const srange& site, const Data& dat);
		void cancel(const srange& site, const Data& dat);

		void expand(srange site, sint len);
		void erase(srange site);
		Range<sarr_iter<annot_dat<Data>>> operator[](const int& site);
		Range<sarr_iter<annot_dat<Data>>> operator[](const srange& site);
		Range<sarr_iter<annot_dat<Data>>> at(const int& site);
		Range<sarr_iter<annot_dat<Data>>> at(const srange& site);
		bool isAnnotated(const int& site) const;
		bool isAnnotated(const srange& site) const;
	};
    
    /*============================================================*/
	template<class Data>
	annot_dat<Data>::annot_dat() {}
	template<class Data>
	annot_dat<Data>::annot_dat(const srange& s, const Data& d) : site(s), data(d) {}
	template<class Data>
	annot_dat<Data>::annot_dat(const annot_dat<Data>& annot) : site(annot.site), data(annot.data) {}
	template<class Data>
	annot_dat<Data>::~annot_dat() {}

	template<class Data>
	annot_dat<Data>& annot_dat<Data>::operator=(const annot_dat<Data>& annot) {
		site = annot.site; data = annot.data; return *this;
	}
	template<class Data>
	bool annot_dat<Data>::operator<(const annot_dat<Data>& annot) const { return site < annot.site; }
	template<class Data>
	bool annot_dat<Data>::operator==(const annot_dat<Data>& annot) const { return site == annot.site && data == annot.data; }
	template<class Data>
	SAnnotation<Data>::SAnnotation() {}
	template<class Data>
	SAnnotation<Data>::~SAnnotation() {}
	template<class Data>
	void SAnnotation<Data>::insert(const srange& site, const Data& dat) {
		if (_annotation.empty()) _annotation.add(annot_dat<Data>(site, dat));
		else {
			auto it = _annotation.begin(), end = _annotation.end();
			if (site < it->site) _annotation.insert(0, annot_dat<Data>(site, dat));
			else {
				++it;
				while (it < end) {
					if ((it - 1)->site < site && site < it->site) {
						_annotation.insert(it, annot_dat<Data>(site, dat)); return;
					}
					++it;
				}
				if ((end - 1)->site < site) _annotation.add(annot_dat<Data>(site, dat));
			}	
		}
	}
	template<class Data>
	void SAnnotation<Data>::append(const srange& site, const Data& dat) {
		if (_annotation.empty()) _annotation.add(annot_dat<Data>(site, dat));
		else {
			auto it = _annotation.begin(), end = _annotation.end();
			if (site.end < it->site.begin) _annotation.insert(0, annot_dat<Data>(site, dat));
			else {
				while (it < end) {
					if (E_.site.overlap(site)) {

					}
					else if (it < end - 1 && 
						it->site.end < site.begin && site.end < (it + 1)->site.begin) 
						_annotation.insert(it + 1, annot_dat<Data>(site, dat));
					++it;
				}
			}
		}
	}
	template<class Data>
	void SAnnotation<Data>::overwrite(const srange& site, const Data& dat) {

	}
	template<class Data>
	void SAnnotation<Data>::cancel(const srange& site, const Data& dat) {

	}
	template<class Data>
	Range<sarr_iter<annot_dat<Data>>> SAnnotation<Data>::operator[](const int& site) { return at(site); }
	template<class Data>
	Range<sarr_iter<annot_dat<Data>>> SAnnotation<Data>::operator[](const srange& site) { return at(site); }
	template<class Data>
	Range<sarr_iter<annot_dat<Data>>> SAnnotation<Data>::at(const int& site) {


		auto it = _annotation.begin(), end = _annotation.end();


		if (site < E_.site) return Range<sarr_iter<annot_dat<Data>>>(it, it);
		if ((end - 1)->site < site) {
			if ((end - 1)->site.overlap(site)) return Range<sarr_iter<annot_dat<Data>>>(end-1, end-1);
			else return Range<sarr_iter<annot_dat<Data>>>(end, end);
		}



		


	}
	template<class Data>
	Range<sarr_iter<annot_dat<Data>>> SAnnotation<Data>::at(const srange& site) {

	}
	template<class Data>
	bool SAnnotation<Data>::isAnnotated(const int& site) const {
		auto range = at(site);
		sforin(it, range.begin, range.end) {
			if (E_,site.overlap(site)) return true;
		}
		return false;
	}
	template<class Data>
	bool SAnnotation<Data>::isAnnotated(const srange& site) const {
		auto pos = at(site);
		sforin(pit, pos.begin, pos.end) {
			if (pit->overlap(site)) return true;
		}
		return false;
	}
}

#endif
