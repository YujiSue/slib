#ifndef SBIO_SVARIANT_H
#define SBIO_SVARIANT_H

#include "sutil/sthread.h"
#include "sbioinfo/sbannot.h"
#include "sbioinfo/sbioseq.h"
#include "sbioinfo/svarutil.h"

namespace slib {
    namespace sbio {
        
        class SBIOINFO_DLL SVariant;
        class SBIOINFO_DLL SVarList;
        
        struct SBIOINFO_DLL svar_data {
            sushort type;
            sbpos pos[2]; // 0: Primary, 1: Secondary (for description of complex insertion or rearrangement)
            String alt;
            sushort read[2]; // 0: Positive strand, 1: Negative strand
            double qual;
            
            svar_data();
            svar_data(const sbpos &pos);
            svar_data(const svar_data &v);
            ~svar_data();
            
			svar_data& operator = (const sbpos& pos);
            svar_data &operator = (const svar_data &v);
            svar_data &operator += (const svar_data &v);
            
            void comp();
            sint total() const;
            double bias() const;
            double phred() const;
            bool comparable(const svar_data &v) const;
            bool lt(const svar_data &var, size_t dist) const;
            bool equal(const svar_data &var, size_t dist) const;
            
            bool operator < (const svar_data &v) const;
            bool operator ==(const svar_data &v) const;
        };
        typedef Array<svar_data> vararray;

		struct scn_data {
			float depth[2], ndepth[2], bgdepth[2], bgndepth[2], ratio[2], frequency;

			scn_data();
			scn_data(const scn_data &cn);
			~scn_data();

			scn_data &operator=(const scn_data &cn);

		};
        
        class SBIOINFO_DLL SVariant : public svar_data {
			friend SVarIO;
            friend SVarList;
			friend SVarFilter;
            
        public:
			sushort flag;
			String name, ref[2];
			scn_data copy;
			bool homo;
			Array<gene_site> genes;
			stringarray mutants;
			sdict attribute;
            
        public:
            SVariant();
            SVariant(sushort f, const svar_data &v);
			//SVariant(sobj obj);
            SVariant(const SVariant &var);
            ~SVariant();
            
			SVariant& operator=(const svar_data& var);
            SVariant &operator=(const SVariant &var);
            
            bool comparable(const SVariant *var) const;
            bool lt(const SVariant *var, size_t dist) const;
            bool equal(const SVariant *var, size_t dist) const;

			void set(sobj& obj);
			sobj toObj();

            bool operator <(const SVariant &var) const;
            bool operator ==(const SVariant &var) const;
        };
        
		#define svar sptr<SVariant>
		typedef Array<svar> svararray;
        
        class SBIOINFO_DLL SVarList : public Array<svar> {
			friend SVarIO;
			friend SVariant;
			friend SVarFilter;

        public:
			sushort list_type;
			sint refnum;
			stringarray refname;
			intarray reflength;
			String list_name, ref_ver;
			sdict list_attribute;
            
            
        public:
			SVarList();
            SVarList(const char *s);
            SVarList(const SVarList &vl);
            virtual ~SVarList();
            
            void load(const char *path, SBSeqList* ref = nullptr, sattribute *converter = nullptr);
            void save(const char *path, SBSeqList* ref = nullptr);
            
			void tidy(size_t s = -1);
            void clearAll();
        };
    }
}
#endif