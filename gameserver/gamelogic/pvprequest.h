#ifndef __PVPREQUEST_H__
#define __PVPREQUEST_H__

class Role;
class PvpUnit;

class PvpRequest
{
public:
	PvpRequest(Role* pRole);

	int GetWeekReward();

private:
	Role* m_role;
};

#endif