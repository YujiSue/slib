#include "sbioinfo/seq.h"
constexpr char SLIB_SBIOSEQ_MAGIC[5] = "SLIB";
slib::sbio::SeqList::SeqList() : Array<Sequence>() {}
slib::sbio::SeqList::SeqList(const char *s, bool l) : SeqList() { if (l) load(s); else open(s); }
slib::sbio::SeqList::~SeqList() {}
void slib::sbio::SeqList::open(const char* path) {
	if (path) _file.open(path);
	char magic[4];
	sint tmp;
	sushort type;
	int div;
	sizearray sizes;
	try {
		//Read magic
		_file.readBytes(magic, 4);
		if (memcmp(magic, SLIB_SBIOSEQ_MAGIC, 4)) throw SBioInfoException();// (ERR_INFO, SLIB_FORMAT_ERROR, magic, "SBIO_SEQ");
		//Read header
		_file.readSDict(attribute);
		//Read sequence count
		_file.readInt(tmp);
		if (tmp < 1) throw SBioInfoException();// (ERR_INFO, SLIB_RANGE_ERROR, std::to_string(tmp).c_str(), ">0");
		resize(tmp);
		sfor(*this) {
			//Read sequence type
			_file.readUShort(type);
			div = (type >> 8) & 0xFF;
			if (!div) div = 1;
			//Sequence init
			$_ = Sequence(type);
			//Read sequence length
			_file.readInt(tmp);
			$_.setLength(tmp, false);
			sizes.add((tmp - 1) / div + 1);
			//Read sequence name
			_file.readInt(tmp);
			$_.name.resize(tmp);
			_file.readChars(&$_.name[0], tmp);
			if (type & sseq::MASKED) {
				//Read sequence mask
				_file.readInt(tmp);
				$_.mask.resize(tmp);
				sforeach(range, $_.mask) {
					_file.readInt(range.begin);
					_file.readInt(range.end);
				}
			}
			//Read sequence attribute
			_file.readSDict($_.attribute);
			//Read sequence annotation
			_file.readInt(tmp); // 0
		}
		//Set offset
		_offsets.add(_file.offset());
		sfori(*this) {
			if (!i) continue;
			_offsets.add(_offsets[i - 1] + sizes[i - 1]);
		}
	}
	catch (Exception ex) {
		ex.print();
		clearAll();
	}
}
void slib::sbio::SeqList::load(const char* path) {
	if(path && empty()) open(path);
	if (!empty()) {
		sfor(*this) {
			$_.setLength($_.length());
			_file.readBytes($_.data(), $_.size());
		}
		_file.close();
	}
}
void slib::sbio::SeqList::save(const char* path) {
	SFile out(path, sio::MAKE), src;
	if (attribute.hasKey("_src_")) {
		src.open(attribute["_src_"]);
		attribute.remove("_src_");
	}
	Array<size_t> off;
	sfor(*this) {
		if ($_.attribute.hasKey("_offset_")) {
			off.add($_.attribute["_offset_"].sizeValue());
			$_.attribute.remove("_offset_");
		}
	}
	try {
		//Write magic
		out.writeBytes(SLIB_SBIOSEQ_MAGIC, 4);
		//Write header
		out.writeSDict(attribute);
		//Write sequence count
		out.writeInt((sint)size());
		out.flush();
		sfor(*this) {
			//Write sequence type
			out.writeUShort($_._type);
			//Write sequence length
			out.writeInt((sint)$_.length());
			//Write sequence name
			out.writeInt((sint)$_.name.length());
			out.writeBytes($_.name.cstr(), $_.name.length());
			out.flush();
			if ($_.isMasked()) {
				//Write sequence mask
				out.writeInt((sint)$_.mask.size());
				sforeach(range, $_.mask) {
					out.writeInt(range.begin);
					out.writeInt(range.end);
				}
				out.flush();
			}
			out.writeSDict($_.attribute);
			out.writeInt((sint)$_.annotation.count()); // 0
			/*
			sforeach(note, $_.annotation) {
				if (note.joint[0]) continue;
				out.writeUInt(note.category);
				out.writeUInt(note.type);
				out.writeInt(note.begin);
				out.writeInt(note.end);
				out.writeInt((int)note.dir);
				out.writeInt(note.name.size());
				out.writeChars(note.name);
				out.writeInt(note.note.size());
				out.writeChars(note.note);
				if (note.joint[1]) {
					out.writeInt(note.chain());
					auto nxt = note.joint[1];
					while (nxt) {
						out.writeInt(nxt->begin);
						out.writeInt(nxt->end);
						nxt = nxt->joint[1];
					}
				}
				else out.writeInt(0);
			}
			*/
			out.flush();
		}
		//Write sequence bytes
		if (loaded()) {
			sfor(*this) { out.writeBytes((void*)$_.data(), $_.size()); out.flush(); }
		}
		else {
			String ln, seq;
			ubytearray buffer;
			int div = 1;
			if (!src.isOpened() || off.empty()) throw Exception();
			sfori(*this) {
				div = at(i).compression();
				if (!div) div = 1;
				seq.reserve((size_t)(at(i).length() * 1.1));
				buffer.reserve(seq.capacity() / div + 1);
				src.setOffset(off[i]);
				while (src) {
					src.readLine(ln);
					if (ln.empty()) continue;
					else if (ln[0] == '>') break;
					seq << ln;
				}
				sseq::format(seq);
				buffer.resize((seq.size() - 1) / div + 1);
				at(i).encoder()((const subyte*)seq.cstr(), 0, seq.size(), buffer.data());
				out.writeBytes(buffer.data(), buffer.size());
				out.flush();
				seq.clear();
				buffer.clear();
			}
		}
		out.close();
	}
	catch (Exception ex) {
		ex.print();
	}
}
bool slib::sbio::SeqList::loaded() const { return size() && at(0).length() && at(0).size(); }
slib::String slib::sbio::SeqList::path() const { return _file.path(); }
size_t slib::sbio::SeqList::total() const {
	size_t total = 0;
	sfor(*this) total += $_.length();
	return total;
}
size_t slib::sbio::SeqList::index(const char* s) const { sfor(*this) { if ($_.name == s) return $INDEX(*this); } return NOT_FOUND; }
sindex slib::sbio::SeqList::nameIndex() const {
	sindex idx;
	sfor(*this) idx[$_.name] = (int)$INDEX(*this);
	return idx;
}
slib::sbio::Sequence slib::sbio::SeqList::subsequence(const RefPos& pos) const { return at(pos.idx).subsequence(pos); }
slib::String slib::sbio::SeqList::raw(const RefPos& pos, bool zero) const { 
	if (pos.idx < 0 || this->size() <= pos.idx) throw RangeException(outRangeErrorText("Sequence index", pos.idx, 0, this->size()));
	if (loaded()) return at(pos.idx).raw((zero ? (srange)pos : slib::shift(pos, -1)), pos.dir);
	else {
		slib::String str(pos.length(true), 'N');
		auto type = at(pos.idx)._type & 0x0F;
		auto enc = (at(pos.idx)._type >> 8) & 0x0F;
		auto off = (pos.begin - (zero ? 0 : 1)) % enc;
		auto epos = srange((pos.begin - (zero ? 0 : 1)) / enc, (pos.end - (zero ? 0 : 1)) / enc);
		ubytearray bytes(epos.length(true));
		//
		auto f = const_cast<SFile*>(&_file);
		f->seek(_offsets[pos.idx] + epos.begin, SFile::SEEK::BEGIN);
		f->readBytes(bytes.data(), bytes.size());
		switch (type) {
		case sseq::DNA:
		{
			switch(enc) {
			case 1:
				sdna::decode(bytes.data(), off, str.size(), (subyte*)&str[0]);
				break;
			case 2:
				sdna::decode2(bytes.data(), off, str.size(), (subyte*)&str[0]);
				break;
			case 4:
				sdna::decode4(bytes.data(), off, str.size(), (subyte*)&str[0]);
				break;
			default:
				break;
			}
			break;
		}
		case sseq::RNA:
			srna::decode(bytes.data(), off, str.size(), (subyte*)&str[0]); 
			break;
		case sseq::AA:
			saa::decode(bytes.data(), off, str.size(), (subyte*)&str[0]);
			break;
		default:
			break;
		}
		return str;
	}
}
void slib::sbio::SeqList::clearAll() {
	sfor(*this) $_.clearAll();
	clear();
}
