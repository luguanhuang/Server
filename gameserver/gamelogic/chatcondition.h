#ifndef __CHATCONDITION_H__
#define __CHATCONDITION_H__

struct ChatConditionParam
{
	UINT64 roleid;
	UINT64 destid;
	UINT32 level;
	UINT32 length;
	UINT32 cooldown;

	ChatConditionParam()
	{
		roleid = 0;
		destid = 0;
		level = 0;
		length = 0;
		cooldown = 0;
	}
};

class ChatCondition
{
public:
	virtual ~ChatCondition() {}
	virtual UINT32 Check(ChatConditionParam& param) = 0;
};

class LevelChatCondition : public ChatCondition
{
public:
	LevelChatCondition(UINT32 level) {m_level = level;}
	virtual UINT32 Check(ChatConditionParam& param);
	
private:
	UINT32 m_level;
};

class LengthChatCondition : public ChatCondition
{
public:
	LengthChatCondition(UINT32 length) {m_length = length;}
	virtual UINT32 Check(ChatConditionParam& param);

private:
	UINT32 m_length;
};

class TimeChatCondition : public ChatCondition
{
public:
	TimeChatCondition(UINT32 time) {m_time = time;}
	virtual UINT32 Check(ChatConditionParam& param);

private:
	UINT32 m_time;
	std::unordered_map<UINT64, time_t> m_roleid2time;
};

class BlackChatCondition : public ChatCondition
{
public:
	BlackChatCondition() {}
	virtual UINT32 Check(ChatConditionParam& param);
};

#endif