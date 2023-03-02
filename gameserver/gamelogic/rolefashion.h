#ifndef __ROLEFASHION_H__
#define __ROLEFASHION_H__

#include "bag.h"
#include "cvsreader.h"
#include "itemconfig.h"
#include "table/FashionComposeTable.h"
#include "unit/roleserilizeinterface.h"
#include "fashionmgr.h"

class Role;
struct XItem;
class RoleFashion;
class CombatAttribute;

namespace KKSG
{
	class FashionData;
};

struct FashionInfo
{
	UINT64 uid;
	UINT32 itemID;
	UINT32 level;
	UINT32 timeleft;
	UINT32 timeend;
	UINT32 pos;
	std::vector<ItemChangeAttr> m_attrs;

	FashionInfo()
	{
		uid = 0;
		itemID = 0;
		level = 0;
		timeleft = (UINT32)(-1);
		timeend = (UINT32)(-1);
		pos = 0;
	}

	void ToThis(const KKSG::FashionData* data);
	void ToKKSG(KKSG::FashionData* data);
};

class RoleFashion : public RoleSerializeInterface
{
public:
	RoleFashion(Role *pRole);
	~RoleFashion();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

	void SetBagListener(BagListener* listener) { m_baglistener = listener; }
	void Changed() { m_isModifed.Set(); }

	void ApplyEffect(CombatAttribute *combatAttr);
	Role *GetRole() { return m_pRole; }

	FashionInfo* FindFashionByUId(UINT64 uid);
	FashionInfo* GetBodyFashion(UINT64 uid);
	FashionInfo* GetBodyFashionPos(UINT32 pos) { return m_FashionOnBody[pos]; };
	const FashionBrief GetFashion();
	const FashionBrief GetWeddingFashion();
	UINT64 AddNewFashion(UINT32 itemid, bool isnotice = true);
	bool DelFashion(UINT64 uid, UINT32 reason, UINT32 subreason, const std::string trastag="");

	void  GetDispplayFashion(std::set<UINT32>& fashions);
	UINT32 GetDispplayHairColor();
	UINT32 GetDispplaySpecialEffectsID();

	KKSG::ErrorCode Wear(UINT64 uid);
	KKSG::ErrorCode Takeoff(UINT64 uid);
	KKSG::ErrorCode Takeoff(FashionInfo* fashion, UINT32 pos);
	KKSG::ErrorCode Wear(std::vector<UINT64>& uids);
	KKSG::ErrorCode Takeoff(std::vector<UINT64>& uids);

	// 穿
	KKSG::ErrorCode DisplayFashionWear(UINT32 item_id);
	KKSG::ErrorCode DisplaySuitFashionWear(UINT32 suit_id);

	// 脱
	KKSG::ErrorCode DisplayFashionTakeOff(UINT32 item_id);
	KKSG::ErrorCode DisplaySuitFashionTakeOff(UINT32 suit_id);

	void GetSuitFashinonItemID(UINT32 suit_id,std::map<UINT32,UINT32>& items);

	int GetFashionNum();
	int GetFashionNumByItemId(UINT32 itemid);

	void UpdateAll(UINT32 nowtime, bool isnotify);
	bool UpdateOne(FashionInfo* fashion, UINT32 nowtime);

	void GetAllFashionItemID(std::set<UINT32>& fashion);
	void GetAllFashionInfo(std::vector<FashionInfo*>& fashion);

	void NotifyChangeSelf(UINT32 type, Role* role, FashionInfo* fashion, bool isoff);
	void NotifyChangeSelf(UINT32 type, Role* role, std::vector<FashionInfo*>& fashions, bool isoff);
	void NotifyChangeOther(Role* role, FashionInfo* fashion, bool isoff);
	void NotifyChangeOther(Role* role, std::vector<FashionInfo*>& fashions, bool isoff);


	KKSG::ErrorCode ActivationFashion(UINT64 uid);
	//	外显通知
	void NotifyDisplayFashion();

