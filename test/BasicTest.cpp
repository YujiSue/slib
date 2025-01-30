
#include "Test.h"

void test::RangeTest() {
// Header
	DOUBLE_LINE
	PRINT_TITLE("Range Test")
	SINGLE_LINE

// Init.
	srange r1;
	SPrint("srange r1");
	PRINT_VALUE("r1", r1);
	srange r2(1, 10);
	SPrint("srange r2(1, 10)");
	PRINT_VALUE("r2", r2);
	srange r3(-1, 1);
	SPrint("srange r3(-1, 1)");
	PRINT_VALUE("r3", r3);
	BROKEN_LINE
// Func.
	PRINT_RESULT("r2.length()", r2.length()); // 9
	PRINT_RESULT("r2.length(closed = true)", r2.length(true)); // 10
	PRINT_RESULT("r2.include(r1)", SNumber(r2.include(r1))); // false
	r2.shift(-5);
	SPrint("r2.shift(-5)");
	PRINT_VALUE("r2", r2); // (-4, 5)
	PRINT_RESULT("r2.include(r1)", SNumber(r2.include(r1))); // true
	PRINT_RESULT("r2.overlap(r3)", SNumber(r2.include(r1))); // true
	r3.expand(10);
	SPrint("r3.expand(10)");
	PRINT_VALUE("r3", r3); // (-1 , 11)
	r1 = r2 | r3; // merge
	SPrint("r1 = r2 | r3");
	PRINT_VALUE("r1", r1); // (-4 , 11)
	r1 = r2 & r3;
	SPrint("r1 = r2 & r3"); // conjunction
	PRINT_VALUE("r1", r1); // (-1 , 5)
	r2.mask(r3);
	SPrint("r2.mask(r3)"); // conjunction
	PRINT_VALUE("r2", r2); // (-1 , 5)
	PRINT_RESULT("(r1 == r2)", SNumber(r1 == r2)); // true
	PRINT_RESULT("(r1 < r3)", SNumber(r1 < r2)); // true
	DOUBLE_LINE
};
void test::AreaTest() {
	// Header
	DOUBLE_LINE
	PRINT_TITLE("Area Test")
	SINGLE_LINE
	// Init.
	sarea a1;
	SPrint("sarea a1");
	PRINT_VALUE("a1", a1);
	sarea a2(0, 1, 2, 3);
	SPrint("sarea a2(0,1,2,3)");
	PRINT_VALUE("a2", a2);
	sarea a3(-1, 1, 2, 2);
	SPrint("sarea a3(-1,1,2,2)");
	PRINT_VALUE("a3", a3);
	BROKEN_LINE
	PRINT_RESULT("a2.area()", a2.area()); // 6
	PRINT_RESULT("a1.include(-1,-1)", SNumber(a1.include(-1, -1))); // false
	PRINT_RESULT("a2.overlap(a3)", SNumber(a2.overlap(a3))); // true
	PRINT_RESULT("a3.include(0,0)", SNumber(a3.include(0, 0))); // false
	PRINT_RESULT("(a2 | a3)", (a2 | a3)); // -1, 1, 3, 3
	PRINT_RESULT("(a2 & a3)", (a2 & a3)); // 0, 1, 1, 2
	DOUBLE_LINE
}
void test::ZoneTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "Zone Test" << LF << std::endl;
	szone z1(0, 1, 2, 3, 4, 5);
	szone z2(z1);
	szone z3(-2, -1, 0, 3, 3, 3);

	std::cout << z1 << std::endl; // 0, 1, 2, 3, 4, 5
	std::cout << z2.volume() << std::endl; //60
	std::cout << (z1.include(0, 0, 0) ? "true" : "false") << std::endl; //false
	std::cout << (z2.overlap(z3) ? "true" : "false") << std::endl; //true
	std::cout << (z3.include(0, 0, 0) ? "true" : "false") << std::endl; //true
	z2.merge(z3);
	std::cout << z2 << std::endl; //-2, -1, 0, 5, 6. 7
	std::cout << z1.conjunction(z3) << std::endl; //0, 1, 2, 1, 1, 1 
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
void test::MemoryTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "Memory Test" << LF << std::endl;
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
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
void test::PtrTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "Ptr Test" << LF << std::endl;
	sptr<int> ptr1 = 1;
	sptr<int> ptr2(ptr1);
	sptr<int> ptr3;
	{
		sptr<int> ptr4 = 100;
		ptr3 = ptr4;
	}
	std::cout << *ptr1 << ":" << *ptr2 << ":" << *ptr3 << std::endl; // 1:1:100
	std::cout << LF << String("*") * 50 << LF << std::endl;
}


