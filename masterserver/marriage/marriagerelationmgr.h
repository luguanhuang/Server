#ifndef __MARRIAGERELATIONMGR_H__
#define __MARRIAGERELATIONMGR_H__

class Marriage;
class CRole;

#define FREE_DIVORCE_TIME GetGlobalConfig().MarriageFreeDivorceDay*86400

struct DelayMarriageProcessInfo
{
	DelayMarriageProcessInfo()
	{
		toGs = true;
		roleID = 0;
		type = KKSG::WeddingType_Normal;
	}
	bool toGs;
	UINT64 roleID;
	KKSG::WeddingType type;
};

class MarriageApplyOne
{
public:
	MarriageApplyOne(Marriage* oneSide);
	UINT32 IsValid();

private:
	UINT32 IsLevelValid();
	UINT32 IsPosValid();

private:
	Marriage* m_pOneSide;
};

class MarriageCondition
{
public:

	MarriageCondition(Marriage* src, Marriage* dest);
	UINT32 IsValid();

private:
	Marriage* m_pSrc;
	Marriage* m_pDest;
};


class MarriageRelationMgr
{
public:
	MarriageRelationMgr();
	~MarriageRelationMgr();
	DECLARE_SINGLETON(MarriageRelationMgr)

public:
	bool Init();
	void Uninit();

	//check team and map and friend degree, check level and marriage
	UINT32 PreApplyRelation(CRole* role, UINT64& destRoleID);

	UINT32 ApplyRelation(CRole* role, UINT64 destRoleID, KKSG::WeddingType type);

	UINT32 PreAgreeApplyRelation(CRole* role, UINT64 srcRoleID, KKSG::WeddingType& type);
	UINT32 AgreeApplyRelation(CRole* role, UINT64 srcRoleID);

	UINT32 HandleRelationOp(CRole* role, const KKSG::MarriageRelationOpArg& roArg, DelayMarriageProcessInfo& delayInfo);

	UINT32 DelayProcess(CRole* role, UINT64 srcRoleID, UINT64 destRoleID, const KKSG::MarriageRelationOpArg& roArg, KKSG::MarriageRelationOpRes& roRes);

	void DoTxLog(UINT64 roleid1, UINT64 roleid2, UINT32 op, UINT32 type, UINT64 weddingid=0);
private:

	UINT32 BuildRelation(Marriage* src, Marriage* dest, UINT32 type);
	UINT32 RefuseApplyRelation(CRole* role, UINT64 srcRoleID);

	UINT32 PreApplyDivorce(CRole* role, bool& toGs);
	UINT32 ApplyDivorce(CRole* role);
	UINT32 ApplyDivorceCancel(CRole* role);
	UINT32 AutoDivorce(Marriage* src, Marriage* dest);

	//int GetOppositePos(int pos);

private:
	bool _CalRelationApplyPos(Marriage* src, Marriage* dest, int pos);

};

#endif // 
