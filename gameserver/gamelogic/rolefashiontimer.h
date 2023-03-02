#ifndef __ROLEFASHIONTIMER_H__
#define __ROLEFASHIONTIMER_H__

class Role;
class RoleFashionTimer : public ITimer
{
	Role *m_pRole;
	HTIMER  m_timeToken;

public:
	RoleFashionTimer();
	~RoleFashionTimer();
	void Start();
	void Stop();
	void SetRole(Role *pRole) { m_pRole = pRole; }
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
};

#endif


