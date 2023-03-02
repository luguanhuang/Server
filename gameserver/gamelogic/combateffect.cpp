#include "pch.h"
#include "bag.h"
#include "combateffect.h"
#include "../table/XSkillEffectMgr.h"
#include "skilldefine.h"
#include "unit/role.h"
#include "globalconfig.h"
#include "scene/scene.h"
#include "unit/rolemanager.h"

typedef std::vector<EffectDataParams::TypeData> EffectTypeDataList;

CombatEffectHelper::CombatEffectHelper()
{
	Set(NULL, NULL, NULL);
}

CombatEffectHelper::CombatEffectHelper(Unit* pDefaultUnit, UINT64 casterID, BuffTable::RowData* pBuffInfo)
{
	Unit* pUnit = pDefaultUnit;

	if(casterID != 0 && pDefaultUnit != NULL && casterID != pDefaultUnit->GetID())
	{
		pUnit = Unit::FindUnit(casterID);
	}

	if (pUnit == NULL || pUnit->GetCurrScene() == NULL
		|| GlobalConfig::GetInstance().ArtifactDisableSceneTypes.count(pUnit->GetCurrScene()->GetSceneType()) != 0)
		SetFromBuff(NULL, pBuffInfo, NULL);
	else if (!pUnit->IsRole())
	{
		Unit* pHost =  pUnit->GetFinalHostUnit();
		if (pHost != NULL && pHost->IsRole())
			SetFromBuff(static_cast<Role*>(pHost)->Get<Bag>(), pBuffInfo, pUnit->GetCurrScene());
		else
			SetFromBuff(NULL, pBuffInfo, NULL);
	}
	else
	{
		Role* pRole = static_cast<Role*>(pUnit);
		SetFromBuff(pRole->Get<Bag>(), pBuffInfo, pUnit->GetCurrScene());
	}
}

CombatEffectHelper::CombatEffectHelper(Unit* pUnit, UINT32 skillHash)
{
	Set(pUnit, skillHash);
}

void CombatEffectHelper::SetFromBuff(Bag* pBag, BuffTable::RowData* pBuffInfo, Scene* pScene)
{
	m_pBuffInfo = pBuffInfo;
	m_pScene = pScene;
	m_CacheType = CET_INVALID;
	m_CacheList = NULL;
	m_SkillHash = 0;
	m_pEffectData = pBag == NULL ? NULL : pBag->GetBuffEffectData(pBuffInfo->BuffID);

	m_Template2BuffTable.clear();
}
void CombatEffectHelper::Set(Unit* pUnit, UINT32 skillHash)
{
	if (pUnit == NULL || (pUnit->GetCurrScene() == NULL && pUnit->GetEnterSceneType() == 0))
	{
		Set(NULL, skillHash, 0);
		return;
	}
	else
	{
		UINT32 sceneType = pUnit->GetEnterSceneType();
		if (pUnit->GetCurrScene() != NULL)
			sceneType = pUnit->GetCurrScene()->GetSceneType();
		if(GlobalConfig::GetInstance().ArtifactDisableSceneTypes.count(sceneType) != 0)
		{
			Set(NULL, skillHash, 0);
			return;
		}
	}
	
	if (!pUnit->IsRole())
	{
		Unit* pHost =  pUnit->GetFinalHostUnit();
		if (pHost != NULL && pHost->IsRole())
			Set(static_cast<Role*>(pHost)->Get<Bag>(), skillHash, pUnit->GetSkillCasterTypeID());
		else
			Set(NULL, skillHash, pUnit->GetSkillCasterTypeID());
	}
	else
	{
		Role* pRole = static_cast<Role*>(pUnit);
		Set(pRole->Get<Bag>(), skillHash, pUnit->GetSkillCasterTypeID());
	}
}

void CombatEffectHelper::Set(Bag* pBag, const string& skillName, UINT32 casterTypeID)
{
	Set(pBag, XCommon::XHash(skillName.c_str()), casterTypeID);
}

void CombatEffectHelper::Set(Bag* pBag, UINT32 skillHash, UINT32 casterTypeID)
{
	m_pBuffInfo = NULL;
	m_pScene = NULL;
	m_CacheType = CET_INVALID;
	m_CacheList = NULL;
	m_SkillHash = skillHash;
	m_ActualSkillHash = skillHash;
	m_pEffectData = pBag == NULL ? NULL : pBag->GetSkillEffectData(skillHash);
	if (m_pEffectData == NULL && pBag != NULL)
	{
		UINT32 preSkillHash = XSkillEffectMgr::Instance()->GetPreSkill(skillHash, casterTypeID);
		if (preSkillHash != 0)
		{
			m_pEffectData = pBag->GetSkillEffectData(preSkillHash);
			m_ActualSkillHash = preSkillHash;
		}
	}

	m_Template2BuffTable.clear();
}

