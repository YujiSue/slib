#ifndef SLIB_SFILE_H
#define SLIB_SFILE_H
#include "sio/sio.h"
namespace slib {
    /**
    * @class SFile
    * \~english @brief File class
    * \~japanese @brief ファイルクラス
    */
    class SLIB_DLL SFile : public SObject {
    public:
        enum SEEK {
            BEGIN = std::ios_base::beg,
            CURRENT = std::ios_base::cur,
            END = std::ios_base::end,
        };
        std::fstream _stream;
        String _path;
        suinteger _size;
        subyte _mode;

    public:
        static SFile root();
        static SFile home();
        static SFile current();
    private:
        inline void _fopen();
    public:
        SFile();
        SFile(const char* path, int m = sio::READ);
        SFile(const SFile &file);
        ~SFile();
        SFile& operator=(const SFile& file);

        void open(const char* path = nullptr, const subyte m = sio::READ);
        void close();

        bool isOpened() const;
        String path() const;
        SFile parent() const;
        SFile child(const char* n) const;
        String dirName() const;
        String fileName(bool ext = true) const;
        String extension(bool lower = true) const;
        stringarray dirList(bool recur = false, bool hidden = false);
        stringarray fileList(const stringarray& ext = {}, bool recur = false, bool hidden = false);
        //stringarray find(const char* que, bool recur = false);
        void setPath(const char* s);
        bool exist() const;
        size_t size() const;
        subyte mode() const;
        bool isDir() const;
        std::streampos offset();
        void setOffset(std::streampos off, bool rev = false);
        void seek(std::streampos off, SEEK ori);
        bool empty() const;
        bool eof() const;
        void flush();
        void clear();
        
        void readLine(String& str, const bool ignore_quot = false);
        void readTo(const char* que);
        void readChars(char* str, size_t size);
        void readString(String& str, size_t size);
        void readBytes(void* dat, size_t size);
        void readChar(char& c);
        void readUChar(unsigned char& c);
        void readByte(sbyte& val);
        void readUByte(subyte& val);
        void readShort(sshort& val);
        void readUShort(sushort& val);
        void readInt(sint& val);
        void readUInt(suint& val);
        void readInteger(sinteger& val);
        void readUInteger(suinteger& val);
        void readFloat(float& val);
        void readDouble(double& val);
        void readReal(sreal& val);
        void readData(SData& dat, size_t size);
        void readSString(SString& str);
        void readSArray(SArray& array);
        void readSDict(SDictionary& dict);
        void readSObject(sobj& obj);
        void operator>>(String& s);
        //void operator>>(SString& s);
        void operator>>(SData& d);
        
        void writeChars(const char* str);
        void writeCharsAs(const char* str, const size_t size);
        void writeString(const String& str);
        void writeString(const SString& str);
        void writeBytes(const void* dat, size_t size);
        void writeChar(const char& c);
        void writeUChar(const unsigned char& c);
        void writeByte(const sbyte& val);
        void writeUByte(const subyte& val);
        void writeShort(const sshort& val);
        void writeUShort(const sushort& val);
        void writeInt(const sint& val);
        void writeUInt(const suint& val);
        void writeInteger(const sinteger& val);
        void writeUInteger(const suinteger& val);
        void writeFloat(const float& val);
        void writeDouble(const double& val);
        
        void writeSString(const SString& str);
        void writeSArray(const SArray& array);
        void writeSDict(const SDictionary& dict);
        void writeSObject(const sobj& obj);

        SFile& operator<<(const int& val);
        SFile& operator<<(const unsigned int& val);
        SFile& operator<<(const size_t& val);
        SFile& operator<<(const int64_t& val);
#if defined(MAC_OS)
        SFile& operator<<(const suinteger& val);
#endif
        SFile& operator<<(const float& val);
        SFile& operator<<(const double& val);
        SFile& operator<<(const char* s);
        SFile& operator<<(const std::string& s);
        SFile& operator<<(const Char& c);
        SFile& operator<<(const String& s);
        SFile& operator<<(const SString& s);
        SFile& operator<<(const SDate& d);
        SFile& operator<<(const SData& d);
        SFile& operator<<(const SElement& e);
        SFile& operator<<(const SObjPtr& o);

        operator bool() const;

        String getClass() const;
        String toString(const char *format = nullptr) const;
        SObject* clone() const;
    };
    #define SDirectory(X) SFile((X), slib::sio::DIRECTORY)
}
extern std::ostream& operator<<(std::ostream& os, const slib::SFile& file);
#endif
