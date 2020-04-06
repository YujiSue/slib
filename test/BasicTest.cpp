#include "sobj.h"
#include "Test.h"

using namespace slib;

void test::MemoryTest() {
	char *str;
	const char *str1 = "abcdefg";
	const char *str2 = "xyz";
	str = CMemory<char>::alloc(16);
	std::cout << (int)str[0] << std::endl; // ?
	CMemory<char>::init(str, 16);
	std::cout << (int)str[0] << std::endl; // 0
	CMemory<char>::copy(str, str1, 7);
	std::cout << str << std::endl; // abcdefg
	CMemory<char>::shift(&str[3], str, 7);
	CMemory<char>::copy(str, str2, 3);
	std::cout << str << std::endl; // xyzabcdefg
	CMemory<char>::release(&str[6], 11);
	std::cout << str << std::endl; // xyzabc

}
void test::RangeTest() {
	srange r1;
	srange r2(1, 10);
	srange r3(-1, 1);
	srangef rf1;
	srangef rf2(-10.0f, 10.0f);
	sranged rd(1.0, 3.1415926);

	std::cout << r1.begin << "," << r1.end << std::endl; // 0 , 0
	std::cout << r2.length() << ":" << r2.length(true) << std::endl; //9 : 10
	std::cout << (r2.include(r1) ? "true" : "false") << std::endl; //false
	r2.shift(-5);
	std::cout << r2.begin << "," << r2.end << std::endl; //-4 , 5
	std::cout << (r2.include(r1) ? "true" : "false") << std::endl; //true
	std::cout << (r2.overlap(r3) ? "true" : "false") << std::endl; //true
	r3.expand(10);
	std::cout << r3.begin << "," << r3.end << std::endl; //-1 , 11
	r1 = r2.merged(r3);
	std::cout << r1.begin << "," << r1.end << std::endl; // -4 , 11
	r1 = r2.conjunctioned(r3);
	std::cout << r1.begin << "," << r1.end << std::endl; // -1 , 5
	r2.conjunction(r3);
	std::cout << (r1 == r2 ? "true" : "false") << std::endl; //true
	std::cout << (rf1 == rf2 ? "true" : "false") << std::endl; //false
	std::cout << (rf1 < rf2 ? "true" : "false") << std::endl; //false
	std::cout << (0.0 < rd ? "true" : "false") << std::endl; //true
};
void test::PtrTest() {
	sptr<int> ptr1 = 1; 
	sptr<int> ptr2(ptr1);
	sptr<int> ptr3;
	{
		sptr<int> ptr4 = 100;
		ptr3 = ptr4;
	}
	std::cout << *ptr1 << ":" << *ptr2 << ":" << *ptr3 << std::endl;
}

