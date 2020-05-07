#include "sbioinfo/sbseqio.h"
#include "sbioinfo/sbioseq.h"

using namespace slib;
using namespace slib::sbio;

abidir::abidir() : number(0), element_type(0), element_size(0),
num_elements(0), data_size(0), data_offset(0), data_handle(0) {}
abidir::abidir(const abidir& dir) {
	number = dir.number;
	element_type = dir.element_type;
	element_size = dir.element_size;
	num_elements = dir.num_elements;
	data_size = dir.data_size;
	data_offset = dir.data_offset;
	data_handle = dir.data_handle;
}
abidir::~abidir() {}

abidir& abidir::operator=(const abidir& dir) {
	number = dir.number;
	element_type = dir.element_type;
	element_size = dir.element_size;
	num_elements = dir.num_elements;
	data_size = dir.data_size;
	data_offset = dir.data_offset;
	data_handle = dir.data_handle;
	return *this;
}
bool abidir::operator<(const abidir& dir) const { return number < dir.number; }
bool abidir::operator==(const abidir& dir) const { return number == dir.number; }


SBSeqIO::SBSeqIO() {}
SBSeqIO::~SBSeqIO() {}
inline void findMaskedRegion(SBioSeq *seq, String str) {
	srange mask;
	size_t pos = 0, len = str.length();
	auto p = &str[0];
	std::function<bool(char&)> isMaskChar;
	switch (seq->seqtype()) {
	case DNA_SEQ:
		isMaskChar = [](char& c) ->bool { return (c == 'N' || c == 'n'); };
		break;
	case RNA_SEQ:
		isMaskChar = [](char& c) ->bool { return (c == 'N' || c == 'n'); };
		break;
	case AA_SEQ:
		isMaskChar = [](char& c) ->bool { return (c == 'X' || c == 'x'); };
		break;
	default:
		isMaskChar = [](char& c) ->bool { return false; };
		break;
	}
	while (pos < len) {
		if (isMaskChar(*p)) {
			mask.begin = pos;
			while (pos < len && isMaskChar(*p)) {
				++pos; ++p;
			}
			mask.end = pos - 1;
			seq->addMask(mask);
		}
		else { ++pos; ++p; }
	}
}
sushort SBSeqIO::fileType(sio::SFile& file) {
	if (!file.isOpened()) file.open();
	char tmp[5];
	file.readBytes(tmp, file.size() < 5 ? file.size() : 5);
	if (tmp[0] == '>') return sbio::FASTA;
	else if (tmp[0] == 'A' && tmp[1] == 'B' && tmp[2] == 'I' && tmp[3] == 'F') return sbio::ABI;
	else if (tmp[0] == 'L' && tmp[1] == 'O' && tmp[2] == 'C' && tmp[3] == 'U' && tmp[4] == 'S') return sbio::GENBANK;
	else return sio::TEXT_FILE;
	file.setOffset(0);
}
void SBSeqIO::loadTXT(sio::SFile& file, SBioSeq* seq) {
	seq->clearAll();
	if (!file.isOpened()) file.open();
	String str;
	file >> str;
	if (!seq->_type) {
		seq->_type = COMPRESS1 | sseq::seqType(str);
		seq->_init();
	}
	sseq::seqForm(str);
	seq->setSeq(str);
}
inline void readABIDir(sio::SFile& file, Map<String, slib::Array<abidir>>& map) {
	char name[5]; memset(name, 0, 5);
	file.readBytes(name, 4);
	abidir dir;
	file.readInt(dir.number); invertEndian<4>(&dir.number);
	file.readShort(dir.element_type); invertEndian<2>(&dir.element_type);
	file.readShort(dir.element_size); invertEndian<2>(&dir.element_size);
	file.readInt(dir.num_elements); invertEndian<4>(&dir.num_elements);
	file.readInt(dir.data_size); invertEndian<4>(&dir.data_size);
	file.readInt(dir.data_offset); invertEndian<4>(&dir.data_offset);
	file.readInt(dir.data_handle); invertEndian<4>(&dir.data_handle);
	map[name].add(dir);
}
void SBSeqIO::loadABI(sio::SFile &file, SBioSeq *seq) {
	seq->clearAll();
	if (!file.isOpened()) file.open();
	const char* magic = "ABIF"; char tmp[4];
	Map<String, slib::Array<abidir>> map;
	if (!seq->type()) {
		seq->_type = ATTRIBUTED | SCORED | DNA_SEQ1;
		seq->_init();
	}
	file.readBytes(tmp, 4);
	if (memcmp(magic, tmp, 4)) throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, tmp, "ABIF");
	seq->attribute["format"] = "ABIF";
	sshort ver;
	file.readShort(ver);
	invertEndian<2>(&ver);
	seq->attribute["version"] = ver;
	readABIDir(file, map);
	auto& dir = map["tdir"][0];
	file.setOffset(dir.data_offset);
	sforin(i, 0, dir.num_elements) readABIDir(file, map);
	if (map.hasKey("PBAS")) {
		String str;
		file.setOffset(map["PBAS"][0].data_offset);
		str.resize(map["PBAS"][0].data_size);
		file.readBytes(&str[0], map["PBAS"][0].data_size);
		seq->setSeq(str);
	}
	seq->name = file.filename(false);
}
void SBSeqIO::loadGBK(sio::SFile& file, SBioSeq* seq) {
	seq->clearAll();
	seq->attribute["format"] = "GenBank";
	if (!file.isOpened()) file.open();
	String row;
	while (!file.eof()) {
		file.readLine(row);
		if (row.beginWith("LOCUS")) {

		}
		else if (row.beginWith("DEFINITION")) {

		}
		else if (row.beginWith("ACCESSION")) {

		}
		else if (row.beginWith("VERSION")) {

		}
		else if (row.beginWith("KEYWORDS")) {

		}
		else if (row.beginWith("SOURCE")) {

		}
		else if (row.beginWith("REFERENCE")) {

		}
		else if (row.beginWith("KEYWORDS")) {

		}
		else if (row.beginWith("FEATURES")) {

		}
		else if (row.beginWith("ORIGIN")) {

		}
		else if (row.beginWith("//")) break;
		else {

		}
	}
}
void SBSeqIO::loadFASTA(sushort type, sio::SFile& file, SBioSeq* seq) {
	seq->clearAll();
	if (!file.isOpened()) file.open();
	String row, str, name;
	file.readLine(name);
	if (name[0] != '>') throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, name, "FASTA");
	name.clip(1);
	while (!file.eof()) {
		file.readLine(row);
		str += row;
	}
	if (!seq->_type) {
		seq->_type = COMPRESS1 | type;
		seq->_init();
	}
	else if (type & MASKED) findMaskedRegion(seq, str);
	seq->setSeq(str);
}
void SBSeqIO::loadFASTA(sushort type, sio::SFile& file, SBSeqList* list) {
	list->clearAll();
	String row, seq, name;
	try {
		list->attribute["_file_type"] = "fasta";
		if (!file.isOpened()) file.open();
		file.readLine(row);
		if (!row.beginWith(">")) throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, row, "FASTA");
		name = row.substring(1);
		list->index[name] = list->size();
		if (!type) {
			file.readLine(seq);
			if (!type) type = sseq::seqType(seq);
		}
		list->add(SBioSeq(type, name));

		while (!file.eof()) {
			file.readLine(row);
			if (row.empty()) break;
			if (row[0] == '>') {
				if (type & MASKED) findMaskedRegion(list->last(), seq);
				list->_length.add(seq.length());
				list->last()->setSeq(seq); 
				seq.clear();
				name = row.substring(1);
				list->index[name] = list->size();
				list->add(SBioSeq(type, name));
			}
			else seq += row;
		}
		if (type & MASKED) findMaskedRegion(list->last(), seq);
		list->last()->setSeq(seq);
		list->_loaded = true;
	}
	catch (SIOException ie) {
		ie.print();
	}
}
void SBSeqIO::saveTXT(sio::SFile& file, SBioSeq* seq) {
	if (!file.isOpened()) file.open(nullptr, sio::WRITE);
	file << seq->raw(); file.close();
}
void SBSeqIO::saveGBK(sio::SFile& file, SBioSeq* seq) {
	/*
	 */
}
void SBSeqIO::saveFASTA(sio::SFile& file, SBioSeq* seq) {
	if (!file.isOpened()) file.open(nullptr, sio::WRITE);
	file << ">" << seq->name << String::LF; file.flush();
	size_t current = 0;
	while (current + FASTA_ROW_CHAR < seq->_length) {
		file << seq->raw(current, FASTA_ROW_CHAR) << String::LF;
		file.flush();
		current += FASTA_ROW_CHAR;
	}
	if (current < seq->_length)
		file << seq->raw(current, seq->_length - current) << String::LF;
	file.close();
}
void SBSeqIO::saveFASTA(sio::SFile& file, SBSeqList* list) {
	if (!file.isOpened()) file.open(nullptr, sio::WRITE);
	sforeach(*list) {
		file << ">" << E_->name << String::LF; file.flush();
		size_t current = 0;
		while (current + FASTA_ROW_CHAR < E_->_length) {
			file << E_->raw(current, FASTA_ROW_CHAR) << String::LF;
			file.flush();
			current += FASTA_ROW_CHAR;
		}
		if (current < E_->_length)
			file << E_->raw(current, E_->_length - current) << String::LF;
	}
	file.close();
}
void SBSeqIO::makeIndex(sushort type, sio::SFile& file, SBSeqList* list) {
	list->clearAll();
	String row, seq, name;
	try {
		list->attribute["_file_type"] = "fasta";
		list->attribute["_file"] = sio::SFile(file.path(), sio::READ);
		if (!file.isOpened()) file.open();
		file.readLine(row);
		if (!row.beginWith(">")) throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, row, "FASTA");
		list->attribute["_offset"].add(file.offset());
		name = row.substring(1);
		list->index[name] = list->size();
		if (!type) {
			file.readLine(seq);
			if (!type) type = sseq::seqType(seq);
		}
		list->add(SBioSeq(type, name));

		while (!file.eof()) {
			file.readLine(row);
			if (row.empty()) break;
			if (row[0] == '>') {
				if (type & MASKED) findMaskedRegion(list->last(), seq);
				list->last()->setLength(seq.length(), true); 
				seq.clear();
				list->_length.add(seq.length());
				name = row.substring(1);
				list->index[name] = list->size();
				list->add(SBioSeq(type, name));
				list->attribute["_offset"].add(file.offset());
			}
			else seq += row;
		}
		if (type & MASKED) findMaskedRegion(list->last(), seq);
		list->last()->setLength(seq.length(), true);
		list->_length.add(seq.length());
		list->_loaded = false;
	}
	catch (SIOException ie) {
		ie.print();
	}
}