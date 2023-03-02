#include "pch.h"
#include "effectconfig.h"
#include "util/XCommon.h"
#include "effectdef.h"


EffectTableData::EffectTableData()
{

}

EffectTableData::~EffectTableData()
{

}

bool EffectTableData::HandlerTableData()
{
	if (!m_oTable.LoadFile("table/EffectTable.txt"))
	{
		LogWarn("Load EffectTable.txt failed");
		return false;
	}

	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); ++i)
	{
		m_effect2Conf[(*i)->EffectID].push_back(*i);

		if ((*i)->BuffID == 0 && (*i)->SkillScript.empty())
		{
			LogWarn("EffectID [%u], BuffID is zero and SkillScript is empty", (*i)->EffectID);
			return false;
		}
		if ((*i)->BuffID != 0 && !(*i)->SkillScript.empty())
		{
			LogWarn("EffectID [%u], it's wrong to fill in both of BuffID [%u] and SkillScript [%s]", (*i)->EffectID, (*i)->BuffID, (*i)->SkillScript.c_str());
			return false;
		}

		if ((*i)->BuffID != 0)
		{
			if (m_effectBuff2Conf.find(std::make_pair((*i)->EffectID, (*i)->BuffID)) != m_effectBuff2Conf.end())
			{
				LogWarn("EffectID [%u] and BuffID [%u] is duplicated key", (*i)->EffectID, (*i)->BuffID);
				return false;
			}
			m_effectBuff2Conf[std::make_pair((*i)->EffectID, (*i)->BuffID)] = *i;
		}
		else if (!(*i)->SkillScript.empty())
		{
			UINT32 skillID = XCommon::XHash((*i)->SkillScript.c_str());
			if (m_effectSkill2Conf.find(std::make_pair((*i)->EffectID, skillID)) != m_effectSkill2Conf.end())
			{
				LogWarn("EffectID [%u] and SkillScript [%s] is duplicated key", (*i)->EffectID, (*i)->SkillScript.c_str());
				return false;
			}
			m_effectSkill2Conf[std::make_pair((*i)->EffectID, skillID)] = *i;
		}

		for (auto j = (*i)->EffectParams.begin(); j != (*i)->EffectParams.end(); ++j)
		{
			if ((*j)[1] > (*j)[2])
			{
				LogWarn("EffectID [%u], BuffID [%u], SkillScript [%s], EffectParams random range error", (*i)->EffectID, (*i)->BuffID, (*i)->SkillScript.c_str());
				return false;
			}
		}
	}

	if (!m_randomTable.LoadFile("table/EffectRandomList.txt"))
	{
		LogWarn("Load EffectRandomList.txt failed");
		return false;
	}

	for (auto i = m_randomTable.Table.begin(); i != m_randomTable.Table.end(); ++i)
	{
		m_mapRandomConf[(*i)->EffectRandomID][(*i)->Profession].push_back(*i);
	}

	if (!m_typeTable.LoadFile("table/EffectTypeTable.txt"))
	{
		LogWarn("Load EffectTypeTable.txt failed");
		return false;
	}

	if (!m_desTable.LoadFile("table/EffectDesTable.txt"))
	{
		LogWarn("Load EffectDesTable.txt failed");
		return false;
	}
	for (auto i = m_desTable.Table.begin(); i != m_desTable.Table.end(); ++i)
	{
		if (m_typeTable.GetByEffectType((*i)->EffectType) == NULL)
		{
			LogWarn("EffectID [%u] EffectType [%u] not exist in EffectTypeTable", (*i)->EffectID, (*i)->EffectType);
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EffectConfig::EffectConfig()
{
}

EffectConfig::~EffectConfig()
{

}

bool EffectConfig::Init()
{
	return HandlerTableData();
}

void EffectConfig::Uninit()
{

}

bool EffectConfig::CheckFile()
{
	EffectTableData tmpData;
	return tmpData.HandlerTableData();
}

bool EffectConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	HandlerTableData();

	return true;
}

void EffectConfig::ClearFile()
{
	m_oTable.Clear();

	m_effect2Conf.clear();
	m_effectBuff2Conf.clear();
	m_effectSkill2Conf.clear();

	m_randomTable.Clear();
	m_mapRandomConf.clear();

	m_desTable.Clear();
}

const EffectConf* EffectConfig::GetBuffEffectConf(UINT32 effectID, UINT32 buffID)
{
	auto iter = m_effectBuff2Conf.find(std::make_pair(effectID, buffID));
	if (iter == m_effectBuff2Conf.end())
	{
		return NULL;
	}
	return iter->second;
}

const EffectConf* EffectConfig::GetSkillEffectConf(UINT32 effectID, UINT32 skillID)
{
	auto iter = m_effectSkill2Conf.find(std::make_pair(effectID, skillID));
	if (iter == m_effectSkill2Conf.end())
	{
		return NULL;
	}
	return iter->second;
}

const std::vector<EffectConf*>* EffectConfig::GetEffectConfByEffectID(UINT32 effectID)
{
	auto iter = m_effect2Conf.find(effectID);
	if (iter == m_effect2Conf.end())
	{
		return NULL;
	}
	return &iter->second;
}

bool EffectConfig::GetEffectData(UINT32 effectID, KKSG::EffectData& data)
{
	const std::vector<EffectConf*>* confVec = EffectConfig::Instance()->GetEffectConfByEffectID(effectID);
	if (confVec == NULL)
	{
		return false;
	}
	data.Clear();
	data.set_effectid(effectID);
	for (auto j = confVec->begin(); j != confVec->end(); j++)
	{
		KKSG::EffectMultiParams* multiParams = data.add_multiparams();
		if ((*j)->BuffID != 0)
		{
			multiParams->set_idtype(EffectIDType_Buff);
			multiParams->set_id((*j)->BuffID);
		}
		else
		{
			multiParams->set_idtype(EffectIDType_Skill);
			multiParams->set_id(XCommon::XHash((*j)->SkillScript.c_str()));
		}
		for (auto k = (*j)->EffectParams.begin(); k != (*j)->EffectParams.end(); k++)
		{
			UINT32 distance = XRandom::randInt(0, (*k)[2] - (*k)[1] + 1);
			multiParams->add_effectparams((*k)[1] + distance);
		}
	}
	return true;
}

void EffectConfig::UpdateEffectData(std::vector<KKSG::EffectData>& vecEffectData)
{
	std::vector<std::vector<KKSG::EffectData>*> vecAllEffectData;
	vecAllEffectData.push_back(&vecEffectData);
	UpdateEffectData(vecAllEffectData);
}

void EffectConfig::UpdateEffectData(std::vector<std::vector<KKSG::EffectData>*>& vecAllEffectData)
{
	for (auto dataIter = vecAllEffectData.begin(); dataIter != vecAllEffectData.end(); ++dataIter)
	{
		std::vector<KKSG::EffectData>& vecEffectData = **dataIter;
		std::vector<KKSG::EffectData> vecNewEffectData;
		for (auto i = vecEffectData.begin(); i != vecEffectData.end(); ++i)
		{
			std::map<UINT32, const KKSG::EffectMultiParams*> mapBuffParams;
			std::map<UINT32, const KKSG::EffectMultiParams*> mapSkillParams;
			for (int j = 0; j < (*i).multiparams_size(); j++)
			{
				if ((*i).multiparams(j).idtype() == EffectIDType_Buff)
				{
					mapBuffParams[(*i).multiparams(j).id()] = &(*i).multiparams(j);
				}
				else if ((*i).multiparams(j).idtype() == EffectIDType_Skill)
				{
					mapSkillParams[(*i).multiparams(j).id()] = &(*i).multiparams(j);
				}
			}
			const std::vector<EffectConf*>* allConf = GetEffectConfByEffectID((*i).effectid());
			if (allConf == NULL)
			{
				LogWarn("EffectID [%u] not find in EffectTable.txt, Erase Data!!!", (*i).effectid());
				continue;
			}
			KKSG::EffectData newEffectData;
			newEffectData.set_effectid((*i).effectid());
			for (auto j = allConf->begin(); j != allConf->end(); j++)
			{
				EffectConf* conf = *j;
				KKSG::EffectMultiParams& newData = *newEffectData.add_multiparams();
				const KKSG::EffectMultiParams* oldData = NULL;
				if (conf->BuffID != 0)
				{
					auto iter = mapBuffParams.find(conf->BuffID);
					if (iter != mapBuffParams.end())
					{
						oldData = iter->second;
					}
					else
					{
						newData.set_idtype(EffectIDType_Buff);
						newData.set_id(conf->BuffID);
					}
				}
				else
				{
					UINT32 skillID = XCommon::XHash(conf->SkillScript.c_str());
					auto iter = mapSkillParams.find(skillID);
					if (iter != mapSkillParams.end())
					{
						oldData = iter->second;
					}
					else
					{
						newData.set_idtype(EffectIDType_Skill);
						newData.set_id(skillID);
					}
				}
				if (oldData != NULL)
				{
					newData = *oldData;
				}
				std::vector<int> vecParams;
				for (UINT32 z = 0; z < conf->EffectParams.size(); z++)
				{
					int param;
					if (z < newData.effectparams_size())
					{
						param = newData.effectparams(z);
					}
					if (z >= newData.effectparams_size() || param < conf->EffectParams[z][1] || param > conf->EffectParams[z][2])
					{
						UINT32 distance = XRandom::randInt(0, conf->EffectParams[z][2] - conf->EffectParams[z][1] + 1);
						param = conf->EffectParams[z][1] + distance;
					}
					vecParams.push_back(param);
				}
				newData.clear_effectparams();
				for (UINT32 z = 0; z < vecParams.size(); z++)
				{
					newData.add_effectparams(vecParams[z]);
				}
			}
			vecNewEffectData.push_back(newEffectData);
		}
		vecEffectData.swap(vecNewEffectData);
	}

	///> 同类型效果在只能生效一个
	std::map<UINT32, std::pair<UINT32, UINT32>> effectBetterMap;
	for (UINT32 dataPos = 0; dataPos < vecAllEffectData.size(); ++dataPos)
	{
		std::vector<KKSG::EffectData>& vecEffectData = *vecAllEffectData[dataPos];
		for (UINT32 i = 0; i < vecEffectData.size(); ++i)
		{
			KKSG::EffectData& nowEffect = vecEffectData[i];
			nowEffect.set_iswork(false);

			const EffectDesConf* conf = EffectConfig::Instance()->GetEffectDesConf(nowEffect.effectid());
			if (conf == NULL)
			{
				continue;
			}
			const EffectTypeConf* typeConf = EffectConfig::Instance()->GetEffectTypeConf(conf->EffectType);
			if (typeConf == NULL)
			{
				continue;
			}
			if (typeConf->CanOverlap)
			{
				nowEffect.set_iswork(true);
				continue;
			}
			auto effectBetterIter = effectBetterMap.find(conf->EffectType);
			if (effectBetterIter == effectBetterMap.end())
			{
				effectBetterMap[conf->EffectType] = std::make_pair(dataPos, i);
				nowEffect.set_iswork(true);
				continue;
			}
			KKSG::EffectData& preEffect = vecAllEffectData[effectBetterIter->second.first]->operator[](effectBetterIter->second.second);

			const std::vector<EffectConf*>* allConf = EffectConfig::Instance()->GetEffectConfByEffectID(nowEffect.effectid());
			if (allConf == NULL)
			{
				continue;
			}
			if (nowEffect.multiparams_size() != allConf->size())
			{
				LogError("UpdateEffectData should guarantee it, code error");
				continue;
			}
			if (nowEffect.multiparams_size() != preEffect.multiparams_size())
			{
				LogError("EffectType [%u] same, but params not same", conf->EffectType);
				continue;
			}

			std::set<std::pair<UINT32, UINT32>> sortConf;
			for (UINT32 j = 0; j < allConf->size(); ++j)
			{
				sortConf.insert(std::make_pair((*allConf)[j]->CompareSortID, j));
			}
			bool needChanged = false;
			for (auto j = sortConf.begin(); j != sortConf.end(); ++j)
			{
				UINT32 pos = j->second;
				const KKSG::EffectMultiParams& nowParams = nowEffect.multiparams(pos);
				const KKSG::EffectMultiParams& preParams = preEffect.multiparams(pos);
				if (nowParams.effectparams_size() != preParams.effectparams_size())
				{
					LogError("EffectType [%u] same, but one effect params's size not same", conf->EffectType);
					break;
				}
				bool isSame = true;
				for (auto k = (*allConf)[pos]->CompareParams.begin(); k != (*allConf)[pos]->CompareParams.end(); ++k)
				{
					if ((*k)[0] >= nowParams.effectparams_size())
					{
						isSame = false;
						break;
					}
					if (nowParams.effectparams((*k)[0]) != preParams.effectparams((*k)[0]))
					{
						if ((*k)[1] == 0)
						{
							needChanged = nowParams.effectparams((*k)[0]) < preParams.effectparams((*k)[0]);
						}
						else
						{
							needChanged = nowParams.effectparams((*k)[0]) > preParams.effectparams((*k)[0]);
						}
						isSame = false;
						break;
					}
				}
				if (!isSame)
				{
					break;
				}
			}
			if (needChanged)
			{
				effectBetterMap[conf->EffectType] = std::make_pair(dataPos, i);
				preEffect.set_iswork(false);
				nowEffect.set_iswork(true);
			}
		}
	}
}

const std::vector<EffectRandomConf*>* EffectConfig::GetRandomConf(UINT32 effectRandomID, UINT32 profID)
{
	auto iter = m_mapRandomConf.find(effectRandomID);
	if (iter == m_mapRandomConf.end())
	{
		return NULL;
	}
	auto iter2 = iter->second.find(profID);
	if (iter2 == iter->second.end())
	{
		return NULL;
	}
	return &iter2->second;
}

void EffectConfig::GetRandomEffectIDs(const std::vector<UINT32>& randomEffectIDs, UINT32 profID, UINT32 num, std::vector<UINT32>& effectIDs)
{
	effectIDs.clear();
	std::string strRandomEffectIDs;
	UINT32 randomSum = 0;
	std::map<UINT32, UINT32> effectID2Weight;
	for (auto i = randomEffectIDs.begin(); i != randomEffectIDs.end(); i++)
	{
		if (strRandomEffectIDs.empty()) strRandomEffectIDs = ToString(*i);
		else strRandomEffectIDs += "|" + ToString(*i);
		const std::vector<EffectRandomConf*>* vecConf = GetRandomConf(*i, profID);
		if (vecConf == NULL)
		{
			continue;
		}
		for (auto j = vecConf->begin(); j != vecConf->end(); j++)
		{
			if ((*j)->Weight == 0)
			{
				continue;
			}
			if (effectID2Weight.find((*j)->EffectID) == effectID2Weight.end())
			{
				effectID2Weight.insert(std::make_pair((*j)->EffectID, 0));
			}
			effectID2Weight[(*j)->EffectID] += (*j)->Weight;
			randomSum += (*j)->Weight;
		}
	}
	if (randomSum == 0)
	{
		LogWarn("randomEffectIDs [%s], profID [%u], Weight Sum is 0", strRandomEffectIDs.c_str(), profID);
		return;
	}
	if (num > effectID2Weight.size())
	{
		LogWarn("randomEffectIDs [%s], profID [%u], all effect num [%u] < should give num [%u]", strRandomEffectIDs.c_str(), profID, effectID2Weight.size(), num);
	}
	for (UINT32 i = 0; i < num; i++)
	{
		if (randomSum == 0 || effectID2Weight.empty())
		{
			break;
		}
		UINT32 random = XRandom::randInt(1, randomSum + 1), prob = 0;
		for (auto j = effectID2Weight.begin(); j != effectID2Weight.end(); j++)
		{
			prob += j->second;
			if (prob >= random)
			{
				effectIDs.push_back(j->first);
				randomSum -= j->second;
				effectID2Weight.erase(j->first);
				break;
			}
		}
	}
}