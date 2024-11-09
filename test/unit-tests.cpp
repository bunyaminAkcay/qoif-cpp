#include "../qoif_cpp.h"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <sys/types.h>

class Test{
    public:
        unsigned int totalTestCount = 0;
        unsigned int passedTestCount = 0;
        Test(){}

        void test(bool value, std::string message){
            if (value) {
                passedTestCount++;
            }
            else {
                std::cout << message << std::endl;
            }
            totalTestCount++;
        }

        void printResult(std::string testName){
            std::string resultString = "PASSED";
            if (passedTestCount != totalTestCount) {
                resultString = "FAILED";
            }
            std::cout << testName << "\t" << resultString << "\t" << passedTestCount << "/" << totalTestCount << std::endl;
        }
};

int bitsToInt(int b1, int b2, int b3, int b4, int b5, int b6, int b7, int b8){
    int value = b1 * 128 + b2 * 64 + b3 * 32 + b4 * 16 + b5 * 8 + b6 * 4 + b7 * 2 + b8;
    return value;
}

void test_check_QOI_OP_RUN(){

    Test test;

    //test1
    {
        Pixel p1{100, 100, 100};Pixel p2{100, 100, 100};
        bool res = QOIF::check_QOI_OP_RUN(p1, p2, 10);
        test.test(res, "Failed check_QOI_OP_RUN test1");
    }

    //test2
    {
        Pixel p1{100, 100, 100};Pixel p2{100, 100, 100};
        bool res = QOIF::check_QOI_OP_RUN(p1, p2, 63);
        test.test(!res, "Failed check_QOI_OP_RUN test2");
    }

    //test3
    {
        Pixel p1{100, 100, 100};Pixel p2{100, 100, 100};
        bool res = QOIF::check_QOI_OP_RUN(p1, p2, 62);
        test.test(res, "Failed check_QOI_OP_RUN test3");
    }

    //test4
    {
        Pixel p1{0, 100, 100};Pixel p2{100, 100, 100};
        bool res = QOIF::check_QOI_OP_RUN(p1, p2, 0);
        test.test(!res, "Failed check_QOI_OP_RUN test4");
    }

    //test5
    {
        Pixel p1{100, 100, 100};Pixel p2{100, 0, 100};
        bool res = QOIF::check_QOI_OP_RUN(p1, p2, 0);
        test.test(!res, "Failed check_QOI_OP_RUN test4");
    }

    //test6
    {
        Pixel p1{100, 100, 0};Pixel p2{100, 100, 100};
        bool res = QOIF::check_QOI_OP_RUN(p1, p2, 0);
        test.test(!res, "Failed check_QOI_OP_RUN test4");
    }
    

    test.printResult("check_QOI_OP_RUN");
}

void test_check_QOI_OP_INDEX(){
    
    Test test;

    Pixel array[64];

    //test1
    {
        Pixel p1{100, 100, 100};
        bool res = QOIF::check_QOI_OP_INDEX(p1, array);
        test.test(!res, "Failed check_QOI_OP_RUN test1");
    }

    //test2
    {
        Pixel pixel{100, 20, 10};
        int index = ( int(pixel.r) * 3 + int(pixel.g) * 5 + int(pixel.b) * 7 + int(pixel.a) * 11) % 64;
        array[index] = pixel;
        bool res = QOIF::check_QOI_OP_INDEX(pixel, array);
        test.test(res, "Failed check_QOI_OP_RUN test1");
    }
    test.printResult("check_QOI_OP_INDEX");
}

