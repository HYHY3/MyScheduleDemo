#include "Schedule.h"
#include <iostream>
#include <Windows.h>

using namespace std;

static const pair<string, string> ScheduleInfo[] = {
    { "199912200520", u8"‚±‚ñ‚É‚¿‚Í‚Q" },
    { "200102181530", u8"ƒRƒ“Hƒoƒ“ƒn" },
    { "200102181530", u8"B‚RI—V‚Ô¸”s" },
    { "201404051610", u8"‚P‚`‚„C‚ ‚è‚ª‚Æ‚¤‚²‚´‚¢‚Ü‚·" },
    { "202309010900", u8"”æ‚ê—l‚Å‚·" },
};

int main(void) {
    //std::cout << "my console app is running..." << std::endl;

    // create a schedule list instance.
    MySchedule::Schedule myTodoList(sizeof(ScheduleInfo) / sizeof(pair<string, string>) + 1); // "2.lastErrorMsg=" will output something, if without +1

    { // add some schedule
        for (const auto&data : ScheduleInfo) {
            myTodoList.addSchedule(data.first, data.second);
        }
        cout << "1.lastErrorMsg=" << myTodoList.getLastErrorMsg() << endl;

        string date1 = "201510101212";
        string task = u8"Q‚é";
        myTodoList.addSchedule(date1, task);
        cout << "2.lastErrorMsg=" << myTodoList.getLastErrorMsg() << endl;
    }

    // search the result from start to end datetime
    auto startTime = "199205150100";
    auto endTime = "202312310659";
    auto result = myTodoList.searchSchedule(startTime, endTime);
    cout << "3.lastErrorMsg=" << myTodoList.getLastErrorMsg() << endl;

    // show the result into console window with UTF-8 style
    SetConsoleOutputCP(CP_UTF8);
    auto startIndex = result.first;
    auto endIndex = result.second;
    while (startIndex != endIndex) {
        cout << "date is " << startIndex->first << ", the task list:" << endl;
        for (const auto& data : startIndex->second) {
            cout << "  " << data << endl;
        }
        ++startIndex;
    }


    while (true);
    return 0;
}