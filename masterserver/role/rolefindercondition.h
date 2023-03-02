#ifndef __ROLE_FINDERCONDITION_H__
#define __ROLE_FINDERCONDITION_H__

class CRole;

class IRoleFinderCondition
{
	virtual bool operator()(CRole* role) = 0;

public:
	std::vector<CRole*>& GetFoundRoles(){return m_foundRoles;}

protected:
	std::vector<CRole*> m_foundRoles;
};

class RoleSceneFindCondition : public IRoleFinderCondition
{
public:
	RoleSceneFindCondition(std::vector<UINT32>& vec);

	virtual bool operator()(CRole* role);

private:
	std::vector<UINT32> m_sceneIDVec;
};

#endif