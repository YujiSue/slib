#include "Test.h"
#include "sobj.h"

using namespace slib;

inline sobj func1(int i, double d, const char* s) {
	return i * d * String(s).intValue();
}
inline void func2(int i) {
	std::cout << "func arg: " << i << std::endl;
}
void test::SObjTest() {
	sobj n1 = 100;
	sobj n2 = 2.5;
	sobj n3 = sfrac("1/4");
	sobj n4 = scomp("1+1i");
	sobj s1 = u8"いろはにほへと";
	sobj s2 = u8"ちりぬるを";
	sobj d1 = SDate();
	sobj array = { n1, n2, n3, n4, s1, s2, d1 };
	sobj dict = { kv("int", n1), kv("real", n2), kv("fraction", n3), kv("complex", n4), kv("string", s1 + s2), kv("date", d1) };

	sobj fn = SFunction<sobj(int, double, const char*)>(func1);

	std::cout << n1 * n2 << std::endl;
	std::cout << n1 * n3 << std::endl;
	std::cout << n2 + n4 << std::endl;
	sforeach(s1) std::cout << E_;
	std::cout << std::endl;
	std::cout << s1[3] << std::endl;
	s1[0] = u8"あ";
	std::cout << s1 * 2 << std::endl;
	std::cout << d1 << std::endl;
	std::cout << array << std::endl;
	std::cout << dict << std::endl;

	std::cout << fn.func<int, double, const char*>()(1, 2.0, "abc") << std::endl;
}
void test::SNumberTest() {
	// Header
	DOUBLE_LINE
	PRINT_TITLE("SNumber Test")
	SINGLE_LINE

	// Init.
	/*
	std::cout << SNumber::toBinary(0x75) << std::endl; //01110101
	std::cout << SNumber::toOct(0x96) << std::endl; //226
	std::cout << SNumber::toHex(135) << std::endl; //87
	std::cout << SNumber::toNumber("100") << std::endl;
	std::cout << SNumber::toNumber("-3.14") << std::endl;
	std::cout << SNumber::toNumber("2.71828e-3") << std::endl;
	std::cout << SNumber::toNumber("YES") << std::endl;
	std::cout << SNumber::toNumber("FALSE") << std::endl;
	std::cout << SNumber::toNumber("1/3") << std::endl;
	std::cout << SNumber::toNumber("-1/10").precised(1) << std::endl;
	std::cout << SNumber::toNumber("0.5+0.3i") << std::endl;
	std::cout << SNumber::toNumber("-1-1i") << std::endl;
	*/

	SNumber num1 = 100;
	SPrint("SNumber num1 = 100");
	PRINT_VALUE("num1", num1);
	SNumber num2 = 3.0;
	SPrint("SNumber num2 = 3.0");
	PRINT_VALUE("num2", num2);
	SNumber num3 = true;
	SPrint("SNumber num3 = true");
	PRINT_VALUE("num3", num3);
	SNumber num4 = sfrac("1/2");
	SPrint("SNumber num4 = sfrac(\"1/2\")");
	PRINT_VALUE("num4", num4);
	SNumber num5 = scomplex("1+1i");
	SPrint("SNumber num5 = scomplex(\"1+1i\")");
	PRINT_VALUE("num5", num5);
	
	BROKEN_LINE

	PRINT_VALUE("-num1", -num1);
	PRINT_VALUE("num1 + num1", num1 + num1);
	PRINT_VALUE("num1 * num2", num1 * num2);
	PRINT_VALUE("num1 / num2", num1 / num2);
	PRINT_VALUE("num1 * num4", num1 * num4);
	PRINT_VALUE("num2 * num4", num2 * num4);
	PRINT_VALUE("num4 + num5", num4 + num5);
	PRINT_VALUE("num2 * num5", num2 + num5);
	PRINT_VALUE("num1 / num5", num1 / num5);

	//BROKEN_LINE


	DOUBLE_LINE
}
void test::SStringTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "SString Test" << std::endl;
	SString str1 = "abc";
	SString str2 = "123";
	SString str3 = u8"いろはにほへと";
	SString str4 = u8"ちりぬるを";

	std::cout << "str1 upper:" << str1.transformed(sstyle::UPPER_CASE) << std::endl; //
	std::cout << "str2 wide:" << str2.transformed(sstyle::FULL_WIDTH) << std::endl; //
	std::cout << "str3+str4:" << str3 + str4 << std::endl; //
	std::cout << "str1+int:" << str1 + 10 << std::endl; //
	std::cout << "str2+num:" << str2 + 0.5 << std::endl; //
	std::cout << "str3 x 2:" << str3 * 2 << std::endl; //
	std::cout << "str4 count:" << str4.charCount() << std::endl;
	std::cout << "str1[1]:" << String(str1[1]) << std::endl;
	std::cout << "str4[2]:" << str4.charAt(2) << std::endl;
	str3.charAt(0) = u8"あ";
	std::cout << "str3:" << str3 << std::endl;
	std::cout << String("*") * 50 << NEW_LINE << std::endl;
}
void test::SDateTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "SDate Test" << std::endl;
	SDate date1;
	std::cout << "date1:" << date1 << std::endl;
	date1 += Time("1Y");
	std::cout << "date1:" << date1 << std::endl;
	SDate date2("2000/01/01");
	std::cout << "date2:" << date2.toString(slib::sstyle::ISO8601) << std::endl;






	SDate jdate1(u8"昭和２０年８月１５日");
	std::cout << "jdate1:" << jdate1.toString(slib::sstyle::ISO8601) << std::endl;
	SDate jdate2(u8"平成元年１月８日");
	std::cout << "jdate2:" << jdate2.toString(slib::sstyle::ISO8601) << std::endl;



	std::cout << String("*") * 50 << NEW_LINE << std::endl;
}
void test::SDataTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "SData Test" << std::endl;
	SData dat1, dat2, dat3;
	SRandom rand;
	sforin(i, 0, 100) {
		auto v = rand.iruni(0, 15);
		dat1.add(v); dat2.add(HEX_STR[v]);
	}


	intarray array;
	String str;
	dat1.asArray<sint>(array);
	std::cout << array << std::endl;
	dat2.asString(str);
	std::cout << str << std::endl;
	std::cout << String("*") * 50 << NEW_LINE << std::endl;
}
void test::SArrayTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "SArray Test" << std::endl;
	SArray array1 = { 100, "abc", true, SDate(), V({u8"いろは", 2.0 }) };
	std::cout << "array1:" << array1 << std::endl;
	SArray array2 = iarray(10);
	std::cout << "array2:" << array2 << std::endl;
	SArray array3 = String("abc,de,f,ghi,jklm").split(",");
	std::cout << "array3:" << array3 << std::endl;
	std::cout << String("*") * 50 << NEW_LINE << std::endl;
}
void test::SDictTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "SDict Test" << std::endl;
	SDictionary dict1 = { kv("int", 100), kv("real",0.5), kv("str",u8"じゅげむ"), kv("array", V({ 100, "abc", true, SDate(), V({u8"いろは", 2.0 })})) };
	std::cout << "dict1:" << dict1 << std::endl;
	SDictionary dict2 = String(u8"id=1;name=Suzuki;value=80").parse(";", "=");
	std::cout << "id:" << dict2["id"] << NEW_LINE << "name:" << dict2["name"] << NEW_LINE << "value:" << dict2["value"] << std::endl;
	std::cout << String("*") * 50 << NEW_LINE << std::endl;
}