void test::ArrayTest() {
	slib::Array<sint> ia1 = { 1, 2, 3, 4, 5 };
	slib::Array<sint> ia2;
	ia2.reserve(10);
	std::cout << ia1.size() << std::endl; //5
	std::cout << ia2.capacity() << std::endl; //10
	std::cout << ia2.size() << std::endl; //0
	ia2 = ia1;
	std::cout << ia2.size() << std::endl; //5
	std::cout << ia2.first() << ":" << ia1[2] << ":"<< ia2.last() << std::endl; //1:3:5
	slib::Array<sint> ia3 = { 7, 8, 9, 10 };
	ia1.add(6);
	ia1.append(ia3);
	sforeach(ia1) { std::cout << E_ << " "; }
	std::cout << std::endl; // 1 2 3 4 5 6 7 8 9 10
	std::cout << (sinteger)ia1.ptr() << "-" << (sinteger)ia1.ptr(ia1.size()) << std::endl; // X - X+0x28
	ia2.clear();
	std::cout << ia2.size() << std::endl; // 0
	std::cout << (ia2.empty()?"true":"false") << std::endl; // true
	std::cout << ia3.get() << std::endl; // 10
	auto ia1_ = ia1.subarray(6, 3);
	std::cout << (ia1_ == ia3 ? "true" : "false") << std::endl; // true
	ia1_.moveTo(ia2);
	std::cout << ia2.size() << std::endl; // 3
	std::cout << ia1_.size() << std::endl; // 0
	sforeach(ia2) { std::cout << E_ << " "; }
	std::cout << std::endl; // 7 8 9
	ia2.reset(0);
	sforeach(ia2) { std::cout << E_ << " "; }
	std::cout << std::endl; // 0 0 0
	ia3.copyTo(ia2);
	sforeach(ia2) { std::cout << E_ << " "; }
	std::cout << std::endl; // 7 8 9
	ia1.set(0, 0);
	ia1.exchange(1, 8);
	ia1.insert(3, -1);
	ia1.remove(5, 2);
	sforeach(ia1) { std::cout << E_ << " "; }
	std::cout << std::endl; // 0 9 3 -1 4 7 8 2 10 
	ia1.sort();
	sforeach(ia1) { std::cout << E_ << " "; }
	std::cout << std::endl; // -1 0 2 3 4 7 8 9 10 
	ia3.resize(8);
	ia3[0] = 3;
	ia3[1] = 1;
	ia3[2] = 4;
	ia3[3] = 1;
	ia3[4] = 5;
	ia3[5] = 9;
	ia3[6] = 2;
	ia3[7] = 6;
	std::cout << (ia3.contain(0)?"true":"false") << std::endl; // false
	std::cout << ia3.find(1) << std::endl; // 1
	std::cout << ia3.rfind(1) << std::endl; // 3
	std::cout << ia3.find(0) << std::endl; // -1
	ia3.swap(ia1);
	sforeach(ia1) { std::cout << E_ << " "; }
	std::cout << std::endl; // 3 1 4 1 5 9 2 6
	try {
		std::cout << ia1[20] << std::endl;
	}
	catch (SException ex) { ex.print(); }

	slib::BiArray<sint> ba1 = { 1, 2, 3, 4, 5 };
	slib::BiArray<sint> ba2;
	ba2.reserve(10);
	std::cout << ba1.offset() << "-" << ba1.size() << "-" << ba1.capacity() << std::endl; //1-5-7
	std::cout << ba2.offset() << "-" << ba2.size() << "-" << ba2.capacity() << std::endl; //5-0-10
	ba2 = ba1;
	std::cout << ba2.offset() << "-" << ba2.size() << "-" << ba2.capacity() << std::endl; //5-5-20
	std::cout << ba2.first() << ":" << ba1[2] << ":" << ba2.last() << std::endl; //1:3:5
	slib::BiArray<sint> ba3 = { 7, 8, 9, 10 };
	slib::BiArray<sint> ba4 = { -5, -4, -3, -2, -1 };
	ba1.add(6);
	ba1.append(ba3);
	ba1.put(0);
	ba1.pile(ba4);
	sforeach(ba1) { std::cout << E_ << " "; }
	std::cout << std::endl; // -5 -4 -3 -2 -1 0 1 2 3 4 5 6 7 8 9 10
	std::cout << (sinteger)ba1.ptr() << "-" << (sinteger)ba1.ptr(ba1.size()) << std::endl; // X - X+0x40
	ba2.clear();
	std::cout << ba2.offset() << "-" << ba2.size() << "-" << ba2.capacity() << std::endl; //5-5-20
	std::cout << (ba2.empty() ? "true" : "false") << std::endl; // true
	std::cout << ba3.pick() << std::endl; // 7
	std::cout << ba3.get() << std::endl; // 10
	auto ba1_ = ba1.subarray(13, 2);
	std::cout << (ba1_ == ba3 ? "true" : "false") << std::endl; // true
	ba1_.moveTo(ba2);
	std::cout << ba2.offset() << "-" << ba2.size() << "-" << ba2.capacity() << std::endl; //5-5-20
	std::cout << ba1_.offset() << "-" << ba1_.size() << "-" << ba1_.capacity() << std::endl; //5-5-20
	sforeach(ba2) { std::cout << E_ << " "; }
	std::cout << std::endl; // 7 8 9
	ba2.reset(0);
	sforeach(ba2) { std::cout << E_ << " "; }
	std::cout << std::endl; // 0 0 0
	ba3.copyTo(ba2);
	sforeach(ba2) { std::cout << E_ << " "; }
	std::cout << std::endl; // 7 8 9
	ba1.set(0, 0);
	ba1.exchange(1, 8);
	ba1.insert(3, -1);
	ba1.remove(5, 2);
	sforeach(ba1) { std::cout << E_ << " "; }
	std::cout << std::endl; // 0 9 3 -1 4 7 8 2 10 
	ba1.sort();
	sforeach(ba1) { std::cout << E_ << " "; }
	std::cout << std::endl; // -1 0 2 3 4 7 8 9 10 
	ba3.resize(8);
	ba3[0] = 3;
	ba3[1] = 1;
	ba3[2] = 4;
	ba3[3] = 1;
	ba3[4] = 5;
	ba3[5] = 9;
	ba3[6] = 2;
	ba3[7] = 6;
	std::cout << (ba3.contain(0) ? "true" : "false") << std::endl; // false
	std::cout << ba3.find(1) << std::endl; // 1
	std::cout << ba3.rfind(1) << std::endl; // 3
	std::cout << ba3.find(0) << std::endl; // -1
	ba3.swap(ba1);
	sforeach(ba1) { std::cout << E_ << " "; }
	std::cout << std::endl; // 3 1 4 1 5 9 2 6
	try {
		std::cout << ba1[20] << std::endl;
	}
	catch (SException ex) { ex.print(); }
}

