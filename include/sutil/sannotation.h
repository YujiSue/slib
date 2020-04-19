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

		void insert(srange site, const Data& dat);
		void append(srange site, const Data& dat);
		void overwrite(srange site, const Data& dat);
		void cancel(srange site, const Data& dat);
		void merge(SAnnotation& annot);
		void expand(sint site, sint len);
		void erase(srange site);
		Array<annot_dat<Data> *> operator[](sint site);
		Array<annot_dat<Data>*> operator[](srange site);
		Array<annot_dat<Data>*> at(sint site);
		Array<annot_dat<Data>*> at(srange site);
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
	void SAnnotation<Data>::insert(srange site, const Data& dat) {
		_annotation.add(annot_dat<Data>(site, dat));
		_annotation.sort();
	}
	template<class Data>
	void SAnnotation<Data>::append(srange site, const Data& dat) {
		bool over = false;
		SAnnotation<Data> tmp;
		sforeach(_annotation) {
			if (E_.site.include(site)) {
				if (E_.site.begin < site.begin) tmp.add(srange(E_.site.begin, site.begin - 1), E_.data);
				if (site.end < E_.site.end) tmp.add(srange(site.end + 1, E_.site.end), E_.data);
				E_.site = site; E_.data += dat; over = true;
				break;
			}
			else if (E_.site.include(site.begin)) {
				if (E_.site.begin < site.begin) tmp.add(srange(E_.site.begin, site.begin - 1), E_.data);
				E_.site.begin = site.begin; E_.data += dat; over = true;
				site.begin = E_.site.end + 1;
			}
			else if (E_.site.include(site.end)) {
				if (site.begin < E_.site.begin) tmp.add(srange(site.begin, E_.site.begin - 1), dat);
				if (site.end < E_.site.end) tmp.add(srange(site.end + 1, E_.site.end), E_.data);
				E_.data += dat; E_.site.end = site.end; over = true;
				break;
			}
		}
		if (over) {
			if (!tmp.empty()) this->merge(tmp);
		}
		else this->insert(site, dat);
	}
	template<class Data>
	void SAnnotation<Data>::overwrite(srange site, const Data& dat) {
		bool over = false;
		SAnnotation<Data> tmp;
		sforeach(_annotation) {
			if (E_.site.include(site)) {
				if (E_.site.begin < site.begin) tmp.add(srange(E_.site.begin, site.begin - 1), E_.data);
				if (site.end < E_.site.end) tmp.add(srange(site.end + 1, E_.site.end), E_.data);
				E_.site = site; E_.data /= dat; over = true;
				break;
			}
			else if (E_.site.include(site.begin)) {
				if (E_.site.begin < site.begin) tmp.add(srange(E_.site.begin, site.begin - 1), E_.data);
				E_.site.begin = site.begin; E_.data /= dat; over = true;
				site.begin = E_.site.end + 1;
			}
			else if (E_.site.include(site.end)) {
				if (site.begin < E_.site.begin) tmp.add(srange(site.begin, E_.site.begin - 1), dat);
				if (site.end < E_.site.end) tmp.add(srange(site.end + 1, E_.site.end), E_.data);
				E_.data /= dat; E_.site.end = site.end; over = true;
				break;
			}
		}
		if (over) {
			if (!tmp.empty()) this->merge(tmp);
		}
		else this->insert(site, dat);
	}
	template<class Data>
	void SAnnotation<Data>::cancel(srange site, const Data& dat) {
		bool over = false;
		SAnnotation<Data> tmp;
		sforeach(_annotation) {
			if (E_.site.include(site)) {
				if (E_.site.begin < site.begin) tmp.add(srange(E_.site.begin, site.begin - 1), E_.data);
				if (site.end < E_.site.end) tmp.add(srange(site.end + 1, E_.site.end), E_.data);
				E_.site = site; E_.data -= dat; over = true;
				break;
			}
			else if (E_.site.include(site.begin)) {
				if (E_.site.begin < site.begin) tmp.add(srange(E_.site.begin, site.begin - 1), E_.data);
				E_.site.begin = site.begin; E_.data -= dat; over = true;
				site.begin = E_.site.end + 1;
			}
			else if (E_.site.include(site.end)) {
				if (site.end < E_.site.end) tmp.add(srange(site.end + 1, E_.site.end), E_.data);
				E_.data -= dat; E_.site.end = site.end; over = true;
				break;
			}
		}
		if (over) {
			if (!tmp.empty()) this->merge(tmp);
		}
	}
	template<class Data>
	void SAnnotation<Data>::merge(SAnnotation<Data>& annot) {
		_annotaion.append(annot._annotation);
		_annotation.sort();
	}
	template<class Data>
	void SAnnotation<Data>::expand(sint site, sint len) {
		sforeach(_annotation) {
			if (E_.site.include(site)) E_.expand(len);
			else if (site < E_.site) E_.shift(len);
		}
	}
	template<class Data>
	void SAnnotation<Data>::erase(srange site) {
		auto len = -site.length(true);
		sforeach(_annotation) {
			if (E_.site.include(site)) E_.expand(len);
			else if (site < E_.site) E_.shift(len);
		}
	}
	template<class Data>
	Array<annot_dat<Data>*> SAnnotation<Data>::operator[](sint site) { return this->at(site); }
	template<class Data>
	Array<annot_dat<Data>*> SAnnotation<Data>::operator[](srange site) { return this->at(site); }
	template<class Data>
	Array<annot_dat<Data>*> SAnnotation<Data>::at(sint site) { 
		Array<annot_dat<Data>*> tmp;
		sforeach(_annotation) { if (E_.site.overlap(site)) tmp.add(&E_); }
		return tmp;
	}
	template<class Data>
	Array<annot_dat<Data>*> SAnnotation<Data>::at(srange site) {
		Array<annot_dat<Data>*> tmp;
		sforeach(_annotation) { if (E_.site.overlap(site)) tmp.add(&E_); }
		return tmp;
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