void test_check_QOI_OP_DIFF(){
    Test test;

    //test1
    {
        Pixel p1{100, 100, 100};Pixel p2{100, 100, 100};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(res, "Failed check_QOI_OP_RUN test1");
    }
    //test2
    {
        Pixel p1{100, 101, 100};Pixel p2{101, 100, 100};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(res, "Failed check_QOI_OP_RUN test2");
    }
    //test3
    {
        Pixel p1{100, 101, 102};Pixel p2{101, 100, 100};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(!res, "Failed check_QOI_OP_RUN test3");
    }
    //test4
    {
        Pixel p1{98, 101, 100};Pixel p2{100, 100, 100};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(res, "Failed check_QOI_OP_RUN test4");
    }
    //test5
    {
        Pixel p1{98, 97, 100};Pixel p2{100, 100, 100};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(!res, "Failed check_QOI_OP_RUN test5");
    }
    //test6
    {
        Pixel p1{98, 100, 255};Pixel p2{100, 100, 0};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(res, "Failed check_QOI_OP_RUN test6");
    }
    //test7
    {
        Pixel p1{98, 100, 255};Pixel p2{100, 100, 0};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(res, "Failed check_QOI_OP_RUN test7");
    }
    //test8
    {
        Pixel p1{98, 100, 0};Pixel p2{99, 100, 254};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(!res, "Failed check_QOI_OP_RUN test8");
    }
    //test9
    {
        Pixel p1{98, 100, 0};Pixel p2{101, 100, 253};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(!res, "Failed check_QOI_OP_RUN test9");
    }
    //test10
    {
        Pixel p1{255, 0, 0};Pixel p2{1, 255, 255};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(res, "Failed check_QOI_OP_RUN test10");
    }
    //test11
    {
        Pixel p1{127, 128, 10};Pixel p2{128, 126, 11};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(!res, "Failed check_QOI_OP_RUN test11");
    }
    //test12
    {
        Pixel p1{127, 128, 10};Pixel p2{128, 125, 11};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(!res, "Failed check_QOI_OP_RUN test12");
    }
    //test13
    {
        Pixel p1{255, 0, 1};Pixel p2{0, 253, 254};
        bool res = QOIF::check_QOI_OP_DIFF(p1, p2);
        test.test(!res, "Failed check_QOI_OP_RUN test13");
    }

    test.printResult("check_QOI_OP_DIFF");
}

void test_check_QOI_OP_LUMA(){
    Test test;

    //test1
    {
        Pixel p1{100, 100, 100};Pixel p2{65, 69, 64};
        bool res = QOIF::check_QOI_OP_LUMA(p1, p2);
        test.test(res, "Failed check_QOI_OP_LUMA test1");
    }
    //test2
    {
        Pixel p1{100, 100, 100};Pixel p2{103, 110, 119};
        bool res = QOIF::check_QOI_OP_LUMA(p1, p2);
        test.test(!res, "Failed check_QOI_OP_LUMA test2");
    }
    //test3
    {
        Pixel p1{100, 100, 100};Pixel p2{103, 110, 118};// -3 -(-10) = 7; -18-(-10) = -8 
        bool res = QOIF::check_QOI_OP_LUMA(p1, p2);
        test.test(res, "Failed check_QOI_OP_LUMA test3");
    }
    //test4
    {
        Pixel p1{100, 100, 100};Pixel p2{101, 110, 117};
        bool res = QOIF::check_QOI_OP_LUMA(p1, p2);
        test.test(!res, "Failed check_QOI_OP_LUMA test4");
    }
    //test5
    {
        Pixel p1{100, 100, 100};Pixel p2{102, 110, 118};// -18 -(-10) 
        bool res = QOIF::check_QOI_OP_LUMA(p1, p2);
        test.test(!res, "Failed check_QOI_OP_LUMA test5");
    }
    //test6
    {
        Pixel p1{100, 255, 0};Pixel p2{100, 0, 248};// 8 -(-1)
        bool res = QOIF::check_QOI_OP_LUMA(p1, p2); 
        test.test(!res, "Failed check_QOI_OP_LUMA test6");
    }
    //test7
    {
        Pixel p1{100, 255, 0};Pixel p2{100, 0, 247}; // b:  (0-247)-(255-0) => (9) - (-1) = 10
        bool res = QOIF::check_QOI_OP_LUMA(p1, p2);
        test.test(!res, "Failed check_QOI_OP_LUMA test7");
    }
    //test8
    {
        Pixel p1{100, 255, 0};Pixel p2{100, 0, 250};
        bool res = QOIF::check_QOI_OP_LUMA(p1, p2);
        test.test(res, "Failed check_QOI_OP_LUMA test8");
    }
    
    test.printResult("check_QOI_OP_LUMA");
}

