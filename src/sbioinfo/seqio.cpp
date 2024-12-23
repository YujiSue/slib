#include "sbioinfo/seqio.h"
#include "sbioinfo/seq.h"

void slib::sbio::sio::writeSeq(IOStream& stream, const Sequence& seq, const char* format) {
	if (!format)  writeTxt(stream, seq);
	String f = sstr::toLower(format);
	if (f.beginWith("fa")) writeFa(stream, seq);
	else if (f.beginWith("gb")) writeGbk(stream, seq);
	else writeTxt(stream, seq);
}
/*
void slib::sbio::sio::writeSeqs(IOStream& stream, const SeqList& seqs, const char* format) {
	String f = format ? sstr::toLower(format) : "txt";
	if (f.beginWith("fa")) {
		sfor(seqs) writeFa(stream, $_);
	}
	else if (f.beginWith("gb")) {
		sfor(seqs) writeGbk(stream, $_);
	}
	else {
		sfor(seqs) {
			writeTxt(stream, $_);
			if ($ < seqs.end() - 1) stream << NL << "---" << NL * 2;
		}
	}
}
*/
void slib::sbio::sio::writeTxt(IOStream& stream, const slib::sbio::Sequence& seq) {
	stream.print(seq.raw());
}
void slib::sbio::sio::writeFa(slib::IOStream& stream, const slib::sbio::Sequence& seq) {
	stream.print(">", seq.name);
	auto sz = seq.length();
	auto off = 0;
	while (off < sz) {
		stream.print(seq.raw(off, FASTA_ROW_COUNT));
		off += FASTA_ROW_COUNT;
	}
}
void slib::sbio::sio::writeFa(IOStream& stream, const  slib::sbio::SeqList& seqs) {
	sfor(seqs) {
		stream.print(">", $_.name);
		auto sz = $_.length();
		auto off = 0;
		while (off < sz) {
			stream.print($_.raw(off, FASTA_ROW_COUNT));
			off += FASTA_ROW_COUNT;
		}
	}
}
void slib::sbio::sio::writeGbk(IOStream& stream, const Sequence& seq) {
	String tmp;
	stream << sstr::rfill("LOCUS", ' ', 12) << seq.name << SP * 6 <<
		sstr::rfill(S(seq.length()) + SP + sbio::sutil::seqTypeUnit(seq.type()), ' ', 12) <<
		sstr::rfill(sbio::sutil::seqTypeName(seq.type()), ' ', 8) << 
		(seq.isCircular() ? "circular" : "linear") << SP <<
		(seq.attribute.hasKey("division") ? seq.attribute["division"] : "UNA") << SP <<
		(seq.attribute.hasKey("date") ? seq.attribute["date"].date() : SDate()).toString("DD-MM-YYYY") << LF;
	//
	if (seq.annotation.count()) {
		stream << sstr::rfill("FEATURES", ' ', 21) << "Location/Qualifiers" << LF;
		sforeach(annot, seq.annotation) {
			stream << sbio::sutil::toGBK(annot, seq.length()) << LF;
		}
	}
	//
	stream.print("ORIGIN");
	auto count = 1;
	while (count < seq.length()) {
		stream << sstr::lfill(String(count), ' ', 9);
		tmp = seq.raw(count - 1, 60);
		sforin(i, 0, 6) { 
			if (tmp.size() < 10 * i) break;
			stream << SP << tmp.substring((10 * i), 10); 
		}
		stream << NL;
		count += 60;
	}
	stream.print("//");
}
void slib::sbio::sio::readTxt(SFile& file, Sequence& seq) {
	String buf;
	file >> buf;
	seq.setSeqAs(buf, seq.type());
}
void slib::sbio::sio::readFa(SFile& file, Sequence& seq) {
	String buf;
	file.readLine(buf);
	if (buf[0] != '>') throw FormatException(formatErrorText("Fasta title row", buf, ">NAME"));
	seq.name = buf.split(SP)[0].substring(1);
	file >> buf;
	seq.setSeqAs(buf, seq.type());
}
void slib::sbio::sio::readGbk(SFile& file, Sequence& seq) {
	/*
	String indent = SP * 12, findent = SP * 21;
	seq.attribute["format"] = "genbank";
	size_t len = -1;
	String ln, data;
	sushort type = 0;
	while (!file.eof()) {
		file.readLine(ln);
		if (ln.beginWith("LOCUS")) {
			auto vals = ln.substring(12).split(" ");
			seq.name = vals[0];
			seq.setLength(vals[1].intValue());
			if (vals[3].match("DNA")) type = sbio::DNA_SEQ;
			else if (vals[3].match("RNA")) type = sbio::RNA_SEQ;
			if (vals[4] == "PRI") seq->attribute["class"] = "primate";
			else if (list[4] == "ROD") seq->attribute["class"] = "rodent";
			else if (list[4] == "MAM") seq->attribute["class"] = "mammal";
			else if (list[4] == "VRT") seq->attribute["class"] = "vertebrate";
			else if (list[4] == "INV") seq->attribute["class"] = "invertebrate";
			else if (list[4] == "PLN") seq->attribute["class"] = "plant";
			else if (list[4] == "BCT") seq->attribute["class"] = "bacteria";
			else if (list[4] == "VRL") seq->attribute["class"] = "viral";
			else if (list[4] == "PHG") seq->attribute["class"] = "phage";
			else if (list[4] == "SYN") seq->attribute["class"] = "synthetic";
			else if (list[4] == "UNA") seq->attribute["class"] = snull;
			else if (list[4] == "EST") seq->attribute["class"] = "EST";
			else if (list[4] == "PAT") seq->attribute["class"] = "patent";
			else if (list[4] == "STS") seq->attribute["class"] = "tag";
			else if (list[4] == "GSS") seq->attribute["class"] = "genome";
			else if (list[4] == "HTG") seq->attribute["class"] = "htgenome";
			else if (list[4] == "HTC") seq->attribute["class"] = "htcdna";
			else if (list[4] == "ENV") seq->attribute["class"] = "environment";
			seq.attribute["date"] = SDate(vals[5], "DD-MMM-YYYY");
			file.readLine(ln);
		}
		if (ln.beginWith("DEFINITION")) {
			seq.attribute["definition"] = ln.substring(12);
			file.readLine(ln);
			while (!file.eof() && ln.beginWith(indent)) {
				seq.attribute["definition"].string() += " " + ln.substring(12);
				file.readLine(ln);
			}
		}
		if (row.beginWith("ACCESSION")) {
			seq->attribute["accession"] = row.substring(12);
		}
		if (row.beginWith("VERSION")) {
			data = row.substring(12);
			auto list = data.split(" ");
			seq->attribute["version"] = list[0];
			if (1 < list.size()) {
				sforin(i, 1, list.size()) {
					if (list[i].beginWith("GI:")) seq->attribute["gi"] = list[i].substring(3);
				}
			}
		}
		if (row.beginWith("KEYWORDS")) {
			data = row.substring(12);
			if (data != ".") seq->attribute["keywords"] = data;
		}
		if (row.beginWith("SOURCE")) {
			data = row.substring(12);
			seq->attribute["source"] = data;
			file.readLine(row);
			if (row.beginWith("  ORGANISM")) {
				seq->attribute["organism"] = row.substring(12);
				seq->attribute["taxonomy"] = sarray();
				file.readLine(row);
				while (!file.eof() && row.beginWith(indent)) {
					auto list = data.split(";");
					sforeach(list) {
						if (E_.last() == '.') seq->attribute["taxonomy"].add(E_.substring(0, E_.length() - 1));
						else seq->attribute["taxonomy"].add(E_);
					}
					file.readLine(row);
				}
			}
		}
		if (row.beginWith("REFERENCE")) {
			if (!seq->attribute.hasKey("reference")) seq->attribute["reference"] = SArray();
			SDictionary dict;
			file.readLine(row);
			while (!file.eof() && row.beginWith("  ")) {
				if (row.beginWith("  AUTHORS")) {
					data = row.substring(12);
					file.readLine(row);
					while (!file.eof() && row.beginWith(indent)) {
						data += row.substring(12);
						file.readLine(row);
					}
					dict["authors"] = sarray(data.split(", "));
				}
				if (row.beginWith("  TITLE")) {
					data = row.substring(12);
					file.readLine(row);
					while (!file.eof() && row.beginWith(indent)) {
						data += SPACE + row.substring(12);
						file.readLine(row);
					}
					dict["title"] = data;
				}
				if (row.beginWith("  JOURNAL")) {
					data = row.substring(12);
					file.readLine(row);
					while (!file.eof() && row.beginWith(indent)) {
						data += SPACE + row.substring(12);
						file.readLine(row);
					}
					dict["journal"] = data;
				}
				if (row.beginWith("  PUBMED")) {
					dict["pubmed"] = row.substring(12).integer();
					file.readLine(row);
				}
			}
			seq->attribute["reference"].add(dict);
		}
		if (row.beginWith("FEATURES")) {
			file.readLine(row);

			while (!file.eof() && row.beginWith(findent)) {




				file.readLine(row);
			}
		}
		if (row.beginWith("ORIGIN")) {
			data.clear();
			file.readLine(row);
			while (!file.eof() && row[0] != ' ') {
				data += row.substring(10).replaced(" ", "");
				file.readLine(row);
			}
			if (len != -1 && len != data.length()) throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR);
			seq->setSeq(data);
		}
		if (row.beginWith("//")) break;
	}
	*/
}
slib::sbio::AbiDir::AbiDir() : number(0), element_type(0), element_size(0),
num_elements(0), data_size(0), data_offset(0), data_handle(0) {}
slib::sbio::AbiDir::AbiDir(const AbiDir& dir) {
	number = dir.number;
	element_type = dir.element_type;
	element_size = dir.element_size;
	num_elements = dir.num_elements;
	data_size = dir.data_size;
	data_offset = dir.data_offset;
	data_handle = dir.data_handle;
}
slib::sbio::AbiDir::~AbiDir() {}
slib::sbio::AbiDir& slib::sbio::AbiDir::operator=(const slib::sbio::AbiDir& dir) {
	number = dir.number;
	element_type = dir.element_type;
	element_size = dir.element_size;
	num_elements = dir.num_elements;
	data_size = dir.data_size;
	data_offset = dir.data_offset;
	data_handle = dir.data_handle;
	return *this;
}
bool slib::sbio::AbiDir::operator<(const slib::sbio::AbiDir& dir) const { return number < dir.number; }
bool slib::sbio::AbiDir::operator==(const slib::sbio::AbiDir& dir) const { return number == dir.number; }
inline void readABIDir(slib::SFile& file, slib::Map<slib::String, slib::Array<slib::sbio::AbiDir>>& map) {
	char name[5]; memset(name, 0, 5);
	file.readBytes(name, 4);
	slib::sbio::AbiDir dir;
	file.readInt(dir.number); slib::invertEndian<4>(&dir.number);
	file.readShort(dir.element_type); slib::invertEndian<2>(&dir.element_type);
	file.readShort(dir.element_size); slib::invertEndian<2>(&dir.element_size);
	file.readInt(dir.num_elements); slib::invertEndian<4>(&dir.num_elements);
	file.readInt(dir.data_size); slib::invertEndian<4>(&dir.data_size);
	file.readInt(dir.data_offset); slib::invertEndian<4>(&dir.data_offset);
	file.readInt(dir.data_handle); slib::invertEndian<4>(&dir.data_handle);
	map[name].add(dir);
}
inline slib::SObjPtr getAbiData(slib::SFile& file, slib::sbio::AbiDir& dir) {
	slib::SObjPtr content = slib::SDictionary({
		D_("type", dir.element_type),
		D_("num", dir.num_elements),
		D_("size", dir.element_size),
		D_("data", snull)
		});
	slib::SData data;
	file.setOffset(dir.data_offset);
	data.resize(dir.data_size);
	file.readBytes(&data[0], dir.data_size);
	switch (dir.element_type) {
	case 1:
	{
		if (dir.num_elements == 1) content["data"] = data[0];
		else content["data"] = data;
		break;
	}
	case 2:
	{
		if (dir.num_elements == 1) content["data"] = data[0];
		else content["data"] = data;
		break;
	}
	case 3:
	{
		auto sz = data.size() / 2;
		auto p = data.as<slib::sushort>();
		sforin(i, 0, sz) { slib::invertEndian<2>(p); ++p; }
		if (dir.num_elements == 1) content["data"] = data.as<slib::sushort>()[0];
		else content["data"] = data;
		break;
	}
	case 4:
	{
		auto sz = data.size() / 2;
		auto p = data.as<slib::sshort>();
		sforin(i, 0, sz) { slib::invertEndian<2>(p); ++p; }
		if (dir.num_elements == 1) content["data"] = data.as<slib::sshort>()[0];
		else content["data"] = data;
		break;
	}
	case 5:
	{
		auto sz = data.size() / 4;
		auto p = data.as<slib::sint>();
		sforin(i, 0, sz) { slib::invertEndian<4>(p); ++p; }
		if (dir.num_elements == 1) content["data"] = data.as<slib::sint>()[0];
		else content["data"] = data;
		break;
	}
	case 7:
	{
		auto sz = data.size() / 4;
		auto p = data.as<float>();
		sforin(i, 0, sz) { slib::invertEndian<4>(p); ++p; }
		if (dir.num_elements == 1) content["data"] = data.as<float>()[0];
		else content["data"] = data;
		break;
	}
	case 8:
	{
		auto sz = data.size() / 8;
		auto p = data.as<double>();
		sforin(i, 0, sz) { slib::invertEndian<8>(p); ++p; }
		if (dir.num_elements == 1) content["data"] = data.as<double>()[0];
		else content["data"] = data;
		break;
	}
	case 10:
	{
		auto y = reinterpret_cast<slib::sshort*>(&data[0]);
		content["data"] =
			S((int)(*y)) + "/" + 
			slib::sstr::lfill(S((int)data[2]), '0', 2) + "/" +
			slib::sstr::lfill(S((int)data[3]), '0', 2);
		break;
	}
	case 11:
	{
		content["data"] =
			slib::sstr::lfill(S((int)data[0]), '0', 2) + ":" +
			slib::sstr::lfill(S((int)data[1]), '0', 2) + ":" +
			slib::sstr::lfill(S((int)data[2]), '0', 2) + "." +
			slib::sstr::lfill(S((int)data[3]), '0', 2);
		break;
	}
	case 18:
	{
		slib::String s(dir.num_elements - 1, '\0');
		memcpy(&s[0], &data[1], dir.num_elements - 1);
		content["data"] = s;
		break;
	}
	case 19:
	{
		slib::String s(dir.num_elements - 1, '\0');
		memcpy(&s[0], &data[0], dir.num_elements - 1);
		content["data"] = s;
		break;
	}
	default:
		content["data"] = data;
		break;
	}
	return content;
}
inline void toAbiObj(slib::SFile& file, slib::Map<slib::String, slib::Array<slib::sbio::AbiDir>>& map, slib::SDictionary &attr) {
	sfor(map) {
		auto& dirs = $_.value();
		if (dirs.empty()) continue;
		if ($_.key() == "tdir") continue;
		if (dirs.size() == 1) attr[$_.key()] = getAbiData(file, dirs[0]);
		else {
			slib::SArray array(dirs.size());
			sfor2(dirs, array) $_2 = getAbiData(file, $_1);
			attr[$_.key()] = array;
		}
	}
}
void slib::sbio::sio::readAbi(SFile& file, Sequence& seq) {
	char tmp[5]; tmp[4] = '\0';
	Map<String, slib::Array<AbiDir>> map;
	seq.attribute["_origin_"] = file.path();
	file.readBytes(tmp, 4);
	if (memcmp(ABI_MAGIC, tmp, 4)) throw FormatException(formatErrorText("ABIF magic", tmp, ABI_MAGIC));
	seq.attribute["format"] = "abif";
	sshort ver;
	file.readShort(ver);
	invertEndian<2>(&ver);
	seq.attribute["version"] = ver;
	readABIDir(file, map);
	auto& dir = map["tdir"][0];
	auto count = dir.num_elements;
	file.setOffset(dir.data_offset);
	sforin(i, 0, count) readABIDir(file, map);
	toAbiObj(file, map, seq.attribute);
	if (map.hasKey("PBAS")) {
		String str;
		file.setOffset(map["PBAS"][0].data_offset);
		str.resize(map["PBAS"][0].data_size);
		file.readBytes(&str[0], map["PBAS"][0].data_size);
		seq.setSeqAs(str, DNA_SEQ);
	}
	seq.name = file.fileName(false);
}

