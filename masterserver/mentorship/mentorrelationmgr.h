#ifndef __MENTORRELATIONMGR_H__
#define __MENTORRELATIONMGR_H__

#define RELATIONCOMPELETEMTIME GetGlobalConfig().MentorNormalCompleteDay*86400
#define RELATIONCOMPELETEMFORCETIME GetGlobalConfig().MentorForceCompleteDay*86400

class MentorShipOneSide;

class ApplyOneSideConditon
{
public:
	ApplyOneSideConditon(MentorShipOneSide* oneSide, int pos);
	bool IsValid();

private:
	bool IsLevelValid();
	bool IsPosValid();
	bool IsTimeValid();

private:
	MentorShipOneSide* m_pOneSide;
	int m_pos;
};

class ApplyConditon
{
public:

	ApplyConditon(MentorShipOneSide* src, MentorShipOneSide* dest, int pos);
	bool IsValid();

private:
	bool IsLevelValid();
	bool IsPosValid();
	bool IsTimeValid();

private:
	MentorShipOneSide* m_pSrc;
	MentorShipOneSide* m_pDest;
	int m_pos;
};


class MentorRelationMgr
{
public:
	MentorRelationMgr();
	~MentorRelationMgr();
	DECLARE_SINGLETON(MentorRelationMgr)

public:
	bool Init();
	void Uninit();

	KKSG::MentorApplyStatus GetRelationApplyStatus(MentorShipOneSide* src, MentorShipOneSide* dest);

	bool BuildRelation(MentorShipOneSide* src, MentorShipOneSide* dest, int pos);
	UINT32 ApplyRelation(MentorShipOneSide* src, MentorShipOneSide* dest, int pos);
	UINT32 ApplyBreak(MentorShipOneSide* src, MentorShipOneSide* dest);
	UINT32 ApplyBreakCancel(MentorShipOneSide* src, MentorShipOneSide* dest);
	UINT32 NormalComplete(MentorShipOneSide* src, MentorShipOneSide* dest);
	UINT32 ForceComplete(MentorShipOneSide* src, MentorShipOneSide* dest);
	UINT32 AutoBreak(MentorShipOneSide* src, MentorShipOneSide* dest);
	UINT32 ApplyInherit(MentorShipOneSide* src, MentorShipOneSide* dest);

	int GetOppositePos(int pos);

private:
	bool _CalRelationApplyPos(MentorShipOneSide* src, MentorShipOneSide* dest, int pos);
	UINT32 _Complete(MentorShipOneSide* src, MentorShipOneSide* dest, bool isForce);

};

#endif // 
