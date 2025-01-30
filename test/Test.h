#ifndef SLIB_TEST_H
#define SLIB_TEST_H

#include "sobj.h"
using namespace slib;

#define PRINT_TITLE(X) SPrint(S("=")*30,sstr::bfill(X,' ',20),S("=")*30);
#define PRINT_VALUE(X,Y) SPrint(S(X), " = ", Y);
#define PRINT_RESULT(F,R) SPrint(S(F), " => ", R);
#define BROKEN_LINE SPrint(S("- ")*40);
#define SINGLE_LINE SPrint(S("-")*80);
#define DOUBLE_LINE SPrint(S("=")*80);

#ifndef BASIC_TEST
#ifdef TEST_ALL
#define BASIC_TEST 1
#endif
#endif
#ifndef MATH_TEST
#ifdef TEST_ALL
#define MATH_TEST 1
#endif
#endif
#ifndef OBJ_TEST
#ifdef TEST_ALL
#define OBJ_TEST 1
#endif
#endif
#ifndef UTIL_TEST
#ifdef TEST_ALL
#define UTIL_TEST 1
#endif
#endif
#ifndef MEDIA_TEST
#ifdef TEST_ALL
#define MEDIA_TEST 1
#endif
#endif

namespace test {
	extern void RangeTest();
	extern void AreaTest();
	extern void ZoneTest();
	extern void MemoryTest();
	extern void PtrTest();
	extern void ArrayTest();
	extern void RegionTest();
	extern void ListTest();
	extern void MapTest();
	extern void SetTest();
	extern void CharTest();
	extern void StringTest();
	extern void TimeTest();
	extern void NodeTest();
	extern void ExceptionTest();
}

namespace test {
	extern void SObjTest();
	extern void SNumberTest();
	extern void SStringTest();
	extern void SDateTest();
	extern void SDataTest();
	extern void SArrayTest();
	extern void SDictTest();
	extern void STextTest();
	extern void SFuncTest();
	extern void STableTest();
	extern void SNodeTest();
	extern void SVoidTest();
	extern void SIOTest();
	extern void SFileTest();
	extern void SNetTest();
	extern void SImageTest();
	extern void SFigureTest();
	//extern void SMovieTest();


}
namespace test {
	extern void SJsonTest();
	extern void SXmlTest();
	extern void SDocumentTest();
	extern void SDBTest();
	extern void SCodeTest();
	extern void SThreadTest();
}


using namespace slib::smath;
namespace test {
	extern void CalcTest();
	extern void RandomTest();

	extern void FracTest();
	extern void ComplexTest();
	extern void LATest();
	extern void GeomTest();
	extern void StatTest();

}



namespace test {
	extern void SPlugInAppTest();
	extern void SCUIAppTest();
}

#include "sbioinfo.h"
using namespace slib::sbio;
namespace test {
	extern void SeqTest();
	extern void SeqIOTest();
	extern void AnnotTest();
	extern void BamTest();
}


namespace test {
	extern void SNodeJSAppTest();
}


#endif