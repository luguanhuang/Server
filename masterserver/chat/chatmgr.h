#ifndef __CHAT_MGR_H__
#define __CHAT_MGR_H__

#include "pb/project.pb.h"
#include "util/uniquequeue.h"

class Guild;
class Team;
class CRole;
class CChatMgr : public ITimer
{
	CChatMgr();
	~CChatMgr();
	DECLARE_SINGLETON(CChatMgr);
public:
	bool Init();
	void Uninit();

	void OnTimer(UINT32 dwID, UINT32 dwCount);
	
	void SaveAllToDB();

	void AddChat(UINT64 storekey, const KKSG::ChatInfo& chatinfo, bool isLoadInDB = false);	

	void SendUser(UINT64 roleID, const KKSG::ChatInfo& chatinfo, bool isPrivate);

	void SendGuild(Guild* pGuild, const KKSG::ChatInfo& chatinfo);

	void SendTeam(Team* team, const KKSG::ChatInfo& chatinfo);

	void SendWorld(const KKSG::ChatInfo& chatinfo);
	
	void SendGroup(const std::vector<UINT64>& roleList, const KKSG::ChatInfo& chatinfo);
    void SendGroupChat(const std::map<UINT64, UINT32>& roleList, const KKSG::ChatInfo& chatinfo);

	void SendPartner(const KKSG::ChatInfo& chatinfo);

	///>离线消息--上线时，将缓存消息推送给用户
	void OffLineChatNotify(CRole* pRole);

	void ClearPrivateList(UINT64 roleid, UINT64 otherid);
	void ClearPrivateListAll(UINT64 roleid);

	void OpenPrivateChat(UINT64 roleid, UINT64 otherid);

	void DeleteOneRoleAllMsg(UINT64 roleid);

	void FillSourceData(KKSG::ChatSource* source);

	void RemovePartnerChat(UINT64 id);

private:
	bool QueryChatRecordDB();
	bool QueryPrivateChatListDB();
	void AddPrivateChat(UINT64 roleid, const KKSG::ChatInfo& chatinfo, bool isLoadInDB);
	void UploadChat(UINT64 storekey, const KKSG::ChatInfo& chatinfo);

	void SaveWorldChatToDB();
	bool SaveGuildChatToDB(UINT32 maxCount);
    bool SaveGroupChatToDB(UINT32 maxCount);
	bool SavePartnerChatToDB(UINT32 maxCount);
	bool SavePrivateChatToDB(UINT32 maxCount);
	bool SavePrivateListToDB(UINT32 maxCount);

private:
	struct PrivateList
	{
		PrivateList(){}
		PrivateList(UINT64 _roleid, bool _hasOfflineChat, UINT32 _lastChatTime):roleid(_roleid),
			hasOfflineChat(_hasOfflineChat), lastChatTime(_lastChatTime){}
		UINT64 roleid;
		bool hasOfflineChat;
		UINT32 lastChatTime;
		bool operator == (const PrivateList& list) const
		{
			return roleid == list.roleid;
		}
	};

private:
	HTIMER m_timeHandler;
	///>世界聊天缓存
	std::list<KKSG::ChatInfo> m_worldChat;
	///>公会聊天缓存：key-公会ID value-公会聊天消息列表
	std::unordered_map<UINT64, std::list<KKSG::ChatInfo>> m_mapGuildChat;
    ///>群聊聊天缓存：key-群聊ID value-群聊聊天消息列表
    std::unordered_map<UINT64, std::list<KKSG::ChatInfo>> m_mapGroupChat;
	///>战友聊天缓存: key-战友ID value-聊天列表
	std::unordered_map<UINT64, std::list<KKSG::ChatInfo>> m_mapPartnerChat;
	///>私聊缓存，key-角色ID value-角色聊天消息列表
	std::unordered_map<UINT64, std::list<KKSG::ChatInfo>> m_mapPrivateChat;
	std::unordered_map<UINT64, std::list<PrivateList>> m_mapPrivateList; //list中的roleid唯一，且有时间顺序

	bool m_isChangedWorld;
	UniqueQueue<UINT64> m_isChangedGuild;
    UniqueQueue<UINT64> m_isChangedGroup;
	UniqueQueue<UINT64> m_isChangedRole;
	UniqueQueue<UINT64> m_isChangedPrivateList;
	UniqueQueue<UINT64> m_isChangedPartner;


	int m_saveToDBCount;
};

#endif