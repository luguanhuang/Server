#ifndef __ATLASCONFIGMGR_H__
#define __ATLASCONFIGMGR_H__


#include "table/CardsFireProperty.h"
#include "table/CardsGroup.h"
#include "table/CardsGroupList.h"
#include "table/CardsList.h"




class Role;
struct ItemChangeAttr; 
class CAtlasConfigMagr
{
	CAtlasConfigMagr();
	~CAtlasConfigMagr();
	DECLARE_SINGLETON(CAtlasConfigMagr);

public:
	bool Init();
	void Uninit();

public:
	bool IsGroupOpen(UINT32 groupId, UINT32 roleLvl);
	UINT32 TryCreateTeam(Role* pRole, UINT32 teamId, UINT32 finishid);
	void GetAllAttr(UINT32 profession, std::map<UINT32, UINT32>& allAttrMap, const std::map<UINT32, std::set<UINT32> >& m_groupMap, const std::map<UINT32, UINT32>& m_finishRcd);
	void GetTeamAttr(std::map<UINT32, UINT32>& allAttrMap, UINT32 teamId);
	void GetNumAttr(std::map<UINT32, UINT32>& allAttrMap, UINT32 num, UINT32 groupId);
	UINT32 GetGroupId(UINT32 teamId);
	void GetAddAttr(UINT32 profession, vector<Sequence<uint, 2>>& attrsVec, const std::set<UINT32>& m_groupSet);
	void GetAddPer(std::map<UINT32, UINT32>& allPerMap, UINT32 groupId, UINT32 num, UINT32 finishId);

	UINT32 NeedCardTimes(Role* pRole, UINT32 cardId, const std::set<UINT32>& hasTeamid);
	void GetQualitCards(UINT32 quailt, std::map<UINT32, UINT32>& cardsMap, UINT32 groupId);
	void GetGroupItem(UINT32 groupId, std::set<UINT32>& itemSet);
	UINT32 GetUpStarTimes(UINT32 groupId);
	bool CanUpStar(UINT32 groupId, UINT32 lvl, UINT32 finishId);

private:
	const std::vector<std::pair<UINT32, UINT32>>* _FindTeamNeedCards(UINT32 teamId, UINT32 finishid);
	void _GetPerAdd(UINT32 groupId, UINT32 num, UINT32 finishId, std::map<UINT32, UINT32>& addPerMap);
	void _GetAllPer(UINT32 groupId, UINT32 finishId, std::map<UINT32, UINT32>& addPerMap);
	void _GetGroupAttr(UINT32 profession, UINT32 groupId, UINT32 finishId, std::map<UINT32, UINT32>& allAttrMap);

private:
	CardsFireProperty m_cardsFire;
	CardsGroup m_cardsGroup;
	std::map<UINT32, std::vector<std::vector<std::pair<UINT32, UINT32>>>> m_teamid2Condition;
	CardsGroupList m_cardGroupList;
	CardsList m_cardList;
};





#endif