/*
void SBSeqIO::loadGBK(sio::SFile& file, SBioSeq* seq) {
	String indent = SPACE * 12, findent = SPACE * 21;
	seq->clearAll();
	seq->attribute["format"] = "GenBank";
	if (!file.isOpened()) file.open();
	size_t len = -1;
	String row, data;
	while (!file.eof()) {
		file.readLine(row);
		if (row.beginWith("LOCUS")) {
			auto list = row.substring(12).split(" ");
			seq->name = list[0];
			seq->setLength(list[1]);
			if (list[3].contain("DNA")) seq->convert(sbio::DNA_SEQ);
			else if (list[3].contain("RNA")) seq->convert(sbio::RNA_SEQ);

			if (list[4] == "PRI") seq->attribute["class"] = "primate";
			else if (list[4] == "ROD") seq->attribute["class"] = "rodent";
			else if (list[4] == "MAM") seq->attribute["class"] = "mammal";
			else if (list[4] == "VRT") seq->attribute["class"] = "vertebrate";
			else if (list[4] == "INV") seq->attribute["class"] = "invertebrate";
			else if (list[4] == "PLN") seq->attribute["class"] = "plant";
			else if (list[4] == "BCT") seq->attribute["class"] = "bacteria";
			else if (list[4] == "VRL") seq->attribute["class"] = "viral";
			else if (list[4] == "PHG") seq->attribute["class"] = "phage";
			else if (list[4] == "SYN") seq->attribute["class"] = "synthetic";
			else if (list[4] == "UNA") seq->attribute["class"] = snull;
			else if (list[4] == "EST") seq->attribute["class"] = "EST";
			else if (list[4] == "PAT") seq->attribute["class"] = "patent";
			else if (list[4] == "STS") seq->attribute["class"] = "tag";
			else if (list[4] == "GSS") seq->attribute["class"] = "genome";
			else if (list[4] == "HTG") seq->attribute["class"] = "htgenome";
			else if (list[4] == "HTC") seq->attribute["class"] = "htcdna";
			else if (list[4] == "ENV") seq->attribute["class"] = "environment";

			seq->attribute["date"] = SDate(list[5], "DD-MMM-YYYY");
			file.readLine(row);
		}
		if (row.beginWith("DEFINITION")) {
			seq->attribute["definition"] = row.substring(12);
			file.readLine(row);
			while (!file.eof() && row.beginWith(indent)) {
				seq->attribute["definition"].string() += " " + row.substring(12);
				file.readLine(row);
			}
		}
		if (row.beginWith("ACCESSION")) {
			seq->attribute["accession"] = row.substring(12);
		}
		if (row.beginWith("VERSION")) {
			data = row.substring(12);
			auto list = data.split(" ");
			seq->attribute["version"] = list[0];
			if (1 < list.size()) {
				sforin(i, 1, list.size()) {
					if (list[i].beginWith("GI:")) seq->attribute["gi"] = list[i].substring(3);
				}
			}
		}
		if (row.beginWith("KEYWORDS")) {
			data = row.substring(12);
			if (data != ".") seq->attribute["keywords"] = data;
		}
		if (row.beginWith("SOURCE")) {
			data = row.substring(12);
			seq->attribute["source"] = data;
			file.readLine(row);
			if (row.beginWith("  ORGANISM")) {
				seq->attribute["organism"] = row.substring(12);
				seq->attribute["taxonomy"] = sarray();
				file.readLine(row);
				while (!file.eof() && row.beginWith(indent)) {
					auto list = data.split(";");
					sforeach(list) {
						if (E_.last() == '.') seq->attribute["taxonomy"].add(E_.substring(0, E_.length() - 1));
						else seq->attribute["taxonomy"].add(E_);
					}
					file.readLine(row);
				}
			}
		}
		if (row.beginWith("REFERENCE")) {
			if (!seq->attribute.hasKey("reference")) seq->attribute["reference"] = SArray();
			SDictionary dict;
			file.readLine(row);
			while (!file.eof() && row.beginWith("  ")) {
				if (row.beginWith("  AUTHORS")) {
					data = row.substring(12);
					file.readLine(row);
					while (!file.eof() && row.beginWith(indent)) {
						data += row.substring(12);
						file.readLine(row);
					}
					dict["authors"] = sarray(data.split(", "));
				}	
				if (row.beginWith("  TITLE")) {
					data = row.substring(12);
					file.readLine(row);
					while (!file.eof() && row.beginWith(indent)) {
						data += SPACE + row.substring(12);
						file.readLine(row);
					}
					dict["title"] = data;
				}
				if (row.beginWith("  JOURNAL")) {
					data = row.substring(12);
					file.readLine(row);
					while (!file.eof() && row.beginWith(indent)) {
						data += SPACE + row.substring(12);
						file.readLine(row);
					}
					dict["journal"] = data;
				}
				if (row.beginWith("  PUBMED")) {
					dict["pubmed"] =row.substring(12).integer();
					file.readLine(row);
				}
			}
			seq->attribute["reference"].add(dict);
		}
		if (row.beginWith("FEATURES")) {
			file.readLine(row);

			while (!file.eof() && row.beginWith(findent)) {




				file.readLine(row);
			}
		}
		if (row.beginWith("ORIGIN")) {
			data.clear();
			file.readLine(row);
			while (!file.eof() && row[0] != ' ') {
				data += row.substring(10).replaced(" ", "");
				file.readLine(row);
			}
			if (len != -1 && len != data.length()) throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR);
			seq->setSeq(data);
		}
		if (row.beginWith("//")) break;
	}
}

*/