static const EffectTypeDataList* GetTypeDataList(CombatEffectType type, CombatEffectHelper* helper)
{
	if (helper->GetCacheQueryType() == type)
		return (const EffectTypeDataList*)helper->GetCacheQueryTypeList();

	if (helper->GetEffectData() == NULL)
		return NULL;

	auto it = helper->GetEffectData()->type2DataMap.find(type);
	if (it == helper->GetEffectData()->type2DataMap.end())
		return NULL;

	helper->SetCacheQuery(type, &(it->second));
	return &(it->second);
}

const BuffTable::RowData* CombatEffectHelper::GetTemplateBuffTable(UINT32 templateBuffID)
{
	auto it = m_Template2BuffTable.find(templateBuffID);
	if (it != m_Template2BuffTable.end())
		return it->second;

	BuffTable::RowData* pRowData = XBuffTemplateManager::Instance()->GetBuffInfo(templateBuffID, 1, m_pScene);
	m_Template2BuffTable[templateBuffID] = pRowData;
	return pRowData;
}

bool CombatEffectHelper::HasEffect(CombatEffectType type)
{
	return GetTypeDataList(type, this) != NULL;
}

int CombatEffectHelper::GetSum(CombatEffectType type)
{
	const EffectTypeDataList* dataList = GetTypeDataList(type, this);
	if (dataList == NULL)
		return 0;

	int sum = 0;
	for (auto it = dataList->begin(); it != dataList->end(); ++it)
	{
		for (auto it2 = it->params.begin(); it2 != it->params.end(); ++it2)
		{
			sum += *it2;
		}
	}
	return sum;
}

template <typename T, unsigned N, const Sequence<T, N>& (*TMethod)(const BuffTable::RowData* )>
bool GetSequenceSum(CombatEffectHelper* helper, CombatEffectType type, UINT32 replaceIndex, double ratio, std::vector<Sequence<T, N> >& vecOut)
{
	if (replaceIndex >= N)
		return false;

	const EffectTypeDataList* dataList = GetTypeDataList(type, helper);
	if (dataList == NULL)
		return false;

	for (auto it = dataList->begin(); it != dataList->end(); ++it)
	{
		const BuffTable::RowData* pBuffData = it->templatebuffID == 0 ? helper->GetBuffData() : helper->GetTemplateBuffTable(it->templatebuffID);
		if (pBuffData == NULL)
			continue;

		if (it->params.size() == 0)
		{
			continue;
		}

		Sequence<T, N> oriData = TMethod(pBuffData);
		oriData[replaceIndex] = (T)(it->params[0] * ratio);
		vecOut.push_back(oriData);
	}

	return true;
}

template <typename T, unsigned N, const std::vector<Sequence<T, N>>& (*TMethod)(const BuffTable::RowData* )>
bool GetVecSequenceSum(CombatEffectHelper* helper, CombatEffectType type, UINT32 replaceIndex, double ratio, std::vector<Sequence<T, N> >& vecOut)
{
	if (replaceIndex >= N)
		return false;

	const EffectTypeDataList* dataList = GetTypeDataList(type, helper);
	if (dataList == NULL)
		return false;

	for (auto it = dataList->begin(); it != dataList->end(); ++it)
	{
		const BuffTable::RowData* pBuffData = it->templatebuffID == 0 ? helper->GetBuffData() : helper->GetTemplateBuffTable(it->templatebuffID);
		if (pBuffData == NULL)
			continue;

		const std::vector<Sequence<T, N>>& originDatas = TMethod(pBuffData);
		if (it->params.size() != 0 && it->params.size() != originDatas.size())
		{
			continue;
		}
		for (int i = 0, max = it->params.size(); i < max; ++i)
		{
			auto data = originDatas[i];
			data[replaceIndex] = (T)(it->params[i] * ratio);
			vecOut.push_back(data);
		}
	}

	return true;
}

