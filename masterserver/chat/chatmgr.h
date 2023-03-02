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

	///>������Ϣ--����ʱ����������Ϣ���͸��û�
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
	///>�������컺��
	std::list<KKSG::ChatInfo> m_worldChat;
	///>�������컺�棺key-����ID value-����������Ϣ�б�
	std::unordered_map<UINT64, std::list<KKSG::ChatInfo>> m_mapGuildChat;
    ///>Ⱥ�����컺�棺key-Ⱥ��ID value-Ⱥ��������Ϣ�б�
    std::unordered_map<UINT64, std::list<KKSG::ChatInfo>> m_mapGroupChat;
	///>ս�����컺��: key-ս��ID value-�����б�
	std::unordered_map<UINT64, std::list<KKSG::ChatInfo>> m_mapPartnerChat;
	///>˽�Ļ��棬key-��ɫID value-��ɫ������Ϣ�б�
	std::unordered_map<UINT64, std::list<KKSG::ChatInfo>> m_mapPrivateChat;
	std::unordered_map<UINT64, std::list<PrivateList>> m_mapPrivateList; //list�е�roleidΨһ������ʱ��˳��

	bool m_isChangedWorld;
	UniqueQueue<UINT64> m_isChangedGuild;
    UniqueQueue<UINT64> m_isChangedGroup;
	UniqueQueue<UINT64> m_isChangedRole;
	UniqueQueue<UINT64> m_isChangedPrivateList;
	UniqueQueue<UINT64> m_isChangedPartner;


	int m_saveToDBCount;
};

#endif