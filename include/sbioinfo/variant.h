#ifndef SBIO_VARIANT_H
#define SBIO_VARIANT_H
#include "sbasic/ptr.h"
#include "smath/geom.h"
#include "sutil/sthread.h"
#include "sbioinfo/annotation.h"
#include "sbioinfo/seq.h"
#include "sbioinfo/varutil.h"
namespace slib {
    namespace sbio {
        class SLIB_DLL Variant;
        class SLIB_DLL VarList;

        class SLIB_DLL SVar {
        public:
            sushort type;
            sbpos pos[2];
            sushort read[2];
            String alt;
            double qual;
        public:
            SVar();
            SVar(const sbpos& pos);
            SVar(const SVar& v);
            ~SVar();
            SVar& operator = (const sbpos& pos);
            SVar& operator = (const SVar& v);
            SVar& operator += (const SVar& v);
            int length() const;
            void categorize();
//            void shaping();
            void complement();
            bool isStrict() const;
            bool isComplex() const;
            sint total() const;
            double bias() const;
            bool comparable(const SVar& v) const;
            bool include(const SVar& v) const;
            //
            bool lt(const SVar& var, SVParam* par) const;
            bool equal(const SVar& var, SeqList* ref, SVParam *par) const;
            void merge(SVar& var, SVParam* par);
            //
            String toString(SeqList* ref = nullptr) const;
            void clear();
            bool operator < (const SVar& v) const;
            bool operator ==(const SVar& v) const;
        };

        class SLIB_DLL CNData {
        public:
            float depth[2], ndepth[2], copy;
        public:
            CNData();
            CNData(const CNData& cn);
            ~CNData();
            CNData& operator=(const CNData& cn);
        };
        
        
        class SLIB_DLL CNVar {
        public:
            sushort type;
            sbpos pos;
            CNData copy;
            double qual;
            
        public:
            CNVar();
            CNVar(const CNVar &var);
            ~CNVar();
            CNVar operator=(const CNVar& var);
            
        };

        class SLIB_DLL Variant {
        public:
            subyte type, genotype;
            sushort flag;
            sbpos pos[2];
            String varid, alt;
            sushort read[2];
            float depth[2][2]; // [0]: pos[0], [1]:pos[1], [n][0]:sample, [n][1]:control 
            float copy[2]; // [0]:pos[0], [1]:pos[1] 
            float frequency, qual, gqual;
            Array<GeneAnnotData> annotation;
            SDictionary attribute;

        public:
            Variant();
            Variant(const SVar &var);
            Variant(const SVar& var1, const SVar& var2, int rev = 0);
            Variant(const Variant &var);
            Variant(const GffData &gff, SeqList *ref);
            Variant(const SDictionary& obj);
            ~Variant();
            Variant& operator=(const SVar& var);
            Variant& operator=(const Variant& var);

            bool comparable(const Variant& var) const;
            bool lt(const Variant& var, VarParam* par) const;
            bool equal(const Variant& var, SeqList* ref, VarParam* par) const;

            void set(const SDictionary& obj);
            void clear();

            String toString(const char *form = nullptr) const;
            SObjPtr toObj() const;

            bool operator<(const Variant& var) const;
            bool operator==(const Variant& var) const;
        };
        enum class VL_FILE_TYPE {
            UNKNOWN = 0x00,
            TXT = 0x01,
            GTF = 0x02,
            VCF = 0x03,
            JSON = 0x04,
        };
        class SLIB_DLL VarList : public slib::Array<slib::SPointer<slib::sbio::Variant>> {
            friend Variant;
            friend VarFilter;
        private:
            SeqList* _ref;
            Variant _buffer;

        protected:
            SObjPtr _file;
            VL_FILE_TYPE _ftype;
            bool _load;
            Array<Pair<String, int>> _lg;

        public:
            SDictionary attribute;

        public:
            VarList();
            VarList(SeqList *ref);
            VarList(const char *s);
            VarList(const VarList&vl);
            ~VarList();

            void open(const char* path, SeqList* ref = nullptr);
            void load(const char *path, SeqList* ref = nullptr);
            void save(const char *path, const char *opts = nullptr);
            bool loaded() const;
            void setReference(SeqList* r);
            const SeqList& reference() const;
            const Array<Pair<String, int>>& linkageGroups() const;
            Variant* next();

            void tidyUp(size_t s = -1);
            
            
            void addFlag(sushort f);
            void removeFlag(sushort f);
            void clearAll();
            String path() const;
        };
    }
}
using sgenvar = slib::SPointer<slib::sbio::Variant>;
#endif
