#ifndef SLIB_SFILE_H
#define SLIB_SFILE_H

#include "sutil/ssys.h"
#include "sio/sio.h"

namespace slib {
    
    namespace sio {
		#define SDirectory(X) SFile((X), DIRECTORY)

        class SLIB_DLL SFile : public SObject {
		public:
			enum SEEK_ORI {
				BEGIN = 0,
				CURRENT = 1,
				END = 2,
			};

        private:
			::std::fstream _stream;
            String _path;
            suinteger _size;
			subyte _mode;
			//suint _access;
			//sarray _dates;
			//sdict _attribute;

        private:
            void _setPath(const char *path);
            void _open();
            void _make();
            
        public:
            SFile();
            SFile(const char *path, int m = 0);
            SFile(const SFile &file);
            ~SFile();
            
            SFile &operator=(const char *path);
            SFile &operator=(const SFile &file);
            
            SFile &operator+=(const char *s);
            SFile operator+(const char *s) const;
            
            //static
            static SFile current();
            static SFile home();
            static SFile createFile(const char *path);
            static SFile makeDir(const char *path);
            
            //variable access
            String path() const;
            suinteger size() const;
            int mode() const;
			suinteger offset();
            void setOffset(suinteger off, bool rev = false);
            void moveOffset(sinteger off);
			void seek(sinteger off, SEEK_ORI ori);
			void flush();
			void clear();

            stringarray components(const char *sep = PATH_SEPARATOR) const;
            String filename(bool ext = true, const char *sep = PATH_SEPARATOR) const;
            String extension(bool low = true) const;
            
            //state check
            bool isOpened() const;
            bool exist() const;
            bool isDir() const;
            bool isRoot() const;
            #if defined(WIN32_OS) || defined(WIN64_OS)
            bool isDriveRoot(const char *s) const;
            #endif
            bool empty() const;
			bool eof();
			 
            //file/dir access
            void open(const char *path = nullptr, int m = sio::READ);
            void make(const char *path = nullptr, int m = sio::CREATE);
            void edit(const char *path = nullptr, int m = sio::WRITE);
			void append(const char* path = nullptr, int m = sio::APPEND);
            void close();
            
            void updir();
            void setdir(const char *name);
            void select(const char *name);
            
            void copyTo(const char *path, int op = 0);
            void moveTo(const char *path, int op = 0);
            void rename(const char *name, int op = 0);
            void remove();
            
            void archive(const char *path = nullptr);
            void expand(const char *path = nullptr);
            
            SFile parent(const char *sep = PATH_SEPARATOR) const;
            SFile child(const char *name, int m = 0) const;
            filearray fileList(const stringarray &ext = {}, bool recur = false, bool hidden = false);
            filearray find(const char *que, bool recur = false);
            
            String getClass() const;
            String toString() const;
            SObject *clone() const;
            
            //input stream
            void readChars(char *str, size_t size);
            void readString(String &str, size_t size);
            void readLine(String &str);
            void readTo(String &str, const char *end);
            void readBytes(void *dat, size_t size);
            void readChar(char &c);
            void readUChar(unsigned char &c);
            void readByte(sbyte &val);
            void readUByte(subyte &val);
            void readShort(sshort &val);
            void readUShort(sushort &val);
            void readInt(sint &val);
            void readUInt(suint &val);
            void readInteger(sinteger &val);
            void readUInteger(suinteger &val);
            void readFloat(float &val);
            void readDouble(double &val);
            void readReal(sreal &val);
            void readData(SData &dat, size_t size);
            void readSObject(sobj &obj);
            void operator>>(String &s);
            void operator>>(SString &s);
            void operator>>(SData &d);
            //output stream
            void writeChars(const char *str);
            void writeString(const String &str);
            void writeString(const SString &str);
            void writeBytes(const void *dat, size_t size);
            void writeChar(const char &c);
            void writeUChar(const unsigned char &c);
            void writeByte(const sbyte &val);
            void writeUByte(const subyte &val);
            void writeShort(const sshort &val);
            void writeUShort(const sushort &val);
            void writeInt(const sint &val);
            void writeUInt(const suint &val);
            void writeInteger(const sinteger &val);
            void writeUInteger(const suinteger &val);
            void writeFloat(const float &val);
            void writeDouble(const double &val);
            void writeReal(const sreal &val);
            void writeSObject(const sobj &obj);
            
            SFile &operator<<(const int &val);
            SFile &operator<<(const unsigned int &val);
            SFile &operator<<(const size_t &val);
            SFile &operator<<(const sinteger &val);
#if defined(MAC_OS)
            SFile &operator<<(const suinteger &val);
#endif
            SFile &operator<<(const float &val);
            SFile &operator<<(const double &val);
            SFile &operator<<(const char *s);
            SFile &operator<<(const std::string &s);
            SFile &operator<<(const String &s);
            SFile &operator<<(const SString &s);
            SFile &operator<<(const SDate &d);
            SFile &operator<<(const sobj &obj);
            
            operator const char *() const;
            
            bool operator<(const SFile &file) const;
            bool operator==(const SFile &file) const;
        };

		extern inline std::ostream& operator<<(std::ostream& os, const sio::SFile& file) { return os << file.path(); }

    }
}
#endif
