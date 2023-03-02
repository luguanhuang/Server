#ifndef __MARRIAGE_H__
#define __MARRIAGE_H__

#include "role/switch.h"
#include "pb/enum.pb.h"
#include "partner/partnerliveness.h"

class MarriageMsg;
class CRole;

enum WeddingStatus
{
	WeddingStatus_Null,
	WeddingStatus_Holded,
	WeddingStatus_Max,
};

struct MarriageDivorceAppyInfo
{
	MarriageDivorceAppyInfo()
	{
		Reset();
	}
	void Reset()
	{
		m_isDivorceApplier = false;
		m_needNotifyDivorce = false;
		m_needNotifyDivorceCancel = false;
	}
	bool m_isDivorceApplier;
	bool m_needNotifyDivorce;
	bool m_needNotifyDivorceCancel;
};

struct MarriageRelation
{
	MarriageRelation();

	void Reset();

	void ConvertFromDb(const KKSG::OneMarriageRelation2DB& relation);
	void Convert2Db(KKSG::OneMarriageRelation2DB* relation);
	UINT32 GetDivorceLeftTime();

	bool m_takeWeddingCar;
	bool m_holdedWedding;
	UINT32 m_marriageTime;
	UINT32 m_divorceTime;
	UINT64 m_roleID;
	KKSG::WeddingType m_weddingType;
	KKSG::MarriagePos m_pos;
	KKSG::MarriageStatus m_status;
	MarriageDivorceAppyInfo m_applyDivorceInfo;
	UINT32 m_liveness;
	std::deque<PartnerLivenessRecord> m_livenessRecord;
	UINT32 m_chestTaked;
};

class Marriage
{
public:
	Marriage(UINT64 roleID);
	~Marriage();

	void Load(const KKSG::MarriageAllData2DB& data);
	void Save(KKSG::MarriageAllData2DB* data);
	void SetChanged();

	bool IsOpen();
	void DoOpen();
	void OnRoleLogin(CRole* role);
	void OnDayPass();
	void OnWeekPass();

	bool AddRelation(Marriage* destMarriage, UINT32 type, KKSG::MarriagePos pos);
	UINT32 FillMarrigeRelation2Client(KKSG::GetMarriageRelationRes& roRes);
	UINT32 ApplyDivorce(UINT64 applyID);
	UINT32 ApplyDivorceCancel(UINT64 applyID);
	UINT32 GetLeftDivorceTime();
	UINT32 Divorce();

	UINT32 CheckHoldWedding(KKSG::WeddingType& type);
	UINT32 CheckTakeWeddingCar();
	UINT32 SetTakeWeddingCar(bool taked = true);
	void SetHoldedWedding(bool holded = true);
	KKSG::WeddingType GetWeddingType();

	void SetNoticeDivorce(bool set = true);
	void SetNoticeDivorceCancel(bool set = true);

	UINT64 GetOwnerID(){return m_roleID;}
	UINT64 GetCoupleID();
	bool IsInMarriagePos();
	bool HasHistory();

	UINT32 GetMarriageType();

	MarriageMsg* GetMarriageMsg(){return m_pMarriageMsg;}

	void AddLivenessRecord(UINT64 roleId, UINT32 actId, UINT32 value);
	void FillLivenessRecord(KKSG::GetMarriageLivenessRes& res);
	UINT32 GetLiveNess() { return m_curRelation.m_liveness; }
	UINT32 GetTakedChest() { return m_curRelation.m_chestTaked; }
	UINT32 GetMarriageTime() { return m_curRelation.m_marriageTime; }
	bool IsChestTaked(UINT32 index);
	void SetChestTaked(UINT32 index);

	UINT32 GetCoupleOfflineTime();

	void NotifyLivenessHint();
	KKSG::MarriagePos GetMarriagePos(){return m_selfPos;}

	void CheckGetMarriageActivityReward(UINT32 endTime);

	UINT32 GetHisMarriageCount() { return m_hisRelation.size();}
private:
	void _NotifyDivorceOrCancel(CRole* role);
	KKSG::MarriageStatus _GetMarriageStatus2Client();
	void _OnAddRelation();
private:
	//save
	UINT64 m_roleID;
	UINT32 m_updateTime;
	KKSG::MarriagePos m_selfPos;
	MarriageRelation m_curRelation;
	std::list<MarriageRelation> m_hisRelation;
	UINT32 m_getActivityEndTime;

	MarriageMsg* m_pMarriageMsg;
};

#endif // 