template <typename T, unsigned N, const std::vector<Sequence<string, N>>& (*TMethod)(const BuffTable::RowData* )>
bool GetVecSequenceSumFromVecString(CombatEffectHelper* helper, CombatEffectType type, UINT32 hashIndex, UINT32 replaceIndex, std::vector<Sequence<T, N> >& vecOut)
{
	if (replaceIndex >= N)
		return false;

	const EffectTypeDataList* dataList = GetTypeDataList(type, helper);
	if (dataList == NULL)
		return false;

	for (auto it = dataList->begin(); it != dataList->end(); ++it)
	{
		const BuffTable::RowData* pBuffData = it->templatebuffID == 0 ? helper->GetBuffData() : helper->GetTemplateBuffTable(it->templatebuffID);
		if (pBuffData == NULL)
			continue;

		const std::vector<Sequence<string, N>>& originDatas = TMethod(pBuffData);
		if (it->params.size() != 0 && it->params.size() != originDatas.size())
		{
			continue;
		}
		for (int i = 0, max = it->params.size(); i < max; ++i)
		{
			auto originData = originDatas[i];
			Sequence<T, N> newData;
			for (int j = 0; j < N; ++j)
			{
				if(j == hashIndex)
					newData[j] = (T)XCommon::XHash(originData[j].c_str());
				else if(j == replaceIndex)
					newData[j] = (T)it->params[i];
				else
					newData[j] = SafeConvert<T>(originData[j]);
			}
			vecOut.push_back(newData);
		}
	}

	return true;
}

template <typename T, unsigned N>
bool _Compare(const Sequence<T, N>& left, const Sequence<T, N>& right, int exclusiveIndex)
{
	for (int i = 0; i < N; ++i)
	{
		if (i == exclusiveIndex)
			continue;
		if (left[i] != right[i])
			return false;
	}

	return true;
}

template <typename T, unsigned N>
void CombatEffectHelper::Merge( std::vector<Sequence<T, N> >& datas, int exclusiveIndex)
{
	if (datas.size() == 0)
		return;

	auto finalIt = datas.begin();
	auto curIt = datas.begin();
	++curIt;

	Sequence<T, N> startData;
	Sequence<T, N> curData;
	for (; finalIt != datas.end() && curIt != datas.end(); ++curIt)
	{
		curData = *curIt;
		auto mergeIt = datas.end();
		for (auto j = datas.begin();; ++j)
		{
			startData = *j;
			if (_Compare<T, N>(startData, curData, exclusiveIndex))
			{
				mergeIt = j;
				break;
			}

			if (j == finalIt)
				break;
		}

		if (mergeIt != datas.end())
		{
			(*mergeIt)[exclusiveIndex] += curData[exclusiveIndex];
			//startData[exclusiveIndex] += curData[exclusiveIndex];
			//datas[mergeIndex] = startData;
		}
		else
		{
			++finalIt;
			if (finalIt < curIt)
				*finalIt = curData;
		}
	}

	datas.erase(++finalIt, datas.end());
}

const Sequence<float, 2>& _GetBuffHPData(const BuffTable::RowData* pBuffData)
{
	return pBuffData->BuffHP;
}

bool CombatEffectHelper::GetBuffHP(std::vector<Sequence<float, 2>>& vecOut)
{
	return GetSequenceSum<float, 2, _GetBuffHPData>(this, CET_Buff_HP, 1, 1, vecOut);
}

const std::vector<Sequence<float, 3>>& _GetBuffChangeAttributeData(const BuffTable::RowData* pBuffData)
{
	return pBuffData->BuffChangeAttribute;
}

bool CombatEffectHelper::GetBuffChangeAttribute(std::vector<Sequence<float, 3>>& vecOut)
{
	bool bRes = GetVecSequenceSum<float, 3, _GetBuffChangeAttributeData>(this, CET_Buff_ChangeAttribute, 1, 0.001, vecOut);
	Merge<float, 3>(vecOut, 1);
	return bRes;
}

bool CombatEffectHelper::GetSkillDamage(float& ratio)
{
	ratio = 0.0f;
	int res = GetSum(CET_Skill_Damage);
	if (res == 0)
		return false;

	ratio = res * 0.001f;
	return true;
}

bool CombatEffectHelper::GetSkillCD(float& fOut)
{
	int res = GetSum(CET_Skill_CD);
	if (res == 0)
		return false;

	fOut = res * 0.001f;
	if (fOut > 1.0f)
		fOut = 1.0f;
	fOut = -fOut;
	return true;
}

float CombatEffectHelper::GetBuffAuraRadius()
{
	return GetSum(CET_Buff_AuraRadius) * 0.001f;
}

