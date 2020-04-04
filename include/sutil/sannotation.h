#ifndef SLIB_SANNOTATION_H
#define SLIB_SANNOTATION_H

#include "sbasic/array.h"

namespace slib {
    template<class Site, class Data>
    struct annot_dat {
		Site site;
        Data data;
        
		annot_dat();
		annot_dat(Site s, Data d);
		annot_dat(const annot_dat& annot);
		~annot_dat();
        
		annot_dat& operator=(const annot_dat& annot);
		bool operator<(const annot_dat& annot) const;
		bool operator==(const annot_dat& annot) const;
    };

	template<class Site, class Data>
	class SAnnotation {
	protected:
		Array<annot_dat<Site, Data>> _annotation;

	public:
		SAnnotation();
		~SAnnotation();

		void append(const annot_dat<Site, Data>& dat);
		void insert(const annot_dat<Site, Data>& dat);
		void overwrite(const annot_dat<Site, Data>& dat);
		void remove(const Site& site);
		Range<sarr_iter<annot_dat<Site, Data>>> operator[](const Site& site);
		Range<sarr_citer<annot_dat<Site, Data>>> operator[](const Site& site) const;
		Range<sarr_iter<annot_dat<Site, Data>>> at(const Site& site);
		Range<sarr_citer<annot_dat<Site, Data>>> at(const Site& site) const;
		bool isAnnotated(const Site& site) const;
	};
    
    /*============================================================*/
	template<class Site, class Data>
	annot_dat<Site, Data>::annot_dat() {}
	template<class Site, class Data>
	annot_dat<Site, Data>::annot_dat(Site s, Data d) : site(s), data(d) {}
	template<class Site, class Data>
	annot_dat<Site, Data>::annot_dat(const annot_dat<Site, Data>& annot) : site(annot.site), data(annot.data) {}
	template<class Site, class Data>
	annot_dat<Site, Data>::~annot_dat() {}

	template<class Site, class Data>
	annot_dat<Site, Data>& annot_dat<Site, Data>::operator=(const annot_dat<Site, Data>& annot) {
		site = annot.site; data = annot.data; return *this;
	}
	template<class Site, class Data>
	bool annot_dat<Site, Data>::operator<(const annot_dat<Site, Data>& annot) const { return site < annot.site; }
	template<class Site, class Data>
	bool annot_dat<Site, Data>::operator==(const annot_dat<Site, Data>& annot) const { return site == annot.site && data == annot.data; }
	template<class Site, class Data>
	SAnnotation<Site, Data>::SAnnotation() {}
	template<class Site, class Data>
	SAnnotation<Site, Data>::~SAnnotation() {}
	template<class Site, class Data>
	void SAnnotation<Site, Data>::append(const annot_dat<Site, Data>& dat) { _annotation.add(dat); }
	template<class Site, class Data>
	void SAnnotation<Site, Data>::insert(const annot_dat<Site, Data>& dat) {
		

	}
	template<class Site, class Data>
	void SAnnotation<Site, Data>::overwrite(const annot_dat<Site, Data>& dat) {

	}
	template<class Site, class Data>
	void SAnnotation<Site, Data>::remove(const Site& site) {

	}
	template<class Site, class Data>
	Range<sarr_iter<annot_dat<Site, Data>>> SAnnotation<Site, Data>::operator[](const Site& site) { return at(site); }
	template<class Site, class Data>
	Range<sarr_citer<annot_dat<Site, Data>>> SAnnotation<Site, Data>::operator[](const Site& site) const { return at(site); }
	template<class Site, class Data>
	Range<sarr_iter<annot_dat<Site, Data>>> SAnnotation<Site, Data>::at(const Site& site) {
		auto it = _annotation.begin(), end = _annotation.end();
		if (site < E_.site) return Range<sarr_iter<annot_dat<Site, Data>>>(it, it);
		if ((end - 1)->site < site) {
			if ((end - 1)->site.overlap(site)) return Range<sarr_iter<annot_dat<Site, Data>>>(end-1, end-1);
			else return Range<sarr_iter<annot_dat<Site, Data>>>(end, end);
		}



		


	}
	template<class Site, class Data>
	Range<sarr_citer<annot_dat<Site, Data>>> SAnnotation<Site, Data>::at(const Site& site) const {

	}
	template<class Site, class Data>
	bool SAnnotation<Site, Data>::isAnnotated(const Site& site) const {
		auto pos = at(site);
		sforin(pit, pos.begin, pos.end) {
			if (pit->overlap(site)) return true;
		}
		return false;
	}
}

#endif