void test::ListTest() {
	slib::List<sint> l1 = { 1, 2, 3, 4, 5 };
	slib::List<sint> l2;
	l2.reserve(10);
	std::cout << l1.size() << "-" << l1.capacity() << std::endl; //1-5-7
	std::cout << l2.size() << "-" << l2.capacity() << std::endl; //5-0-10
	l2 = l1;
	std::cout << l2.size() << "-" << l2.capacity() << std::endl; //5-5-20
	std::cout << l2[2] << ":" << l2.last() << std::endl; //1:3:5
	slib::List<sint> l3 = { 7, 8, 9, 10 };
	slib::List<sint> l4 = { -5, -4, -3, -2, -1 };
	l1.add(6);
	l1.append(l3);
	l1.put(0);
	l1.pile(l4);
	sforeach(l1) { std::cout << E_ << " "; }
	std::cout << std::endl; // -5 -4 -3 -2 -1 0 1 2 3 4 5 6 7 8 9 10
	l2.clear();
	std::cout << l2.size() << "-" << l2.capacity() << std::endl; //5-5-20
	std::cout << (l2.empty() ? "true" : "false") << std::endl; // true
	std::cout << l3.pick() << std::endl; // 7
	std::cout << l3.get() << std::endl; // 10
	auto l1_ = l1.sublist(13, 2);
	std::cout << (l1_ == l3 ? "true" : "false") << std::endl; // true
	l1_.moveTo(l2);
	std::cout << l2.size() << "-" << l2.capacity() << std::endl; //5-5-20
	std::cout << l1_.size() << "-" << l1_.capacity() << std::endl; //5-5-20
	sforeach(l2) { std::cout << E_ << " "; }
	std::cout << std::endl; // 7 8 9
	l2.reset(0);
	sforeach(l2) { std::cout << E_ << " "; }
	std::cout << std::endl; // 0 0 0
	l3.copyTo(l2);
	sforeach(l2) { std::cout << E_ << " "; }
	std::cout << std::endl; // 7 8 9
	l1.set(0, 0);
	sforeach(l1) { std::cout << E_ << " "; }
	std::cout << std::endl;
	l1.exchange(1, 8);
	sforeach(l1) { std::cout << E_ << " "; }
	std::cout << std::endl;
	l1.insert(3, -1);
	sforeach(l1) { std::cout << E_ << " "; }
	std::cout << std::endl;
	l1.remove(5, 2);
	sforeach(l1) { std::cout << E_ << " "; }
	std::cout << std::endl; // 0 9 3 -1 4 7 8 2 10 
	l1.sort();
	sforeach(l1) { std::cout << E_ << " "; }
	std::cout << std::endl; // -1 0 2 3 4 7 8 9 10 
	l3.resize(8);
	l3[0] = 3;
	l3[1] = 1;
	l3[2] = 4;
	l3[3] = 1;
	l3[4] = 5;
	l3[5] = 9;
	l3[6] = 2;
	l3[7] = 6;
	std::cout << (l3.contain(0) ? "true" : "false") << std::endl; // false
	std::cout << l3.find(1) << std::endl; // 1
	std::cout << l3.rfind(1) << std::endl; // 3
	std::cout << l3.find(0) << std::endl; // -1
	l3.swap(l1);
	sforeach(l1) { std::cout << E_ << " "; }
	std::cout << std::endl; // 3 1 4 1 5 9 2 6
	try {
		std::cout << l1[20] << std::endl;
	}
	catch (SException ex) { ex.print(); }
}

