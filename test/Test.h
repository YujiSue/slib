#ifndef SLIB_TEST_H
#define SLIB_TEST_H

#include "sobj.h"
#include "sapp.h"
#include "sapp/snodeapp.h"

#define MATH_TEST
#ifndef MATH_TEST
#define CALC_TEST
#define FRAC_TEST
#define COMP_TEST
#define LA_TEST
#define GEOM_TEST
#define STAT_TEST
#endif
#define BASIC_TEST
#ifndef BASIC_TEST
#define RANGE_TEST
#define AREA_TEST
#define ZONE_TEST
#define MEM_TEST
#define PTR_TEST
#define ARRAY_TEST
#define REGION_TEST
#define LIST_TEST
#define MAP_TEST
#define SET_TEST
#define CHAR_TEST
#define STRING_TEST
#define TIME_TEST
#define NODE_TEST
#define EXCEPTION_TEST
#endif

#define SOBJ_TEST
#ifndef SOBJ_TEST
#define SOBJECT_TEST
#define SNUMBER_TEST
#define SSTRING_TEST
#define SDATE_TEST
#define SDATA_TEST
#define SARRAY_TEST
#define SDICT_TEST
#define SFUNC_TEST
#define STEXT_TEST
#define STABLE_TEST
#define SIO_TEST
#define SNET_TEST
#endif
#define SUTIL_TEST
#ifndef SUTIL_TEST
#define CODE_TEST
#define DB_TEST
#define THREAD_TEST
#define PROCESS_TEST
#define JSON_TEST
#define XML_TEST
#endif


#define APP_TEST



//#define CV_TEST


//#define SBIOINFO_TEST
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
        extern void CalcTest();
        extern void FracTest();
        extern void ComplexTest();
        extern void LinearAlgebraTest();
        extern void GeometryTest();
        extern void StatisticTest();

}


namespace test {
        extern void SJsonTest();
        extern void SXmlTest();
        extern void SDocumentTest();
        extern void SDBTest();
        extern void SCodeTest();
        extern void SThreadTest();
}

namespace test{
        //extern void SBSeqTest();
}
namespace test {
        extern void SPlugInAppTest();
        extern void SCUIAppTest();
}

namespace test {
        extern void SNodeJSAppTest();
}

#endif