#include "sbioinfo/sfastq.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::sbio;

SFastq::SFastq() : SBSeqList() {}
SFastq::~SFastq() {}

void SFastq::importFq(const char *p) {
    try {
        SFile file(p, sio::READ);
        String row[4];
        while (!file.eof()) {
            sforin(i, 0, 4) file.readLine(row[i]);
            if(row[0][0] == '@' && row[2][0] == '+') {
                SBioSeq seq(sbio::DNA_SEQ);
                seq.setName(row[0]+row[2]);
                seq.setSeq(row[1]);
                seq.resize(2*seq.length());
                memcpy(seq.ptr(seq.length()), row[3].cstr(), seq.length());
            }
            else throw SBioException(ERR_INFO, SLIB_FORMAT_ERROR, row[0]+".."+row[2]+"..", "FASTQ");
        }
    } catch (SIOException ie) {
		ie.print();
    }
}
void SFastq::exportFq(const char *p) {
    try {
        String qual;
        stringarray names;
        SFile file(p, sio::CREATE);
        sforeach(*this) {
            names = E_->name().split("+");
            qual.resize(E_->length(), '\0');
            file<<names[0]<<NEW_LINE;
            file<<E_->raw()<<NEW_LINE;
            file<<names[1]<<NEW_LINE;
            memcpy(qual.ptr(), E_->ptr(E_->length()), E_->length());
            file<<qual<<NEW_LINE;
        }
    } catch (SIOException ie) {
		ie.print();
    }
}