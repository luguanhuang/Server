#ifndef __ARENASTARDB_H__
#define __ARENASTARDB_H__

class ArenaStarRoleData;
class ArenaStarDbH
{
public:
	bool Read();
	void Update(UINT32 wirteNumLlimit);

	void AddChangeRole(UINT64 roleID);
	UINT32 WaitSize() { return m_waitRoles.size(); }

private:
	
	std::unordered_set<UINT64> m_waitRoles;//数据改变等待写数据库
};

#endif
