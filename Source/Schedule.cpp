#include "Schedule.h"
#include <algorithm>


namespace MySchedule {
/**
* @brief the length of date-time format is 12
*/
constexpr auto DATETIME_FORMAT_NUMBER = 12;

/**
* @brief the validity year: 1900 - present
*/
constexpr auto DATETIM_YEAR_BEGIN = 1900;

/**
* @brief the validity month: 1 - 12
*/
constexpr auto DATETIM_MONTH_BEGIN = 1;
constexpr auto DATETIM_MONTH_END = 12;

/**
* @brief the validity day: 1 - 31
*/
constexpr auto DATETIM_DAY_BEGIN = 1;
constexpr auto DATETIM_DAY_END = 31;

/**
* @brief the validity hour: 0 - 23
*/
constexpr auto DATETIM_HOUR_BEGIN = 0;
constexpr auto DATETIM_HOUR_END = 23;

/**
* @brief the validity minute: 0 - 59
*/
constexpr auto DATETIM_MINUTE_BEGIN = 0;
constexpr auto DATETIM_MINUTE_END = 59;

/**
* @brief the maximum character number of each task info is 256
*/
constexpr auto TASK_INFO_MAX_NUMBER = 256;

Schedule::Schedule(size_t maxCount) : mMaxScheduleNumber(maxCount), mCurrentScheduleNumber(0) {
	// To do
}

Schedule::~Schedule() {
	// To do
}

std::string Schedule::getLastErrorMsg() const {
	return mLastErrorMsg;
}

bool Schedule::isValidDateTime(const std::string& dateTime) {

	// check length
	if (dateTime.length() != DATETIME_FORMAT_NUMBER) {
		mLastErrorMsg = "the length of input datetime is invalid!";
		return false;
	}

	// check format
	if (!std::all_of(dateTime.begin(), dateTime.end(), ::isdigit)) {
		mLastErrorMsg = "the format of input datetime is invalid!";
		return false;
	}

	auto year = std::stoi(dateTime.substr(0, 4));		// offset = 0,  length = 4
	auto month = std::stoi(dateTime.substr(4, 2));		// offset = 4,  length = 2
	auto day = std::stoi(dateTime.substr(6, 2));		// offset = 6,  length = 2
	auto hour = std::stoi(dateTime.substr(8, 2));		// offset = 8,  length = 2
	auto minute = std::stoi(dateTime.substr(10, 2));	// offset = 10, length = 2

	// check year
	if (year < DATETIM_YEAR_BEGIN) {
		mLastErrorMsg = "the year of input datetime is invalid!";
		return false;
	}

	// check month
	if (month < DATETIM_MONTH_BEGIN || month > DATETIM_MONTH_END) {
		mLastErrorMsg = "the month of input datetime is invalid!";
		return false;
	}

	// check day
	if (day < DATETIM_DAY_BEGIN || day > DATETIM_DAY_END) {
		mLastErrorMsg = "the day of input datetime is invalid!";
		return false;
	}

	// check hour
	if (hour < DATETIM_HOUR_BEGIN || hour > DATETIM_HOUR_END) {
		mLastErrorMsg = "the hour of input datetime is invalid!";
		return false;
	}

	// check minute
	if (minute < DATETIM_MINUTE_BEGIN || minute > DATETIM_MINUTE_END) {
		mLastErrorMsg = "the minute of input datetime is invalid!";
		return false;
	}

	return true;
}

bool Schedule::addSchedule(const std::string& dateTime, const std::string& task) {
	// check input datetime
	if (!isValidDateTime(dateTime)) {
		mLastErrorMsg = "the input datetime is invalid!";
		return false;
	}

	// check input task
	if (task.length() > TASK_INFO_MAX_NUMBER) {
		mLastErrorMsg = "the input task is out of range!";
		return false;
	}

	// use exclusive lock to add a schedule
	std::unique_lock<decltype(mSharedMutex)> lock(mSharedMutex);

	// check storage of the schedule list
	if (mCurrentScheduleNumber >= mMaxScheduleNumber) {
		mLastErrorMsg = "the schedule list is full. can not add a new one!";
		return false;
	}

	// add a schedule with datetime as key and task as value
	if (mScheduleList[dateTime].find(task) == mScheduleList[dateTime].end()) {
		mScheduleList[dateTime].emplace(task);
		++mCurrentScheduleNumber;
	}
	return true;
}

std::pair<ScheduleListType::const_iterator, ScheduleListType::const_iterator> Schedule::searchSchedule(const std::string& startT, const std::string& endT) {
	// check start datetime
	if (!isValidDateTime(startT)) {
		mLastErrorMsg = "the start datetime is invalid!";
		return std::make_pair(mScheduleList.end(), mScheduleList.end());
	}

	// check end datetime
	if (!isValidDateTime(endT)) {
		mLastErrorMsg = "the end datetime is invalid!";
		return std::make_pair(mScheduleList.end(), mScheduleList.end());
	}

	// check datetime range
	if (startT > endT) {
		mLastErrorMsg = "the datetime range is invalid!";
		return std::make_pair(mScheduleList.end(), mScheduleList.end());
	}

	// use shared lock to search for schedule list
	std::shared_lock<decltype(mSharedMutex)> lock(mSharedMutex);

	// get the first schedule info
	ScheduleListType::iterator startSchedule = mScheduleList.lower_bound(startT);	// the index of first datetime >= startT
	if (startSchedule == mScheduleList.end()) {
		mLastErrorMsg = "no schedule to show!";
		return std::make_pair(mScheduleList.end(), mScheduleList.end());
	}
	// get the last schedule info
	ScheduleListType::iterator endSchedule = mScheduleList.upper_bound(endT);	// the index of first datetime > end
	return std::make_pair(startSchedule, endSchedule);
}

}	// namespace MySchedule