void test::MapTest() {
	slib::Map<String, sint> m1 = { kvpair<String, sint>("a", 1),  kvpair<String, sint>("b", 2),  kvpair<String, sint>("c", 3) };
	slib::Map<String, sint> m2;
	m2.reserve(10);
	std::cout << m1.size() << "-" << m1.capacity() << std::endl; //
	std::cout << m2.size() << "-" << m2.capacity() << std::endl; //
	std::cout << (m2.empty() ? "true" : "false") << std::endl; // true
	m2 = m1;

	slib::Map<String, sint> m3;
	m3[u8"い"] = 1;
	m3[u8"ろ"] = 2;
	m3[u8"は"] = 3;

	m1.swap(m3);
	auto keys = m1.keyset();
	std::cout << keys << std::endl;

	m2.append(m1);
	std::cout << m2 << std::endl;
	std::cout << (m2.hasKey("a") ? "true" : "false") << "," << (m2.hasKey(u8"へ") ? "true" : "false") << std::endl;

	std::cout << m2.rlookup(1) << std::endl;

	m1.remove(u8"い");
	std::cout << m1 << std::endl;
	m1.remove(u8"ろ");
	std::cout << m1 << std::endl;
	m1.remove(u8"は");
	std::cout << m1 << std::endl;

	m2.clear();
	std::cout << m2.size() << "-" << m2.capacity() << std::endl;

	slib::Map<sint, sint> m4;
	m4.reserve(5);
	m4.insert(kvpair<sint, sint>(1, 1));
	m4.insert(kvpair<sint, sint>(2, 2));
	m4.insert(kvpair<sint, sint>(3, 3));
	m4.insert(kvpair<sint, sint>(4, 4));
	m4.insert(kvpair<sint, sint>(5, 5));
	m4.insert(kvpair<sint, sint>(6, 6));
	std::cout << m4 << std::endl;
}

void test::CharTest() {
	std::cout << String(u8"Size of utf-8 char. あ = ").localize().cstr() << (int)Char::u8size(u8"あ") << std::endl;
	std::cout << String(u8"'あ' is wide?").localize().cstr() << (Char::isWide(u8"あ")?"true":"false") << std::endl;
	std::cout << String(u8"'あ' is narrow?").localize().cstr() << (Char::isNarrow(u8"あ")?"true":"false") << std::endl;
	std::cout << String(u8"'-123.4' is numeric?").localize().cstr() << (Char::isNumeric(u8"-123.4") ? "true" : "false") << std::endl;
	
	
	/*
	static bool isNumChar(char c);
	static bool isWideNum(const char* s);
	static bool isWSChar(char c);
	static sbyte toNum(char c);
	static sbyte toNum(const char* s);
	static String wideChar(char c);
	static char narrowChar(const char* s);
	*/
}

void test::StringTest() {

}