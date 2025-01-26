#include "stdafx.h"
#include "CppUnitTest.h"
#include "../DemoSchedule/Schedule.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MySchedule;

namespace UnitTestSchedule
{

TEST_CLASS(TestScheduleClass)
{
public:

    TEST_METHOD(TestConstructor)
    {
        Schedule schedule(20);
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());
    }

    TEST_METHOD(TestisValidDateTimeOfaddSchedule)
    {
        Schedule schedule(20);

        // OK
        Assert::IsTrue(schedule.addSchedule("203908150746", u8"予定"));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // NG (length: not equal 12)
        Assert::IsFalse(schedule.addSchedule("20390815074625", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the length of input datetime is invalid!"), schedule.getLastErrorMsg());

        // NG (format: has alphabet)
        Assert::IsFalse(schedule.addSchedule("2039MMDD0746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the format of input datetime is invalid!"), schedule.getLastErrorMsg());

        // NG (year: less than 1900)
        Assert::IsFalse(schedule.addSchedule("189908150746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the year of input datetime is invalid!"), schedule.getLastErrorMsg());

        // NG (month: less than 1)
        Assert::IsFalse(schedule.addSchedule("203900150746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the month of input datetime is invalid!"), schedule.getLastErrorMsg());

        // NG (month: greater than 12)
        Assert::IsFalse(schedule.addSchedule("203913150746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the month of input datetime is invalid!"), schedule.getLastErrorMsg());

        // NG (day: less than 1)
        Assert::IsFalse(schedule.addSchedule("203908000746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the day of input datetime is invalid!"), schedule.getLastErrorMsg());

        // NG (day: greater than 31)
        Assert::IsFalse(schedule.addSchedule("203908320746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the day of input datetime is invalid!"), schedule.getLastErrorMsg());

        // NG (day: 04/31)
        Assert::IsFalse(schedule.addSchedule("203904310746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the day of input datetime is invalid!"), schedule.getLastErrorMsg());

        // OK (day: 04/30)
        Assert::IsTrue(schedule.addSchedule("203904300746", u8"予定"));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // NG (day: 06/31)
        Assert::IsFalse(schedule.addSchedule("203906310746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the day of input datetime is invalid!"), schedule.getLastErrorMsg());

        // OK (day: 06/30)
        Assert::IsTrue(schedule.addSchedule("203906300746", u8"予定"));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // NG (day: 09/31)
        Assert::IsFalse(schedule.addSchedule("203909310746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the day of input datetime is invalid!"), schedule.getLastErrorMsg());

        // OK (day: 09/30)
        Assert::IsTrue(schedule.addSchedule("203909300746", u8"予定"));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // NG (day: 11/31)
        Assert::IsFalse(schedule.addSchedule("203911310746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the day of input datetime is invalid!"), schedule.getLastErrorMsg());

        // OK (day: 11/30)
        Assert::IsTrue(schedule.addSchedule("203911300746", u8"予定"));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // NG (day: 02/30 of leap year)
        Assert::IsFalse(schedule.addSchedule("202402300746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the day of input datetime is invalid!"), schedule.getLastErrorMsg());

        // OK (day: 02/29 of leap year 2024)
        Assert::IsTrue(schedule.addSchedule("202402290746", u8"予定"));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // OK (day: 02/29 of leap year 2000)
        Assert::IsTrue(schedule.addSchedule("200002290746", u8"予定"));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // NG (day: 02/29 of non-leap year)
        Assert::IsFalse(schedule.addSchedule("202302290746", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the day of input datetime is invalid!"), schedule.getLastErrorMsg());

        // OK (day: 02/28 of non-leap year)
        Assert::IsTrue(schedule.addSchedule("202302280746", u8"予定"));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // NG (hour: less than 0)
        Assert::IsFalse(schedule.addSchedule("20390815-146", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the format of input datetime is invalid!"), schedule.getLastErrorMsg());

        // NG (hour: greater than 23)
        Assert::IsFalse(schedule.addSchedule("203908152446", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the hour of input datetime is invalid!"), schedule.getLastErrorMsg());

        // NG (minute: less than 0)
        Assert::IsFalse(schedule.addSchedule("2039081507-1", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the format of input datetime is invalid!"), schedule.getLastErrorMsg());

        // NG (minute: greater than 59)
        Assert::IsFalse(schedule.addSchedule("203908150760", u8"予定"));
        Assert::AreEqual(std::string("the input datetime is invalid!the minute of input datetime is invalid!"), schedule.getLastErrorMsg());
    }

    TEST_METHOD(TestextractTaskInfoOfaddSchedule)
    {
        Schedule schedule(20);

        // OK (onlyUTF-8全角：ひらがな、カタカナ、漢字、数字、常用句読点)
        std::string validTask = u8"タスク予定あり、１２時３０分。ｓｏ　ＦＡ？！．．．";
        Assert::IsTrue(schedule.addSchedule("203908150746", validTask));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // OK (onlyUTF-8全角：ひらがな、カタカナ、漢字、数字、常用句読点)
        const std::string constValidTask = u8"タスク予定あり、１２時３０分。ｓｏ　ＦＡ？！．．．";
        Assert::IsTrue(schedule.addSchedule("203908150746", constValidTask));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // OK (onlyUTF-8全角：ひらがな、カタカナ、漢字、数字、常用句読点)
        Assert::IsTrue(schedule.addSchedule("203908150746", u8"タスク予定あり、１２時３０分。ｓｏ　ＦＡ？！．．．"));
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // OK (extract 256 characters when exceeding max length)
        std::string inputTaskInfo;
        for (int i = 0; i < 300; ++i) {
            inputTaskInfo += u8"あ";
        }
        Assert::IsTrue(schedule.addSchedule("203908150746", inputTaskInfo)); // should truncate at 256 valid chars
        Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());

        // OK trimming (UTF-8全角半角：ひらがな、カタカナ、漢字、数字(半角)、常用句読点)
        Assert::IsTrue(schedule.addSchedule("203908150746", u8"タスク予定あり、１２時30分。ｓｏ　ＦＡ？！．．．"));
        Assert::AreEqual(std::string("only supporting Full-width characters with UTF-8 encoding!"), schedule.getLastErrorMsg());

        // OK trimming (UTF-8全角半角：ひらがな、カタカナ、漢字、数字、常用句読点(半角))
        Assert::IsTrue(schedule.addSchedule("203908150746", u8"タスク予定あり,１２時３０分。ｓｏ　ＦＡ？！．．．"));
        Assert::AreEqual(std::string("only supporting Full-width characters with UTF-8 encoding!"), schedule.getLastErrorMsg());

        // OK trimming (UTF-8全角半角：ひらがな、カタカナ(半角)、漢字、数字、常用句読点)
        Assert::IsTrue(schedule.addSchedule("203908150746", u8"タスク予定ｱﾘ、１２時３０分。ｓｏ　ＦＡ？！．．．"));
        Assert::AreEqual(std::string("only supporting Full-width characters with UTF-8 encoding!"), schedule.getLastErrorMsg());

        // OK trimming (UTF-8全角予想外記号：ひらがな、カタカナ、漢字、数字、常用句読点)
        Assert::IsTrue(schedule.addSchedule("203908150746", u8"タスク予定あり☒、１２時３０分。ｓｏ　ＦＡ？！．．．"));
        Assert::AreEqual(std::string("only supporting Full-width characters with UTF-8 encoding!"), schedule.getLastErrorMsg());

        // NG (ASCII)
        Assert::IsFalse(schedule.addSchedule("203908150746", "TaskInfo. !12?30...0,"));
        Assert::AreEqual(std::string("the task info is empty!only supporting Full-width characters with UTF-8 encoding!"), schedule.getLastErrorMsg());

        // NG (not UTF-8全角：ひらがな、カタカナ、漢字、数字、常用句読点)
        Assert::IsFalse(schedule.addSchedule("203908150746", "タスク予定あり、１２時３０分。ｓｏ　ＦＡ？！．．．"));
        Assert::AreEqual(std::string("the task info is empty!only supporting Full-width characters with UTF-8 encoding!"), schedule.getLastErrorMsg());
    }

    TEST_METHOD(TestMaxCountOfaddSchedule)
    {
        // total count = 4
        Schedule schedule(4);

        // OK (totalCount = 1, the same schedule)
        Assert::IsTrue(schedule.addSchedule("203908150746", u8"予定１"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());
        Assert::IsTrue(schedule.addSchedule("203908150746", u8"予定１"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());
        Assert::IsTrue(schedule.addSchedule("203908150746", u8"予定１"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());

        // OK (totalCount = 3, the same datetime but different task info)
        Assert::IsTrue(schedule.addSchedule("201905180647", u8"予定２"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());
        Assert::IsTrue(schedule.addSchedule("201905180647", u8"予定３"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());

        // OK (totalCount = 4)
        Assert::IsTrue(schedule.addSchedule("201411232136", u8"予定４"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());

        // NG (totalCount = 4)
        Assert::IsFalse(schedule.addSchedule("200907231645", u8"予定５"));
        Assert::AreEqual(std::string("the schedule list is full. can not add a new one!"), schedule.getLastErrorMsg());

        // NG (totalCount = 4)
        Assert::IsFalse(schedule.addSchedule("199706051852", u8"予定６"));
        Assert::AreEqual(std::string("the schedule list is full. can not add a new one!"), schedule.getLastErrorMsg());
    }

    TEST_METHOD(TestsearchSchedule)
    {
        Schedule schedule(6);

        // add 5 schedules
        Assert::IsTrue(schedule.addSchedule("200907231645", u8"予定１"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());
        Assert::IsTrue(schedule.addSchedule("201905180647", u8"予定２ー１"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());
        Assert::IsTrue(schedule.addSchedule("201905180647", u8"予定２－２"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());
        Assert::IsTrue(schedule.addSchedule("201905180647", u8"予定２－３"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());
        Assert::IsTrue(schedule.addSchedule("202208150746", u8"予定３"));
        Assert::AreEqual(std::string(), schedule.getLastErrorMsg());

        // Not found (invalid start datetime)
        {
            auto actualResult = schedule.searchSchedule("20231023121318", "202401010900");
            Assert::IsTrue(actualResult.first == actualResult.second);
            Assert::AreEqual(std::string("the start datetime is invalid!the length of input datetime is invalid!"), schedule.getLastErrorMsg());
        }

        // Not found (invalid end datetime)
        {
            auto actualResult = schedule.searchSchedule("202401010900", "20241215165035");
            Assert::IsTrue(actualResult.first == actualResult.second);
            Assert::AreEqual(std::string("the end datetime is invalid!the length of input datetime is invalid!"), schedule.getLastErrorMsg());
        }
        
        // Not found (invalid range datetime)
        {
            auto actualResult = schedule.searchSchedule("202401010900", "202310231213");
            Assert::IsTrue(actualResult.first == actualResult.second);
            Assert::AreEqual(std::string("the datetime range is invalid!"), schedule.getLastErrorMsg());
        }

        // Not found (no data to show)
        {
            auto actualResult = schedule.searchSchedule("200707231645", "200807231645");
            Assert::IsTrue(actualResult.first == actualResult.second);
            Assert::AreEqual(std::string("no schedule to show!"), schedule.getLastErrorMsg());
        }

        // Not found (no data to show)
        {
            auto actualResult = schedule.searchSchedule("202308150746", "202408150746");
            Assert::IsTrue(actualResult.first == actualResult.second);
            Assert::AreEqual(std::string("no schedule to show!"), schedule.getLastErrorMsg());
        }

        // Not found (no data to show)
        {
            auto actualResult = schedule.searchSchedule("201308150746", "201408150746");
            Assert::IsTrue(actualResult.first == actualResult.second);
            Assert::AreEqual(std::string("no schedule to show!"), schedule.getLastErrorMsg());
        }

        // found
        {
            auto actualResult = schedule.searchSchedule("200707231645", "201007231645");
            Assert::IsTrue(actualResult.first != actualResult.second);
            Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());
        }

        // found
        {
            auto actualResult = schedule.searchSchedule("202108150746", "202308150746");
            Assert::IsTrue(actualResult.first != actualResult.second);
            Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());
        }

        // found
        {
            auto actualResult = schedule.searchSchedule("201007231645", "202108150746");
            Assert::IsTrue(actualResult.first != actualResult.second);
            Assert::AreEqual(std::string(""), schedule.getLastErrorMsg());
        }
    }
};

}  // UnitTestSchedule