	UINT32 ActiveFashionCharm(UINT32 suit_id,KKSG::ActivateFashionRes &roRes);
	void GetDisplayFashion(KKSG::OutLookDisplayFashion *display_fashion);
	void GetWeddingDisplayFashion(KKSG::OutLookDisplayFashion* display_fashion);
	void AddOwnFashionID(UINT32 item_id);
	void AddEquipItem(UINT32 item_id);
	bool CheckEquipNeedForProfession(UINT32 item_id);
	void SetInitDisplayFashion();

	void CheckTakeOffFashion(UINT32 item_id);

	void ChangeProfession(UINT32 from, UINT32 to);

	bool HasFashion(UINT32 itemid);
	bool CheckSFashion();//检查是否有S级时装;
	void CheckASFashionSuit(bool &bHasA, bool &bHasS);//检查是否有整套A，S时装;

	void  ActivationHairColor(UINT32 hair_id,UINT32 hair_color_id,KKSG::ActivateHairColorRes &roRes);
	KKSG::ErrorCode UseHairColor(UINT32 hair_id,UINT32 hair_color_id);

	FashionInfo* GetFashionItemByUID(UINT64 uid);
	void DeleteFashionByUID(UINT64 uid);

	UINT32 GetFashionSynthesisFaildCount(const UINT32 fashion_id);
	void DeleteFashionByItemID(UINT32 item_id,UINT32 item_count);
	void ClearFashionSynthesisByID(UINT32 fashion_id);
	void AddFashionSynthesisByID(UINT32 fashion_id); 

	UINT32 GetFashionIBShopBuyTime();
	void SetFashionIBShopBuyTime(UINT32 buytime);

	UINT32 GetFashionComposeTime();
	void SetFashionComposeTime(UINT32 composetime);

	void ObtainSpecialEffectsID(UINT32 suit_it);
	void ChooseSpecialEffects(UINT32 suit_it,KKSG::ChooseSpecialEffectsRes& res);

	void AddComposeSuccessNum(UINT32 quality);
	void AddComposeFailedNum();

	UINT32 GetComposeNum();
	UINT32 GetComposeSuccessNum();
	UINT32 GetComposeFailedNum();
	UINT32 GetCompseSucessNumByQuality(UINT32 quality);
	// test
	void TestAddDebugSpecialEffectsID(UINT32 special_effects_id);

private:
	void OpenTimeLimit(FashionInfo* fashion);

	int  TakeComposeCost(FashionComposeTable::RowData *Row);

	bool ValidPos(UINT32 pos) { return pos < FashionCount; }

	void AddCollected(UINT32 itemid) { m_collected.insert(itemid); };

	void FashionTimeout(FashionInfo* fashion);

	void AddOwnFashions(UINT32 item_id);
	void AddFashionForCharm(UINT32 item_id);

	void UpdateDisplayFashion(std::vector<UINT32>& fashion_id);

	void NotifyAddItem(UINT32 item_id);
	void NotifyDelItem(UINT32 item_id);
	void DoTxLog(UINT32 suitid, UINT32 count);
	void DoHairTxLog(UINT32 hair_id,UINT32 hair_color_id);
	bool CheckFashionDisplaySystemOpen();

	void CheckHairColor();	
private:
	Role *m_pRole;
	Switch m_isModifed;
	BagListener* m_baglistener;

	bool first_open_;
	FashionBrief m_briefs;

	std::set<UINT32> m_collected;

	FashionInfo* m_FashionOnBody[FashionCount];
	std::map<UINT64, FashionInfo*> m_Fashions;

	std::unordered_map<UINT32,UINT32> display_fashion_;
	// 一旦拥有，以后就可以外显了
	std::vector<SuitDisplayStore> own_fashins_;
	// 
	std::unordered_set<UINT32> own_display_items_;

	std::unordered_map<UINT32,std::unordered_set<UINT32>> hair_color_list_;
	UINT32 hair_color_id_;

	std::unordered_map<UINT32,UINT32> fashionsynthersis_fail_info_;

	UINT32 fashion_ibshop_buy_time_;
	UINT32 fashion_compose_time_;

	UINT32 special_effects_suitid_;
	std::unordered_set<UINT32>  special_effects_suitid_list_;

	UINT32 compose_success_num_;
	UINT32 compose_failed_num_;
	std::unordered_map<int,UINT32> quality_compose_;
};

#endif // __ROLEFASHION_H__