#include "Schedule.h"
#include <iostream>

using namespace std;


int main(void) {
	//std::cout << "my console app is running..." << std::endl;

	MySchedule::Schedule myTodoList(5);

	{
		string date1 = "201510101212";
		string task = "寝る";
		myTodoList.addSchedule("199912200520", "こんにちは！1");
		myTodoList.addSchedule("200102181530", "こんばんは。２");
		myTodoList.addSchedule("201404051610", "ありがとうございます、ａ");
		myTodoList.addSchedule("202309010900", "疲れ様です？a");
		myTodoList.addSchedule(date1, task);

        std::cout << "ErrorMsg=" << myTodoList.getLastErrorMsg() << std::endl;
	}

	auto startTime = "199205150100";
	auto endTime = "202312310659";
	auto result = myTodoList.searchSchedule(startTime, endTime);
	std::cout << "ErrorMsg=" << myTodoList.getLastErrorMsg() << std::endl;

	auto startIndex = result.first;
	auto endIndex = result.second;
	
	while (startIndex != endIndex) {
		cout << startIndex->first;
		for (const auto& data : startIndex->second) {
			cout << "\n    " << data << " data.length =" << data.size();
		}
		cout << endl;
		++startIndex;
	}


	while (true);

	return 0;
}