void test_QOI_OP_RUN(){
    Test test;

    //test1
    {
        int run = 0;
        int res = QOIF::QOI_OP_RUN(run);
        test.test(res == bitsToInt(1,1,0,0,0,0,0,0), "Failed QOI_OP_RUN test1");
    }
    //test2
    {
        int run = 32;
        int res = QOIF::QOI_OP_RUN(run);
        test.test(res == bitsToInt(1,1,1,0,0,0,0,0), "Failed QOI_OP_RUN test2");
    }

    test.printResult("test_QOI_OP_RUN  ");
}

void test_QOI_OP_DIFF(){
    Test test;

    //test1
    {
        Pixel p1{100, 100, 100};Pixel p2{100, 100, 100};
        int res = QOIF::QOI_OP_DIFF(p1, p2);
        test.test(res == bitsToInt(0,1,1,0,1,0,1,0), "Failed QOI_OP_DIFF test1");
    }
    //test2
    {
        Pixel p1{101, 101, 101};Pixel p2{100, 100, 100};
        int res = QOIF::QOI_OP_DIFF(p1, p2);
        test.test(res == bitsToInt(0,1,1,1,1,1,1,1), "Failed QOI_OP_DIFF test2");
    }
    //test3
    {
        Pixel p1{101, 100, 101};Pixel p2{100, 102, 100};
        int res = QOIF::QOI_OP_DIFF(p1, p2);
        test.test(res == bitsToInt(0,1,1,1,0,0,1,1), "Failed QOI_OP_DIFF test3");
    }

    test.printResult("test_QOI_OP_DIFF");
}

void test_QOI_OP_LUMA(){

    Test test;

    //test1
    {
        Pixel p1{100, 100, 100};Pixel p2{132, 132, 132};
        uint8_t* res = QOIF::QOI_OP_LUMA(p1, p2);
        test.test(res[0] == bitsToInt(1,0,0,0,0,0,0,0) && res[1] == bitsToInt(1,0,0,0,1,0,0,0), "Failed QOI_OP_LUMA test1");
    }
    //test2
    {
        Pixel p1{117, 100, 100};Pixel p2{100, 90, 98};//dg = 10; dr = 117 -100 = 17; dr_dg = 17 - 10 = 7 => 7+8 = 15 => 1111
        uint8_t* res = QOIF::QOI_OP_LUMA(p1, p2);
        test.test(res[0] == bitsToInt(1,0,1,0,1,0,1,0) && res[1] == bitsToInt(1,1,1,1,0,0,0,0), "Failed QOI_OP_LUMA test2");
    }

    test.printResult("test_QOI_OP_LUMA");
}

void test_QOI_OP_RGB(){

    Test test;

    //test1
    {
        Pixel p1{255, 0, 100};
        uint8_t* res = QOIF::QOI_OP_RGB(p1);
        test.test(res[0] == bitsToInt(1,1,1,1,1,1,1,0) && res[1] == bitsToInt(1,1,1,1,1,1,1,1) && res[2] == bitsToInt(0,0,0,0,0,0,0,0) && res[3] == bitsToInt(0,1,1,0,0,1,0,0), "Failed QOI_OP_RGB test1");
    }

    test.printResult("test_QOI_OP_RGB  ");
}

void test_QOI_OP_RGBA(){

    Test test;

    //test1
    {
        Pixel p1{255, 0, 100, 32};
        uint8_t* res = QOIF::QOI_OP_RGBA(p1);
        test.test(res[0] == bitsToInt(1,1,1,1,1,1,1,0) && res[1] == bitsToInt(1,1,1,1,1,1,1,1) && res[2] == bitsToInt(0,0,0,0,0,0,0,0) && res[3] == bitsToInt(0,1,1,0,0,1,0,0) && res[4] == bitsToInt(0,0,1,0,0,0,0,0), "Failed QOI_OP_RGBA test1");
    }

    test.printResult("test_QOI_OP_RGBA  ");
}

int main(){
    test_check_QOI_OP_RUN();
    test_check_QOI_OP_INDEX();
    test_check_QOI_OP_DIFF();
    test_check_QOI_OP_LUMA();

    test_QOI_OP_RUN();
    //QOI_OP_INDEX test does not exist.
    test_QOI_OP_DIFF();
    test_QOI_OP_LUMA();
    test_QOI_OP_RGB();
    test_QOI_OP_RGBA();

    return 0;
}