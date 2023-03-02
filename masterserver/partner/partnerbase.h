#ifndef _PARTNER_INTERFACE_H_
#define _PARTNER_INTERFACE_H_

class Partner;
class PartnerBase
{
public:
	PartnerBase(Partner* partner): m_owner(partner), m_dirty(false) {}
	virtual ~PartnerBase() {}

	virtual bool LoadFromDB(const KKSG::PartnerSaveData& data) = 0;
	virtual void SaveToDB(KKSG::PartnerSaveData& data) = 0;
	virtual void OnDayPass() = 0;

	void SetDirty(bool dirty = true);
	inline bool IsDirty() { return m_dirty; }

protected:
	Partner* m_owner;
	bool m_dirty;
};

#endif