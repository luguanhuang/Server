#ifndef __TEAMMEMBER_H__
#define __TEAMMEMBER_H__

class TeamCopyMember
{
public:
	TeamCopyMember()
	{
	}
	~TeamCopyMember()
	{
	}

	void LoadM(const KKSG::TeamSynMember& data);

	inline UINT64 RoleID() const { return m_data.roleid(); }
	inline UINT32 ServerID() const { return m_data.serverid(); }

	bool IsHelper() const;

	inline KKSG::TeamMemberType GetMemberType() const { return m_data.memtype(); }

	bool IsRobot() const;

	void InfoPrint();

private:

	KKSG::TeamSynMember m_data;
};

class TeamEnterData
{
public:
	TeamEnterData();
	TeamEnterData(KKSG::TeamMemberType memtype);
	TeamEnterData(bool gotoNext, KKSG::TeamMemberType memtype);
	~TeamEnterData();

	void Reset();

	bool IsGotoNext();
	bool IsHelper();
	bool IsUseTicket();

	static TeamEnterData GlobalTeamEnterData;

private:

	bool m_gotoNext;		
	KKSG::TeamMemberType m_memtype;
};


#endif // __TEAMMEMBER_H__