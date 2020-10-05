#include "sbioinfo.h"
#include "Test.h"

using namespace slib;
using namespace slib::sbio;

void test::SBAnnotTest() {
	SBAnnotDB db;
	db.open("F:\\bioinfo\\test.db");
	SBAnnotDB::geneparray genes;
	db.geneInfo(genes, "cep-1");
	sforeach_(gene, genes) {
		std::cout << (*gene)->name << ":" << std::endl;
		sforeach_(trans, (*gene)->transcripts) {
			std::cout << TAB << (*trans)->name << ":" << std::endl;
			sforeach_(strct, (*trans)->structures) {
				std::cout << (*strct).begin << ".." << (*strct).end << "(" << (*strct).type << ")" << std::endl;
			}
		}
	}
}
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

