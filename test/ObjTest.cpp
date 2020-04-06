#include "Test.h"
#include "sobj.h"

using namespace slib;

#ifdef SOBJECT_TEST
inline int func1(int i, double d, const char* s) {
	std::cout << i << " " << d << " " << s <<" " << std::flush;
	return 0;
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

	sobj fn = SFunction<int(int, double, const char*)>(func1);

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

	std::cout << fn.func<int(int, double, const char*)>()(1, 2.0, "abc") << std::endl;
}
#endif
#ifdef SNUMBER_TEST
void SNumberTest() {
	SNumber num1 = 100; //integer:整数
	std::cout << "num1:" << num1 << std::endl;
	SNumber num2 = 3.0; //real:実数
	std::cout << "num2:" << num2 << std::endl;
	SNumber num3 = sfrac("1/2"); //fraction:分数
	std::cout << "num3:" << num3 << std::endl;
	SNumber num4 = scomp("1+1i"); //complex:複素数
	std::cout << "num4:" << num4 << std::endl;

	SNumber num5 = num1 + num1; // addition:足し算
	std::cout << "num5:" << num5 << std::endl;
	SNumber num6 = num2 - num3; // subtraction:引き算
	std::cout << "num6:" << num6 << std::endl;
	SNumber num7 = num1 * num2; // :掛け算
	std::cout << "num7:" << num7 << std::endl;
	SNumber num8 = num2 / num3; // :割り算
	std::cout << "num8:" << num8 << std::endl;
	SNumber num9 = num1 % num2; // :剰余
	std::cout << "num9:" << num9 << std::endl;
	SNumber num10 = -num1; // :正負の反転
	std::cout << "num10:" << num10 << std::endl;
	++num1; //Increment:１つ増える
	std::cout << "num1:" << num1 << std::endl;
	--num10; //Decrement:１つ減る
	std::cout << "num10:" << num10 << std::endl;
}
#endif
#ifdef SSTRING_TEST
void SStringTest() {
	SString str1 = "abc";
	SString str2 = "123";
	SString str3 = u8"いろはにほへと";
	SString str4 = u8"ちりぬるを";

	std::cout << "str1 upper:" << str1.transformed(String::TO_UPPER) << std::endl; // :大文字に変換
	std::cout << "str2 wide:" << str2.transformed(String::TO_WIDE) << std::endl; // :全角に変換
	std::cout << "str3+str4:" << str3 + str4 << std::endl; // :文字列の合成１
	std::cout << "str1+int:" << str1 + 10 << std::endl; // :文字列の合成２
	std::cout << "str2+num:" << str2 + 0.5 << std::endl; // :文字列の合成３
	std::cout << "str3 x 2:" << str3 * 2 << std::endl; // :文字列の繰り返し
	std::cout << "str4 count:" << str4.charCount() << std::endl;
	std::cout << "str1[1]:" << String(str1[0]) << std::endl;
	std::cout << "str4[2]:" << str4.charAt(2) << std::endl;
	str3.charAt(0) = u8"あ";
	std::cout << "str3:" << str3 << std::endl;
}
#endif
#ifdef SDATE_TEST
extern void SDateTest() {
	SDate date1;
	std::cout << "date1:" << date1 << std::endl;
	date1 += Time("1y");
	std::cout << "date1:" << date1 << std::endl;

	SDate date2(SDate::YMDHMS);
	std::cout << "date2:" << date2 << std::endl;
	SDate date3(SDate::YMDHMS_J);
	std::cout << "date3:" << date3 << std::endl;
}
#endif
#ifdef SDATA_TEST
void SDataTest() {
	SData dat1;
}
#endif
#ifdef SARRAY_TEST
void SArrayTest() {
	SArray array1 = { 100, "abc", true, SDate(), V({u8"いろは", 2.0 }) };
	std::cout << "array1:" << array1 << std::endl;
	SArray array2 = iarray(10);
	std::cout << "array2:" << array2 << std::endl;
	SArray array3 = String("abc,de,f,ghi,jklm").split(",");
	std::cout << "array3:" << array3 << std::endl;
}
#endif
#ifdef SDICT_TEST
void SDictTest() {
	SDictionary dict1 = { kv("int", 100), kv("real",0.5), kv("str",u8"じゅげむ"), kv("array", V({ 100, "abc", true, SDate(), V({u8"いろは", 2.0 })})) };
	SDictionary dict2 = String(u8"id=1;name=Suzuki;value=80").parse(";", "=");
	std::cout << "id:" << dict2["id"] << NEW_LINE << "name:" << dict2["name"] << NEW_LINE << "value:" << dict2["value"] << std::endl;
}
#endif