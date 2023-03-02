#ifndef __SPACTIVITYDATASMALL_H__
#define __SPACTIVITYDATASMALL_H__

#include "spactivitydatabase.h"

class Role;

class SpActOpenServerData : public SpActivityDataBase
{
public:
	SpActOpenServerData(Role* pRole, UINT32 actid);
	virtual ~SpActOpenServerData();

	virtual void Load(const KKSG::SpActivityOne& spActOneData);
	virtual void Save(KKSG::SpActivityOne& spActOneData);

	KKSG::ErrorCode GetBigPrize();

private:
	bool m_getBigPrize;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SpActBackFlowShopGood
{
	SpActBackFlowShopGood(bool _isBuy = false, UINT32 _lockTime = 0)
	{
		isBuy = _isBuy;
		lockTime = _lockTime;
	}
	bool isBuy;
	UINT32 lockTime;
};

class SpActBackFlowData : public SpActivityDataBase
{
public:
	SpActBackFlowData(Role* pRole, UINT32 actid);
	virtual ~SpActBackFlowData();

	virtual void Load(const KKSG::SpActivityOne& spActOneData);
	virtual void Save(KKSG::SpActivityOne& spActOneData);

	virtual void UpdatePerMinute();

	virtual void OnSpActivityOpen();
	virtual void OnSpActivityClose();

	virtual void IncreaseProgress(const SpActivityTaskConf* conf, UINT32 count);

	void SetInitData(UINT32 worldLevel, UINT32 lostDay);

	inline UINT32 GetWorldLevel() const { return m_worldLevel; }

	KKSG::ErrorCode GetTreasure(UINT32 pos);

	void FillTreasureData(KKSG::BackFlowActivityOperationRes& res);

	UINT32 GetPayGiftCount(const std::string& paramid);
	void UpdatePayGiftCount(const std::string& paramid);

	UINT32 GetShopLeftTime();

	void FillShopData(KKSG::BackFlowShopData& shopData);

	void UpdateShopFreshCount();

	KKSG::ErrorCode FreshShop();
	void DoFreshShop();

	KKSG::ErrorCode BuyGoodInShop(UINT32 goodid, UINT32 delayid, bool& needDelay);

	bool SetGoodBuySucceed(UINT32 goodid);

	inline bool IsFinishBackFlowScene() const { return m_isFinishBackFlowScene; }
	void SetIsFinishBackFlowScene();

private:
	UINT32 m_worldLevel;
	UINT32 m_lostDay;
	UINT32 m_point;
	std::vector<UINT32> m_alreadyGet;
	std::unordered_map<std::string, UINT32> m_PayGift;//Ã¿ÈÕÀñ°ü
	UINT32 m_shopUpdateTime;
	UINT32 m_freshCount;
	std::unordered_map<UINT32, SpActBackFlowShopGood> m_shopGoods;
	bool m_isFinishBackFlowScene;
	UINT32 m_lastSmallDragonFinishTime;
	UINT32 m_lastNestFinishTime;
	UINT32 m_nestFinishCount;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActArgentaData : public SpActivityDataBase
{
public:
	SpActArgentaData(Role* pRole, UINT32 actid);
	virtual ~SpActArgentaData();

	virtual void Load(const KKSG::SpActivityOne& spActOneData);
	virtual void Save(KKSG::SpActivityOne& spActOneData);

	virtual void UpdatePerMinute();
	virtual void OnFirstEnterScene();	

	void SetInitialData();

	inline UINT32 GetArgentaStartTime() const { return m_argentaStartTime; }
	inline UINT32 GetLevel() const { return m_level; }
	
	KKSG::ErrorCode GetDailyReward(UINT32 id);

	void FillInfo(KKSG::ArgentaActivityRes& res);

private:
	UINT32 m_argentaStartTime;
	UINT32 m_level;
	UINT32 m_lastUpdateTime;	
	std::vector<UINT32> m_getDailyRewards;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActAncientData : public SpActivityDataBase
{
public:
	SpActAncientData(Role* pRole, UINT32 actid);
	virtual ~SpActAncientData();

	virtual void Load(const KKSG::SpActivityOne& spActOneData);
	virtual void Save(KKSG::SpActivityOne& spActOneData);

	virtual void OnSpActivityOpen();

	UINT32 GetAward();
	void SetAward(UINT32 nAward);

private:
	UINT32 m_nAward;
};	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActThemeData : public SpActivityDataBase
{
public:
	SpActThemeData(Role* pRole, UINT32 actid);
	virtual ~SpActThemeData();
	virtual void Load(const KKSG::SpActivityOne& spActOneData);
	virtual void Save(KKSG::SpActivityOne& spActOneData);
	virtual void OnSpActivityOpen();

	bool IsFirstCompleteScene(UINT32 scenetype, UINT32 sceneid);
	void SetFirstCompleteScene(UINT32 scenetype, UINT32 sceneid);

	void SetHint(bool isHint);
	void NotifyClient();

private:
	std::map<UINT32, std::vector<UINT32>> m_mapFirstScene;
	bool m_isHint;
};


#endif