float CombatEffectHelper::GetBuffTriggerRate()
{
	return GetSum(CET_Buff_TriggerRate) * 0.001f;
}

bool CombatEffectHelper::GetSkillAddBuff(std::vector<BuffDesc>& vecBuffs, const SkillFlags* flags)
{
	const EffectTypeDataList* dataList = GetTypeDataList(CET_Skill_AddBuff, this);
	if (dataList == NULL)
		return false;

	for (auto it = dataList->begin(); it != dataList->end(); ++it)
	{
		for (auto it2 = it->constantParams.begin(); it2 != it->constantParams.end(); ++it2)
		{
			std::vector<string> res = Split(*it2, "^");
			if (res.size() >= 4 && flags != NULL && !flags->IsFlagSet(SafeConvert<UINT32>(res[3])))
				continue;
			if (res.size() == 0)
				continue;

			BuffDesc bd;
			bd.buffID = SafeConvert<int>(res[0]);
			bd.BuffLevel = res.size() >= 2 ? SafeConvert<int>(res[1]) : 1;
			bd.delayTime = res.size() >= 3 ? SafeConvert<float>(res[2]) : 0;
			bd.skillID = m_SkillHash;

			vecBuffs.push_back(bd);
		}
	}

	return true;
}

bool CombatEffectHelper::GetSkillAddMobBuff(uint skillLevel, std::vector<BuffDesc>& vecBuffs)
{
	const EffectTypeDataList* dataList = GetTypeDataList(CET_Skill_AddMobBuff, this);
	if (dataList == NULL)
		return false;

	for (auto it = dataList->begin(); it != dataList->end(); ++it)
	{
		for (auto it2 = it->constantParams.begin(); it2 != it->constantParams.end(); ++it2)
		{
			std::vector<string> res = Split(*it2, "^");

			if (res.size() == 0)
				continue;

			BuffDesc bd;
			bd.buffID = SafeConvert<int>(res[0]);
			bd.BuffLevel = res.size() >= 2 ? SafeConvert<int>(res[1]) : skillLevel;

			vecBuffs.push_back(bd);
		}
	}

	return true;
}

const std::vector<Sequence<string, 3>>& _GetBuffReduceSkillCDData(const BuffTable::RowData* pBuffData)
{
	return pBuffData->ReduceSkillCD;
}

bool CombatEffectHelper::GetBuffReduceSkillCD(std::vector<Sequence<UINT32, 3>>& vecOut)
{
	bool bRes = GetVecSequenceSumFromVecString<UINT32, 3, _GetBuffReduceSkillCDData>(this, CET_Buff_ReduceCD, 0, 1, vecOut);
	Merge<UINT32, 3>(vecOut, 1);
	return bRes;
}

const std::vector<Sequence<string, 2>>& _GetBuffChangeSkillDamageData(const BuffTable::RowData* pBuffData)
{
	return pBuffData->ChangeSkillDamage;
}

bool CombatEffectHelper::GetBuffChangeSkillDamage(std::vector<Sequence<UINT32, 2>>& vecOut)
{
	return GetVecSequenceSumFromVecString<UINT32, 2, _GetBuffChangeSkillDamageData>(this, CET_Buff_ChangeSkillDamage, 0, 1, vecOut);
}

bool CombatEffectHelper::GetBuffChangeDamage(double& castDamage, double&receiveDamage)
{
	castDamage = GetSum(CET_Buff_ChangeDamage_Cast) * 0.001l;
	receiveDamage = GetSum(CET_Buff_ChangeDamage_Receive) * 0.001l;
	return true;
}

const std::vector<Sequence<float, 3>>& _GetBuffDotData(const BuffTable::RowData* pBuffData)
{
	return pBuffData->BuffDOT;
}

const std::vector<Sequence<int, 2>>& _GetBuffDotFromCasterData(const BuffTable::RowData* pBuffData)
{
	return pBuffData->BuffDOTValueFromCaster;
}

bool CombatEffectHelper::GetBuffRegenerate(std::vector<Sequence<float, 3>>& vecDotOut, std::vector<Sequence<int, 2>>& vecFromCasterOut)
{
	bool bRes = true;
	bRes = bRes && GetVecSequenceSum<float, 3, _GetBuffDotData>(this, CET_Buff_DOTorHOT, 1, 0.001, vecDotOut);
	bRes = bRes && GetVecSequenceSum<int, 2, _GetBuffDotFromCasterData>(this, CET_Buff_DOTorHOT, 1, 0.001, vecFromCasterOut);

	return bRes;
}

