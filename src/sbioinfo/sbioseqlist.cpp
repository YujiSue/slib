#include "sbioinfo/sbioseq.h"

using namespace slib;
using namespace slib::sbio;

constexpr const char *SLIB_SBIOSEQ_MAGIC = "SLIB";

SBSeqList::SBSeqList() : _loaded(false), Array<sbseq>() {}
SBSeqList::SBSeqList(const char *s, bool l) : SBSeqList() { if (l) load(s); else makeIndex(s); }
SBSeqList::~SBSeqList() {}
bool SBSeqList::isLoaded() const { return _loaded; }
intarray& SBSeqList::lengthList() { return _length; }
const intarray& SBSeqList::lengthList() const { return _length; }
size_t SBSeqList::seqIdx(const char *name) {
    if (index.hasKey(name)) return index[name];
    return NOT_FOUND;
}
suinteger SBSeqList::total() const {
    suinteger total = 0;
    sforeach(*this) total += E_->length();
    return total;
}
void SBSeqList::load(const char *path) {
    char magic[4];
    sint tmp;
    sushort type;
    bool scored = false;
    try {
		sio::SFile file(path, sio::READ);
		//Read magic
		file.readBytes(magic, 4);
		if (memcmp(magic, SLIB_SBIOSEQ_MAGIC, 4)) throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, magic, "SBIO_SEQ");
		//Read header
		file.readSObject(attribute);
		//Read sequence count
		file.readInt(tmp);
        if (tmp < 1) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, std::to_string(tmp).c_str(), ">0");
        resize(tmp);
        sforin(i, 0, size()) {
            //Read sequence type
			file.readUShort(type);
            if (!scored && type&SCORED) scored = true;
            //Sequence init
            at(i) = sbseq(type);
			auto& seq = at(i);
            //Read sequence length
			file.readInt(tmp);
			seq->setLength(tmp);
			_length.add(tmp);
            //Read sequence name
			file.readInt(tmp);
			seq->name.resize(tmp);
			file.readChars(&seq->name[0], tmp);
            index.set(seq->name, i);
            if (type&MASKED) {
                //Read sequence mask
				file.readInt(tmp);
				seq->_mask.resize(tmp);
                sforeachi_(j, seq->mask()) {
					file.readInt(tmp);
					seq->_mask[j].begin = tmp;
					file.readInt(tmp);
					seq->_mask[j].end = tmp;
                }
            }
			//Read sequence attribute
			if (type & ATTRIBUTED) 
				file.readSObject(seq->attribute);
            if (type&ANNOTATED) {
                //Read sequence annotation
				file.readInt(tmp);
				seq->annotation.reserve(tmp);
				sforeach(seq->annotation) {
					sbseq_annotation annot;
					file.readUInt(annot.type);
					suint ui;
					file.readUInt(ui);
					if (ui & 0x80000000) {
						annot.dir = true; ui -= 0x80000000;
					}
					annot.pos.resize(ui);
					sforeach_(pit, annot.pos) {
						file.readInt(pit->begin);
						file.readInt(pit->end);
					}
					file.readSObject(annot.attribute);
					seq->annotation.add(annot);
				}
            }
        }
        //Read sequence bytes
        sforin(i, 0, Array<sbseq>::size()) {
			file.readBytes(Array<sbseq>::at(i)->ptr(), Array<sbseq>::at(i)->size());
        }
		file.close();
		_loaded = true;
    }
    catch (sio::SIOException ie) {
        ie.print();
		clear();
    }
	catch (SBioInfoException be) {
		be.print();
		clear();
	}
}
void SBSeqList::save(const char *path) {
	ubytearray tmp;
	String srctype;
	sfile src;
	sarray off;
	if (!isLoaded()) {
		src = attribute["_file"];
		srctype = attribute["_file_type"];
		off = attribute["_offset"];
	}
	attribute.remove("_offset");
	attribute.remove("_file");
	attribute.remove("_file_type");
    try {
		sio::SFile file(path, sio::CREATE);
		 //Write magic
		file.writeBytes(SLIB_SBIOSEQ_MAGIC, 4);
		//Write header
		file.writeSObject(attribute);
        //Write sequence count
		file.writeInt((sint)size());
		file.flush();
		sforin(i, 0, size()) {
            auto &seq = at(i);
            //Write sequence type
			file.writeUShort(seq->type());
			//Write sequence length
			file.writeInt(seq->length());
            //Write sequence name
			file.writeInt((sint)seq->name.length());
			file.writeBytes(seq->name.cstr(), seq->name.length());
			file.flush();
			if (seq->isMasked()) {
                //Write sequence mask
				file.writeInt(seq->mask().size());
                sforeachi_(j, seq->mask()) {
					file.writeInt(seq->mask()[j].begin);
					file.writeInt(seq->mask()[j].end);
                }
				file.flush();
            }
			if (seq->isAttributed())
				file.writeSObject(at(i)->attribute);
			file.flush();
            if (seq->isAnnotated()) {
                //Write sequence annotation
				file.writeInt(seq->annotation.size());
				sforeach(seq->annotation) {
					file.writeUInt(E_->type);
					suint ui = E_->pos.size();
					if (E_->dir) ui |= 0x80000000;
					file.writeUInt(ui);
					sforeach_(pit, E_->pos) {
						file.writeInt(pit->begin);
						file.writeInt(pit->end);
					}
					file.writeSObject(E_->attribute);
				}
				file.flush();
            }
        }
        //Write sequence bytes
        sforin(i, 0, size()) {
			if (isLoaded()) file.writeBytes((void*)(at(i)->ptr()), at(i)->size());
			else {
				tmp.clear();
				src->setOffset(off[i].uinteger());
				if (srctype == "fasta") {
					tmp.resize(at(i)->length());
					auto offptr = tmp.ptr();
					String row;
					while (!src->eof()) {
						src->readLine(row);
						if (row.empty() || row[0] == '>') break;
						memcpy(offptr, row.cstr(), row.size());
						offptr += row.size();
					}
					switch (at(i)->seqtype()) {
					case DNA_SEQ:
					{
						DNA_CONVERTER[0][at(i)->compress()](tmp.ptr(), 0, tmp.size(), tmp.ptr());
						if (1 < at(i)->compress()) 
							tmp.resize((at(i)->length() - 1) / at(i)->compress() + 1);
						break;
					}
					case RNA_SEQ:
					{
						RNA_CONVERTER[0][at(i)->compress()](tmp.ptr(), 0, tmp.size(), tmp.ptr());
						break;
					}
					case AA_SEQ:
					{
						AA_CONVERTER[0][at(i)->compress()](tmp.ptr(), 0, tmp.size(), tmp.ptr());
						break;
					}
					default:
						break;
					}
				}
				else {
					tmp.resize((at(i)->length() - 1) / (0 < at(i)->compress() ? at(i)->compress() : 1) + 1);
					src->readBytes(tmp.ptr(), tmp.size());
				}
				file.writeBytes((void*)(tmp.ptr()), tmp.size());
            }
			file.flush();
        }
		file.close();
	}
	catch (SException ex) {
		ex.print();
	}
	catch (sio::SIOException ie) {
		ie.print();
	}
}
void SBSeqList::makeIndex(const char *path) {
    char magic[4];
    sint tmp;
    sushort type;
    try {
		auto file = sio::SFile(path, sio::READ);
        //Read magic
		file.readBytes(magic, 4);
        if (memcmp(magic, SLIB_SBIOSEQ_MAGIC, 4)) {
            std::cout<<magic<<std::endl;
        }
		//Read header
		file.readSObject(attribute);
        //Read sequence count
		file.readInt(tmp);
        if (tmp < 1) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, std::to_string(tmp).c_str(), ">0");
        resize(tmp);
		attribute["_offset"] = SArray(tmp);
        sforin(i, 0, size()) {
            auto &seq = Array<sbseq>::at(i);
            //Read sequence type
			file.readUShort(type);
            //Sequence init
            seq = SBioSeq(type);
            //Read sequence length
			file.readInt(tmp);
			seq->setLength(tmp, true);
			_length.add(tmp);
            //Read sequence name
			file.readInt(tmp);
            seq->name.resize(tmp);
			file.readBytes(&seq->name[0], tmp);
            index.set(seq->name, i);
            if (type&MASKED) {
                //Read sequence mask
				file.readInt(tmp);
                seq->_mask.resize(tmp);
                sforeachi_(j, seq->mask()) {
					file.readInt(tmp);
                    seq->_mask[j].begin = tmp;
					file.readInt(tmp);
                    seq->_mask[j].end = tmp;
                }
            }
			//Read sequence attribute
			if (type & ATTRIBUTED) 
				file.readSObject(at(i)->attribute);
            if (type&ANNOTATED) {
                //Read sequence annotation
				/*
				 */
            }
        }
        suinteger off = file.offset();
        sforin(i, 0, size()) {
			attribute["_offset"][i] = off;
            auto &seq = Array<sbseq>::at(i);
            off += (seq->length()-1)/(0<seq->compress()?seq->compress():1)+1;
        }
		file.close();
		attribute["_file"] = sio::SFile(path, sio::READ);
		attribute["_file_type"] = "slib";
		_loaded = false;
    } catch (sio::SIOException ie) {
		ie.print();
    }
}
SBioSeq SBSeqList::getSeq(sushort type, const sbpos &pos) {
    return SBioSeq(type, at(pos.idx)->name+"_sub",
                   at(pos.idx)->raw(pos.begin, pos.length()+1, pos.dir));
}
SBioSeq SBSeqList::getSeq(sushort type, int idx, size_t pos, size_t len, bool dir) {
    return SBioSeq(type, at(idx)->name+"_sub", at(idx)->raw(pos, len, dir));
}
void SBSeqList::clearAll() {
	_length.clear();
	index.clear();
	attribute.clear();
	Array<sbseq>::clear();
}