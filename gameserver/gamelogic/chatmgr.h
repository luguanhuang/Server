#ifndef __CHATMGR_H__
#define __CHATMGR_H__

#include "pch.h"
#include "table/ChatTable.h"

namespace KKSG
{
	class ChatInfo;
	class ChatRes;
}
class ChatHandler;
class Role;
class ChatMgr
{
	ChatMgr();
	~ChatMgr();
	DECLARE_SINGLETON(ChatMgr);
public:
	bool Init();
	void Uninit();

	//UINT32 HandleChatInfo(Role* role, UINT32 channel, const KKSG::ChatInfo* chatinfo, KKSG::ChatRes* res 
	//	, const std::vector<UINT64>* destroleids = NULL);

	//检查条件;
	UINT32 CheckCondition(Role* role, UINT32 channel);

	//发送世界聊天剩余次数;
	void SendWorldChannelLeftTimesNotify(Role* pRole);

	///> 聊天是否合法, 返回错误码
	KKSG::ErrorCode ChatLegal(Role* pRole, const KKSG::ChatInfo& chatinfo, KKSG::ChatCanSendRes& res);

	///>获取剩余世界聊天次数
	UINT32 GetLeftWorldChatTimes(Role* pRole);

	void FillSourceData(KKSG::ChatSource* source);

private:
	void ParseCondition(UINT32 channel, ChatTable::RowData* rowdata, ChatHandler* handler);

private:
	ChatTable m_table;
	std::map<UINT32, ChatHandler*> m_handles;
	UINT32 m_dwLastUpTime;
};

#endif