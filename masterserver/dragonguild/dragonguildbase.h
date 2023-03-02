#ifndef _DRAGONGUILD_INTERFACE_H_
#define _DRAGONGUILD_INTERFACE_H_

class DragonGuild;
class DragonGuildBase
{
public:
	DragonGuildBase(DragonGuild* dragonguild): m_owner(dragonguild), m_dirty(false) {}
	virtual ~DragonGuildBase() {}

	virtual bool LoadFromDB(const KKSG::DragonGuildSaveData& data) = 0;
	virtual void SaveToDB(KKSG::DragonGuildSaveData& data) = 0;
	virtual void OnDayPass() = 0;

	void SetDirty(bool dirty = true);
	inline bool IsDirty() { return m_dirty; }

protected:
	DragonGuild* m_owner;
	bool m_dirty;
};

#endif