void test::SFuncTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "SFunc Test" << std::endl;
	SFunction<sobj(int i, double d, const char* s)> f1(func1);
	std::cout << f1(2, 4.5, "10") << std::endl; //90
	SFunction<void(int)> f2 = func2;
	f2(10); //func arg: 10
	SFunction<void(void)> f3 = []() { std::cout << "lamda test." << std::endl; };
	f3(); //lamda test.
	std::cout << String("*") * 50 << NEW_LINE << std::endl;
}
void test::STableTest() {
	std::cout << String("*") * 50 << std::endl;
	std::cout << "STable Test" << std::endl;
	STable tbl1;
	tbl1.setName("Table");
	tbl1.resize(5, 3);
	tbl1.setValue(0, 0, u8"な");
	tbl1.setValue(1, 1, u8"な");
	tbl1.setValue(2, 2, u8"め");
	tbl1.setValue(3, 0, u8"よ");
	tbl1.setValue(3, 1, u8"こ");
	tbl1.setValue(0, 2, 10);
	tbl1.setValue(4, 0, 3.14);
	std::cout << tbl1 << std::endl;
	STable tbl2;
	tbl2.addColumn(scolumn(NUMBER_COLUMN, "A"));
	tbl2.addColumn(scolumn(NUMBER_COLUMN, "B"));
	tbl2.addColumn(scolumn(TEXT_COLUMN, "C"));
	tbl2.addColumn(scolumn(DATE_COLUMN, "D"));
	tbl2.addRow({ 100,0.1111,u8"あいう",SDate() });
	std::cout << tbl2 << std::endl;


	std::cout << String("*") * 50 << NEW_LINE << std::endl;
}
void test::SNodeTest() {

}
void test::SVoidTest() {

}
void test::SIOTest() {

}
void test::SFileTest() {

}
void test::SNetTest() {

	SNetWork net;




}
void test::SImageTest() {

}
void test::SFigureTest() {

}