void test::ArrayTest() {
	// Header
	DOUBLE_LINE
	PRINT_TITLE("Array Test")
	SINGLE_LINE
	// Init.
	
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
	std::cout << ia1 << std::endl; // 1 2 3 4 5 6 7 8 9 10
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
	std::cout << ia2 << std::endl; // 7 8 9
	ia2.reset(0);
	std::cout << ia2 << std::endl; // 0 0 0
	ia3.copyTo(ia2);
	std::cout << ia2 << std::endl; // 7 8 9
	ia1.set(0, 0);
	ia1.exchange(1, 8);
	ia1.insert(3, -1);
	std::cout << ia1 << std::endl;
	ia1.remove(5, 2);
	std::cout << ia1 << std::endl; // 0 9 3 -1 4 7 8 2 10 
	ia1.sort();
	std::cout << ia1 << std::endl; // -1 0 2 3 4 7 8 9 10 
	auto it1 = ia1.begin() + 3;
	it1 = ia1.remove(it1, it1 + 2);
	std::cout << ia1 << std::endl; // -1 0 2 3 4 7 8 9 10 
	std::cout << *it1 << std::endl; //7
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
	std::cout << ia1 << std::endl; // 3 1 4 1 5 9 2 6
	try {
		std::cout << ia1[20] << std::endl;
	}
	catch (SException ex) { ex.print(); }
	stringarray sa1 = { u8"色は", u8"匂へど", u8"散りぬるを" };
	sa1.erase(u8"匂へど");
	std::cout << sa1 << std::endl; 

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
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
void test::RegionTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "Region Test" << LF << std::endl;
	sregion r1 = { srange(1, 2), srange(3, 4), srange(5, 6),srange(11, 20), srange(101, 200) };
	std::cout << r1.length(true) << std::endl; //116
	std::cout << r1.range() << std::endl; //1, 200
	sregion r2(r1);
	sregion r3 = { srange(5, 15), srange(51, 60) };
	std::cout << (r1.include(15) ? "true" : "false") << std::endl; //true
	std::cout << (r1.include(srange(15, 20)) ? "true" : "false") << std::endl; //true
	std::cout << (r1.overlap(srange(31, 40)) ? "true" : "false") << std::endl; //false
	std::cout << (r1.overlap(r3) ? "true" : "false") << std::endl; //true
	std::cout << r1.subregion(srange(15, 120)) << std::endl; //(15,20),(101,120)
	r2.shift(-10);
	std::cout << r2 << std::endl;//(-9,-8),(-7,-6),(-5,-4),(1,10),(91,190)
	r2.expand(0, 10);
	std::cout << r2 << std::endl;//(-9,-8),(-7,-6),(-5,-4),(11,20),(101,200)
	r2.merge(srange(5, 30));
	std::cout << r2 << std::endl;//(-9,-8),(-7,-6),(-5,-4),(5,30),(101,200)
	r2.merge(r3);
	std::cout << r2 << std::endl;//(-9,-8),(-7,-6),(-5,-4),(5,30),(51,60),(101,200)
	r2.conjunction(srange(0, 100));
	std::cout << r2 << std::endl;//(5,30),(51,60)
	r1.exclude(srange(10, 150));
	std::cout << r1 << std::endl;//(1,2),(3,4),(5,6),(151,200)
	std::cout << r1.find(6) << std::endl;//(2, 2)
	std::cout << r1.find(srange(2, 5)) << std::endl;//(0, 2)
	std::cout << complement(srange(0, 300), r1) << std::endl; // (0,0),(7,150),(201,300)
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
void test::ListTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "List Test" << LF << std::endl;
	slib::List<sint> l0;
	l0.add(10);
	std::cout << l0[0] << std::endl;
	std::cout << l0.last() << std::endl;

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
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
void test::MapTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "Map Test" << LF << std::endl;
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
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
void test::SetTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "Set Test" << LF << std::endl;
	Set<int, double, const char*> set1(4, 2.5, "10");
	std::cout << set1.getValue<0>() * set1.getValue<1>() * String(set1.getValue<2>()).intValue() << std::endl;
	set1.setValue<1>(0.15);
	std::cout << set1.getValue<0>() * set1.getValue<1>() * String(set1.getValue<2>()).intValue() << std::endl;
	Set<int, double, const char*> set2 = set1;
	std::cout << set1.getValue<0>() << "," << set1.getValue<1>() << "," << set1.getValue<2>() << std::endl;
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
void test::CharTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "Char Test" << LF << std::endl;
	std::cout << String(u8"Size of utf-8 char. あ = ") << (int)Char::u8size(u8"あ") << std::endl;
	std::cout << String(u8"'あ' is wide?") << (Char::isWide(u8"あ")?"true":"false") << std::endl;
	std::cout << String(u8"'あ' is narrow?") << (Char::isNarrow(u8"あ")?"true":"false") << std::endl;
	std::cout << String(u8"'５' is numeric?") << (Char::isNumeric(u8"５") ? "true" : "false") << std::endl;
	
	std::cout << Char::wideChar('5') << std::endl;
	std::cout << Char::wideChar('a') << std::endl;
	std::cout << Char::narrowChar("３") << std::endl;
	std::cout << Char::narrowChar("Ｇ") << std::endl;

	String s1 = u8"木花之佐久夜毘売";
	auto c = s1.u8charAt(3);
	std::cout << c << std::endl;
	std::cout << c + 3 << std::endl;
	std::cout << c.index() << std::endl;
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
void test::StringTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "String Test" << LF << std::endl;

	String s0 = "\t abcdef\n";

	std::cout << s0 << std::endl;
	s0 = String::trim(s0);
	std::cout << s0 << std::endl;
	std::cout << String::squot(s0) << std::endl;
	std::cout << String::dquot(s0) << std::endl;
	s0 = String::upper(s0);
	std::cout << s0 << std::endl;
	s0 = String::lower(s0);
	std::cout << s0 << std::endl;
	s0 = String::wide(s0);
	std::cout << s0 << std::endl;
	s0 = String::narrow(s0);
	s0 = String::enclose(s0, "<", ">");
	std::cout << s0 << std::endl;
	std::cout << String::dequot(s0) << std::endl;

	ubytearray a1 = { 0x43, 0x4F, 0x32 };
	String s1(100);
	String s2(3.14);
	String s3("abc");
	String s4 = S(子丑寅卯辰巳午未申酉戌亥);

	std::cout << (s1.isNumeric()?"true":"false") << std::endl;
	std::cout << (s2.isNumeric() ? "true" : "false") << std::endl;
	std::cout << (s3.isNumeric() ? "true" : "false") << std::endl;

	std::cout << s3.size() << std::endl;
	std::cout << s3.length() << std::endl;
	std::cout << s1[0] << std::endl;
	std::cout << s2[1] << std::endl;
	std::cout << s3[2] << std::endl;

	std::cout << (s3 + 1.414) << std::endl;
	std::cout << s1.integer() + 1.732 << std::endl;
	std::cout << s2.doubleValue() + 2.71828 << std::endl;
	s1.fill(10, ' ', true);
	std::cout << s1 << std::endl;
	std::cout << s2.filled(10, '0') << std::endl;
	s1.trimming();
	std::cout << s1 << std::endl;

	std::cout << s4.size() << std::endl;
	std::cout << s4.length() << std::endl;
	std::cout << s4.charCount() << std::endl;
	std::cout << s4.u8charAt(2) << std::endl;
	std::cout << s4.charIndex(2) << std::endl;

	String s5;
	s5.interpret(a1.ptr(), a1.size());
	std::cout << s5 << std::endl;
	s5.add('"');
	s5.insert(0, "\"");
	std::cout << s5 << std::endl;
	std::cout << (s5.isQuoted()?"true":"false") << std::endl;

	String s6 = "abc";
	s6 *= 5;
	std::cout << s6 << std::endl;
	s6.append(String("xyz") * 5);
	std::cout << s6 << std::endl;
	s6.remove(8, 10);
	std::cout << s6 << std::endl;
	std::cout << s6.count("a") << ":" << s6.count("b") << ":" << s6.count("c") << ":" << s6.count("x") << ":" << s6.count("y") << ":" << s6.count("z") << std::endl;
	std::cout << (s6.beginWith("ab")?"true":"false") << std::endl;
	std::cout << (s6.beginWith("yz") ? "true" : "false") << std::endl;
	std::cout << (s6.endWith("ab") ? "true" : "false") << std::endl;
	std::cout << (s6.endWith("yz") ? "true" : "false") << std::endl;
	s6.replace(11, 3, "def");
	std::cout << s6 << std::endl;
	s6.replace("xyz", "123");
	std::cout << s6 << std::endl;
	std::cout << s6.substring(2, 10) << std::endl;
	std::cout << s6 << std::endl;
	std::cout << (s6.contain("ab") ? "true" : "false") << std::endl;
	std::cout << (s6.contain("xy") ? "true" : "false") << std::endl;
	std::cout << s6.search("1") << std::endl;
	std::cout << s6.split("1") << std::endl;
	std::cout << s6.find("a") << std::endl;
	std::cout << s6.find("1") << std::endl;
	std::cout << s6.rfind("a") << std::endl;
	std::cout << s6.rfind("1") << std::endl;
	std::cout << s6 << std::endl;
	std::cout << (s6.match(REG(/\\d{3}/))?"true":"false") << std::endl;
	std::cout << s6.search(REG(/[a-z]{1}[0-9]{1}/g)) << std::endl;
	s6.replace(REG(/\\d{3}/g), "456");
	std::cout << s6 << std::endl;
	s6.clip(16);
	std::cout << s6 << std::endl;
	std::cout << (s6.equal(REG(/\\d+/)) ? "true" : "false") << std::endl;
	std::cout << (s6.equal(REG(/[a-z]+/)) ? "true" : "false") << std::endl;
	String s7 =
		S(Twas brillig, and the slithy toves)
		+ LF +
		S(Did gyre and gimble in the wabe;)
		+ CR +
		S(All mimsy were the borogoves, )
		+ CRLF +
		S(And the mome raths outgrabe.);
	std::cout << toString(s7.splitline(), " = ") << std::endl;
	String s8 =
		"Edmund:Why brand they us with base?"
		+ LF +
		"Hamlet:Could not, with all their quantity of love, make up my sum."
		+ LF +
		"Macbeth:Life's but a walking shadow, a poor player."
		+ LF +
		"Puck:Lord, what fools these mortals be!";
	std::cout << toString(s8.parse(LF,":")) << std::endl;
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
void test::TimeTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "Time Test" << LF << std::endl;
	std::cout << (Time::isLeapYear(1900) ? "true" : "false") << std::endl; //false
	std::cout << (Time::isLeapYear(2000) ? "true" : "false") << std::endl; //true
	//For Japanese
	std::cout << slocale::JPYear(2000).first << slocale::JPYear(2000).second << S(年) << std::endl; //平成１２年
	std::cout << S(西暦) << slocale::ADYear(u8"昭和", 20) << S(年) << std::endl; //西暦1945年
	
	Time t1;
	Time t2("1Y2M3D");
	Time t3("３年６ヶ月");
	Time t4("１２時間５０分");
	Time t5("10h30m45s");

	std::cout << t3.toString() << std::endl;
	std::cout << (t2+t3).toString() << std::endl;
	std::cout << (t4-t5).toString() << std::endl;
	std::cout << t5.toJString() << std::endl;
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
class TestNode : public Node<TestNode> {
public:
	String nodeid;
	TestNode() {}
	TestNode(const char *s) : nodeid(s) {}
};
void test::NodeTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "Node Test" << LF << std::endl;
	TestNode root("root"),
		node1("node1"),
		node2("node1-1"),
		node3("node2"),
		node4("node3");

	root.addChild(node1);
	root[0].addChild(node2);
	root.addChild(node3);
	root.addChild(node4);

	std::cout << root.childCount() << std::endl;
	std::cout << root[0].nodeid << std::endl;
	std::cout << (root[0].isLeaf() ? "leaf" : "stem") << std::endl;
	std::cout << root[0][0].layer() << std::endl;
	std::cout << (root[1].isLeaf() ? "leaf" : "stem") << std::endl;
	std::cout << root[2].index() << std::endl;

	TestNode node5("node1-0");
	std::cout << root[0][0].nodeid << "=>";
	root[0].insertChild(0, node5);
	std::cout << root[0][0].nodeid << "," << root[0][1].nodeid << "=>";
	TestNode node6("node1-1'");
	root[0].setChild(1, node6);
	std::cout << root[0][0].nodeid << "," << root[0][1].nodeid << std::endl;
	std::cout << root[1].nodeid << "=>";
	root.removeChildAt(1);
	std::cout << root[1].nodeid << std::endl;
	root[1].moveChildTo(root[0][0], 0);
	std::cout << root[0][0].childCount() << std::endl;
	std::cout << root[0][0][0].nodeid << std::endl;
	std::cout << root[0][0][0].address() << std::endl;
	std::cout << root.childCount() << "=>";
	root.clearChildren();
	std::cout << root.childCount() << std::endl;
	std::cout << LF << String("*") * 50 << LF << std::endl;
}
void test::ExceptionTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "Exception Test" << LF << std::endl;
	try {
		sptr<int> p;
		std::cout << *p << std::endl;
	}
	catch (SException e) {
		EXPORT_MSG(e);
	}
	std::cout << LF << String("*") * 50 << LF << std::endl;
}