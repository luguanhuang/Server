#ifndef __SAVEROLEDATATIMER_H__
#define __SAVEROLEDATATIMER_H__

class Role;
class SaveRoleDataTimer : public ITimer
{
	Role *m_pRole;
	HTIMER  m_timeToken;

public:
	SaveRoleDataTimer();
	~SaveRoleDataTimer();
	void Start();
	void Stop();
	void SetRole(Role *pRole) { m_pRole = pRole; }
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
};

#endif // __SAVEROLEDATATIMER_H__


