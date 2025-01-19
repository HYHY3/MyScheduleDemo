#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <map>
#include <string>
#include <unordered_set>
#include <shared_mutex>

/**
* @namespace MySchedule
* @brief a namespace to control a schedule feature.
*/
namespace MySchedule {
using ScheduleListType = std::map<std::string, std::unordered_set<std::string>>;	// key = datetime, value = a set of tasks without duplicate 

/**
* @class Schedule
* @brief a class for managing a schedule with datetime and task.
*
* the Schedule class allows to add schedule with datetime and task format and to search for schedule list
* in a given datetime range including both start and end datetime.
*/
class Schedule {
private:
	/**
	* @brief a map that stores schedule with datetime as the key and task as the value.
	*/
	ScheduleListType mScheduleList;

	/**
	* @brief a maximum number of schedule allowed to store.
	*/
	const size_t mMaxScheduleNumber;

	/**
	* @brief a current total number of schedule.
	*/
	size_t mCurrentScheduleNumber;

	/**
	* @brief a shared mutex for multi-thread to add a schedule and to search for the schedule list
	*/
	std::shared_timed_mutex mSharedMutex;

protected:
	/**
	* @brief the last error message of the process.
	*/
	std::string mLastErrorMsg;

public:
	/**
	* @brief construct a schedule object with a maximum number of schedule.
	*
	* @param[in] maxCount: the maximum number of schedule allowed to store.
	*/
	Schedule(size_t maxCount);

	/**
	* @brief destruct the schedule object.
	*/
	virtual ~Schedule();

	/**
	* @brief get the last error message of the process
	*
	* @return the last error message
	*/
	std::string getLastErrorMsg() const;

	/**
	* @brief add a schedule to the schedule list with thread-safe.
	*
	* @param[in] dateTime: the date and time of the schedule in YYYYMMDDhhmm format.
	* @param[in] task: the task or event to be scheduled.
	*
	* @return if a schedule is added successfully return true, otherwise return false.
	*/
	bool addSchedule(const std::string& dateTime, const std::string& task);

	/**
	* @brief get the range of the schedule list from start to end datetime with thread-safe.
	*
	* @param[in] start: the start datetime.
	* @param[in] end: the end datetime.
	*
	* @return a pair of const iterator to point to the range of schedule list 
	*/
	std::pair<ScheduleListType::const_iterator, ScheduleListType::const_iterator> searchSchedule(const std::string& startT, const std::string& endT);

	/**
	* @brief provide a default a way to validate the input datetime. override this function to validate the datetime as your own way.
	*
	* @param[in] dateTime: the input date and time
	* @return if the input datetime is correct return ture, otherwise return false.
	*/
	virtual bool isValidDateTime(const std::string& dateTime);
};

}	// namespace MySchedule

#endif // SCHEDULE_H


