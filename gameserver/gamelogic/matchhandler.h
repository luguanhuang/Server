#ifndef __MATCHHANDLER_H__
#define __MATCHHANDLER_H__

class Role;
class MHBase;

class MatchHandler
{
public:
	//krole = null 表示不需要填充数据
	KKSG::ErrorCode RoleCondition(Role* pRole, int type, KKSG::KMatchRole* krole = NULL);

private:
	MHBase* GetSubHandler(int type);
	static std::unordered_map<int, MHBase*> m_subHandler;

	friend class MHBase;
};

class MHBase
{
public:
	MHBase(int type);
	virtual ~MHBase() {}

	virtual KKSG::ErrorCode MatchCondition(Role* pRole);
	virtual KKSG::ErrorCode FillMatchData(Role* pRole, KKSG::KMatchRole* krole);
};

class MHHero : public MHBase
{
public:
	MHHero():MHBase(KKSG::KMT_HERO){}
	KKSG::ErrorCode FillMatchData(Role* pRole, KKSG::KMatchRole* krole);

	static MHHero GlobalMHHero;
};

class MHWeekendAct : public MHBase
{
public:
	MHWeekendAct():MHBase(KKSG::KMT_WEEKEND_ACT){}	
	KKSG::ErrorCode MatchCondition(Role* pRole);
	KKSG::ErrorCode FillMatchData(Role* pRole, KKSG::KMatchRole* krole);

	static MHWeekendAct GlobalMHWeekendAct;
};

class MHPkTwo : public MHBase
{
public:
	MHPkTwo():MHBase(KKSG::KMT_PKTWO){}
	KKSG::ErrorCode MatchCondition(Role* pRole);
	KKSG::ErrorCode FillMatchData(Role* pRole, KKSG::KMatchRole* krole);

	static MHPkTwo GlobalMHPkTwo;
};

class MHSurvive : public MHBase
{
public:
	MHSurvive():MHBase(KKSG::KMT_SKYCRAFT){}
	KKSG::ErrorCode MatchCondition(Role* pRole);
	KKSG::ErrorCode FillMatchData(Role* pRole, KKSG::KMatchRole* krole);

	static MHSurvive GlobalMHSurvive;
};

#endif