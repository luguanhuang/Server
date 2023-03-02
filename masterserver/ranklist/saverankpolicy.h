#ifndef __SAVERANKPOLICY_H__
#define __SAVERANKPOLICY_H__

class SaveToDBPolicy
{
public:
	virtual ~SaveToDBPolicy() {}
	virtual bool IsTimeToSave() = 0;
};

class SaveToDBEveryPeriod : public SaveToDBPolicy
{
public:
	SaveToDBEveryPeriod();

	void SetSaveInterval(int time);

	virtual bool IsTimeToSave();

private:
	time_t m_tSaveInterval;
	time_t m_tLastSaveTime;
};

class SaveToDBAtSpecialTime : public SaveToDBPolicy
{
public:
	SaveToDBAtSpecialTime();

	void SetSaveTime(int hour, int min);

	virtual bool IsTimeToSave();

public:
	bool m_IsTodayUpdate;
	int m_hour;
	int m_minute;
};

#endif // __SAVERANKPOLICY_H__