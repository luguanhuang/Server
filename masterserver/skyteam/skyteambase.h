#ifndef _SKY_TEAM_BASE_H_
#define _SKY_TEAM_BASE_H_

class SkyTeam;
class SkyTeamBase
{
public:
	SkyTeamBase(SkyTeam* owner): m_owner(owner), m_dirty(false) {}
	virtual ~SkyTeamBase() {}

	virtual bool LoadFromDB(const KKSG::SkyTeamSaveData& data) = 0;
	virtual void SaveToDB(KKSG::SkyTeamSaveData& data) = 0;
	virtual void OnDayPass() = 0;

	void SetDirty(bool dirty = true);
	inline bool IsDirty() { return m_dirty; }

protected:
	SkyTeam* m_owner;
	bool m_dirty;
};

#endif
