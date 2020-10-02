#include "sbioinfo.h"
#include "Test.h"

using namespace slib;
using namespace slib::sbio;

void test::SBSeqIOTest() {
	SBioSeq seq1, seq2, seq3, seq4;
	//sio::SFile txtFile("");
	//SBSeqIO::loadTXT(txtFile, &seq1);
	//sio::SFile faFile("");
	//SBSeqIO::loadFASTA(CIRC_DNA_SEQ1, faFile, &seq2);
	sio::SFile abiFile("C:\\Users\\yujis\\Downloads\\test.ab1");
	SBSeqIO::loadABI(abiFile, &seq3);
	std::cout << seq3.raw() << std::endl;

}

void test::SBSeqTest() {

}