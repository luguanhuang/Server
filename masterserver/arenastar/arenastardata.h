#ifndef __ARENASTARDATA_H__
#define __ARENASTARDATA_H__

const UINT32 ArenaStarMaxRank = UINT32(-1);

//操作对象时必须将数据更到最新(季度)
class ASRoleData
{
public:
	ASRoleData(UINT64 roleID);//use for new data
	ASRoleData(const KKSG::ArenaStarRoleData& data);//use for db data
	~ASRoleData();

	inline UINT64 RoleID() const { return m_data.roleid(); }
	inline KKSG::ArenaStarRoleData& Data() { return m_data; }

    void OnSeaonEnd(KKSG::ArenaStarType type, UINT32 rank, UINT32 season);
    void CheckFillSeasonData(KKSG::ArenaStarType type, UINT32 season);//更新数据
    UINT32 GetMaxSeason(KKSG::ArenaStarType type);

	int GetNotice(KKSG::ArenaStarType type);
	void SetNotice(KKSG::ArenaStarType type, int notice);

	void FillHistoryData(KKSG::ArenaStarType type, KKSG::ArenaStarHistData& histdata);

	void Clear();
	void InfoPrint();

private:
	KKSG::ArenaStarAllData* GetAllData(KKSG::ArenaStarType type);
	void SetChangedFlag();

	KKSG::ArenaStarRoleData m_data;
};

#endif
