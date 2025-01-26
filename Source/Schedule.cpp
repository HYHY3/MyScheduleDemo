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
constexpr auto DATETIME_YEAR_BEGIN = 1900;

/**
* @brief the validity month: 1 - 12
*/
constexpr auto DATETIME_MONTH_BEGIN = 1;
constexpr auto DATETIME_MONTH_END = 12;

/**
* @brief the validity day: 1 - 31
*/
constexpr auto DATETIME_DAY_BEGIN = 1;
constexpr auto DATETIME_DAY_END = 31;

/**
* @brief the validity hour: 0 - 23
*/
constexpr auto DATETIME_HOUR_BEGIN = 0;
constexpr auto DATETIME_HOUR_END = 23;

/**
* @brief the validity minute: 0 - 59
*/
constexpr auto DATETIME_MINUTE_BEGIN = 0;
constexpr auto DATETIME_MINUTE_END = 59;

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
    // no error message
    mLastErrorMsg.clear();

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
    if (year < DATETIME_YEAR_BEGIN) {
        mLastErrorMsg = "the year of input datetime is invalid!";
        return false;
    }

    // check month
    if (month < DATETIME_MONTH_BEGIN || month > DATETIME_MONTH_END) {
        mLastErrorMsg = "the month of input datetime is invalid!";
        return false;
    }

    // check day
    if (day < DATETIME_DAY_BEGIN || day > DATETIME_DAY_END) {
        mLastErrorMsg = "the day of input datetime is invalid!";
        return false;
    }

    // check 30-day months
    if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
        if (day > 30) {
            mLastErrorMsg = "the day of input datetime is invalid!";
            return false;
        }
    }

    // check day of February
    if (month == 2) {
        if (((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))) {
            // leap year
            if (day > 29) {
                mLastErrorMsg = "the day of input datetime is invalid!";
                return false;
            }
        }
        else {
            // non-leap year
            if (day > 28) {
                mLastErrorMsg = "the day of input datetime is invalid!";
                return false;
            }
        }
    }

    // check hour
    if (hour < DATETIME_HOUR_BEGIN || hour > DATETIME_HOUR_END) {
        mLastErrorMsg = "the hour of input datetime is invalid!";
        return false;
    }

    // check minute
    if (minute < DATETIME_MINUTE_BEGIN || minute > DATETIME_MINUTE_END) {
        mLastErrorMsg = "the minute of input datetime is invalid!";
        return false;
    }

    return true;
}

std::string Schedule::extractTaskInfo(const std::string& task) {
    size_t curPos = 0;
    int validatedCount = 0;

    // no error message
    mLastErrorMsg.clear();

    // extract task info from the input data
    while ((validatedCount < TASK_INFO_MAX_NUMBER) && (curPos < task.length())) {
        // determine byte length of the first character
        unsigned char byte1 = static_cast<unsigned char>(task[curPos]);
        if ((byte1 & 0xF0) != 0xE0) {            
            mLastErrorMsg = "only supporting Full-width characters with UTF-8 encoding!";
            break;
        }

        // check continuation bytes of 3-byte UTF-8:https://en.wikipedia.org/wiki/UTF-8
        unsigned char byte2 = task[curPos + 1];
        unsigned char byte3 = task[curPos + 2];
        if (((byte2 & 0xC0) != 0x80) || ((byte3 & 0xC0) != 0x80)) {
            mLastErrorMsg = "only supporting Full-width characters with UTF-8 encoding!";
            break;
        }

        // 3-byte UTF-8  <=> code point
        uint32_t codePoint = ((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);

        // check if the code point is within Japanese full-width character ranges: http://www.rikai.com/library/kanjitables/kanji_codes.unicode.shtml
        if ((codePoint >= 0x3000 && codePoint <= 0x30FF) || // Full-width punctuation(3000 ` 303F) and Hiragana(3040 ` 309F) and Katakana(30A0 ` 30FF)
            (codePoint >= 0x4E00 && codePoint <= 0x9FFF) || // Common Kanji
            (codePoint >= 0xFF01 && codePoint <= 0xFF5E))   // Full-width alphanumeric and unctuation 
        {
            ++validatedCount;
            curPos += 3; // move to the next character in 3-byte sequences UTF-8
            continue;
        }
        mLastErrorMsg = "only supporting Full-width characters with UTF-8 encoding!";
        break;
    }

    return std::move(task.substr(0, curPos));
}

bool Schedule::addSchedule(const std::string& dateTime, const std::string& task) {
    // no error message
    mLastErrorMsg.clear();

    // check input datetime
    if (!isValidDateTime(dateTime)) {
        mLastErrorMsg = "the input datetime is invalid!" + mLastErrorMsg;
        return false;
    }

    // check input task
    std::string extractedTaskInfo = extractTaskInfo(task);
    if (extractedTaskInfo.empty()) {
        mLastErrorMsg = "the task info is empty!" + mLastErrorMsg;
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
    if (mScheduleList[dateTime].find(extractedTaskInfo) == mScheduleList[dateTime].end()) {
        mScheduleList[dateTime].emplace(extractedTaskInfo);
        ++mCurrentScheduleNumber;
    }

    return true;
}

std::pair<ScheduleListType::const_iterator, ScheduleListType::const_iterator> Schedule::searchSchedule(const std::string& startT, const std::string& endT) {
    // no error message
    mLastErrorMsg.clear();

    // check start datetime
    if (!isValidDateTime(startT)) {
        mLastErrorMsg = "the start datetime is invalid!" + mLastErrorMsg;
        return std::make_pair(mScheduleList.end(), mScheduleList.end());
    }

    // check end datetime
    if (!isValidDateTime(endT)) {
        mLastErrorMsg = "the end datetime is invalid!" + mLastErrorMsg;
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
    // get the last schedule info
    ScheduleListType::iterator endSchedule = mScheduleList.upper_bound(endT);	// the index of first datetime > end

    if (startSchedule == endSchedule) {
        mLastErrorMsg = "no schedule to show!";
    }

    return std::make_pair(startSchedule, endSchedule);
}

}	// namespace MySchedule