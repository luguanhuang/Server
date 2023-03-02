#ifndef __GMFBASE_H__
#define __GMFBASE_H__

#include "pb/enum.pb.h"

namespace KKSG
{
	class GmfHalfRoom;
	class GmfRoom;
	class GmfRoleDatas;
	class GuildDarenaUnit;
}

class Scene;

enum GMFHalfRoomType
{
	GMF_HRT_NONE = 0,

	GMF_HRT_ONE = 1,
	GMF_HRT_TWO = 2,
};

struct GuildMFRole
{
	GuildMFRole()
	{
		roleID = 0;
		state = KKSG::GUILD_MF_NONE;

		pkpoint = 0;
		profession = 1;
	}
	UINT64 roleID;
	KKSG::GuildMatchFightState state;

	std::string name;
	UINT32 pkpoint;
	int profession;
};

class GuildMFHalfRoom
{
public:
	GuildMFHalfRoom();

	inline UINT64 GetGuildID(){ return m_guildID; }
	inline void SetGuildID(UINT64 gid){ m_guildID = gid; }

	inline UINT32 GetPretigeRank(){ return m_prestigerank; }
	inline void SetPretigeRank(UINT32 rank){ m_prestigerank = rank; }

	GuildMFRole* GetGuildMFRoleByRoleID(UINT64 roleID);
	GuildMFRole* GetGulldMFRoleByIndex(UINT32 index);

	int GetRoleCount(){ return (int)(m_roles.size()); }

	UINT64 GetNextFightRoleID(Scene* pScene);

	bool HaveRoleID(UINT64 roleID);

	void AddRoleID(UINT64 roleID, const std::string& name, UINT32 pkpoint, int profession);
	void DelRoleID(UINT64 roleID);

	void FillHalfRoles(KKSG::GmfHalfRoles& data);

	inline std::vector<GuildMFRole>& GetRoles(){ return m_roles; }
	inline void Clear() { m_roles.clear(); }

	void TestPrint();

private:	

	UINT64 m_guildID;
	UINT32 m_prestigerank;
	std::vector<GuildMFRole> m_roles;
};

class Scene;
class GuildMFRoom
{
public:
	GuildMFRoom();

	void SaveData(KKSG::GmfRoom& data);
	void LoadData(const KKSG::GmfRoom& data);

	//void FillRolesByHalfRoomType(GMFHalfRoomType type, std::vector<GuildMFRole>& roles);

	GMFHalfRoomType GetHalfRoomTypeByRoleID(UINT64 roleid);
	GMFHalfRoomType GetHalfRoomTypeByGuildID(UINT64 guildid);
	UINT64 GetGuildIDByRoleID(UINT64 roleid);

	KKSG::GuildMatchFightState GetRoleMFState(UINT64 roleID);
	void SetRoleMFState(UINT64 roleID, KKSG::GuildMatchFightState state);

	UINT64 GetNextFightRoleID(Scene* pScene, GMFHalfRoomType type);

	void AddRoleID(UINT64 guildid, UINT64 newRoleID, const std::string& name, UINT32 pkpoint, int profession);
	void DelRoleID(UINT64 roleID);
	bool HaveRoleID(UINT64 roleID);

	void FillBaseData(KKSG::GmfRoleDatas& data);

	UINT64 GetGuildID(GMFHalfRoomType type);
	UINT32 GetPrestigeRank(GMFHalfRoomType type);
	UINT32 GetRoleCount(GMFHalfRoomType type);
	void GetAllRoles(std::vector<UINT64>& allroles);
	GMFHalfRoomType GetOpHalfRoomType(GMFHalfRoomType type);

	void Clear();

	void TestPrint();

private:

	GuildMFRole* GetGuildMFRole(UINT64 roleID);

	GuildMFHalfRoom m_halfRome11;
	GuildMFHalfRoom m_halfRome22;

	time_t m_createTime;

	UINT64 m_UID;
	UINT32 m_oWarType;
	UINT32 m_oBattleId;
};

#endif