#include "pch.h"
#include "artifactconfig.h"
#include "util/XCommon.h"
#include "item.h"
#include "globalconfig.h"


ArtifactTableData::ArtifactTableData()
{

}

ArtifactTableData::~ArtifactTableData()
{

}

bool ArtifactTableData::HandlerTableData()
{
	if (!m_oListTable.LoadFile("table/ArtifactList.txt"))
	{
		LogWarn("Load ArtifactList.txt failed");
		return false;
	}
	if (!m_oSuitTable.LoadFile("table/ArtifactSuit.txt"))
	{
		LogWarn("Load ArtifactSuit.txt failed");
		return false;
	}
	if (!m_oComposeTable.LoadFile("table/ArtifactCompose.txt"))
	{
		LogWarn("Load ArtifactCompose.txt failed");
		return false;
	}
	if (!m_oInscriptTable.LoadFile("table/ArtifactSkillInscription.txt"))
	{
		LogWarn("Load ArtifactSkillInscription.txt failed");
		return false;
	}

	for (auto i = m_oComposeTable.Table.begin(); i != m_oComposeTable.Table.end(); ++i)
	{
		if (m_composeMap.find(std::make_pair((*i)->ArtifactLevel, (*i)->ArtifactQuality)) != m_composeMap.end())
		{
			LogWarn("ArtifactCompose.txt, ArtifactLevel [%u], ArtifactQuality[%u] is duplicated key", (*i)->ArtifactLevel, (*i)->ArtifactQuality);
			return false;
		}
		m_composeMap[std::make_pair((*i)->ArtifactLevel, (*i)->ArtifactQuality)] = *i;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ArtifactConfig::ArtifactConfig()
{
}

ArtifactConfig::~ArtifactConfig()
{

}

bool ArtifactConfig::Init()
{
	return LoadFile();
}

void ArtifactConfig::Uninit()
{

}

bool ArtifactConfig::CheckFile()
{
	ArtifactTableData tmpData;
	return tmpData.HandlerTableData();
}

bool ArtifactConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	HandlerTableData();

	return true;
}

void ArtifactConfig::ClearFile()
{
	m_oListTable.Clear();
	m_oSuitTable.Clear();
	m_oComposeTable.Clear();
	m_oInscriptTable.Clear();

	m_composeMap.clear();
}

void ArtifactConfig::InitArtifactAttribute(XItem* pItem, bool isRecast)
{
	if (pItem == NULL)
	{
		return;
	}
	const ArtifactListConf* conf = GetArtifactListConf(pItem->itemID);
	if (conf == NULL)
	{
		return;
	}
	pItem->changeAttr.clear();
	if (!isRecast)
	{
		_InitOneAttribute(pItem, conf->Attributes1);
		_InitOneAttribute(pItem, conf->Attributes2);
		_InitOneAttribute(pItem, conf->Attributes3);
	}
	else
	{
		_RecastOneAttribute(pItem, conf->Attributes1);
		_RecastOneAttribute(pItem, conf->Attributes2);
		_RecastOneAttribute(pItem, conf->Attributes3);	
	}
}

const ArtifactComposeConf* ArtifactConfig::GetArtifactComposeConf(UINT32 level, UINT32 quality)
{
	auto iter = m_composeMap.find(std::make_pair(level, quality));
	if (iter != m_composeMap.end())
	{
		return iter->second;
	}
	return NULL;
}

void ArtifactConfig::_InitOneAttribute(XItem* pItem, const std::vector<Sequence<UINT32, 4>>& attrs)
{
	UINT32 probSum = 0;
	for (auto i = attrs.begin(); i != attrs.end(); ++i)
	{
		probSum += (*i)[3];
	}
	if (probSum == 0)
	{
		pItem->changeAttr.push_back(ItemChangeAttr(0, 0));
		return;
	}
	UINT32 rand = XRandom::randInt(1, probSum + 1);
	UINT32 prob = 0;
	for (auto i = attrs.begin(); i != attrs.end(); ++i)
	{
		prob += (*i)[3];
		if (prob >= rand)
		{
			pItem->changeAttr.push_back(ItemChangeAttr((*i)[0], XRandom::randInt((*i)[1], (*i)[2] + 1)));
			return;
		}
	}
	pItem->changeAttr.push_back(ItemChangeAttr(0, 0));
}

void ArtifactConfig::_RecastOneAttribute(XItem* pItem, const std::vector<Sequence<UINT32, 4>>& attrs)
{
	if (attrs.size() == 0)
	{
		pItem->changeAttr.push_back(ItemChangeAttr(0, 0));
		return;
	}
	std::map<UINT32, UINT32> attrProb;
	std::map<UINT32, std::pair<UINT32, UINT32>> attrMap;
	UINT32 probSum = 0;
	for (auto i = attrs.begin(); i != attrs.end(); ++i)
	{
		attrProb[(*i)[0]] += (*i)[3];
		probSum += (*i)[3];
		UINT32 attrMin = (*i)[1], attrMax = (*i)[2];
		if (attrMin > attrMax) std::swap(attrMin, attrMax);
		auto iter = attrMap.find((*i)[0]);
		if (iter != attrMap.end())
		{
			attrMin = std::min(attrMin, iter->second.first);
			attrMax = std::max(attrMax, iter->second.second);
		}
		attrMap[(*i)[0]] = std::make_pair(attrMin, attrMax);
	}
	UINT32 rand = XRandom::randInt(1, probSum + 1);
	UINT32 prob = 0;
	UINT32 attrID = 0;
	for (auto i = attrProb.begin(); i != attrProb.end(); ++i)
	{
		prob += i->second;
		if (prob >= rand)
		{
			attrID = i->first;
			break;
		}
	}
	UINT32 attrMin = attrMap[attrID].first, attrMax = attrMap[attrID].second;
	if (attrMin > attrMax) std::swap(attrMin, attrMax);

	probSum = 0;
	for (UINT32 i = 0; i < GetGlobalConfig().RecastAttrRate.size(); ++i)
	{
		probSum += GetGlobalConfig().RecastAttrRate[i][1];
	}
	if (probSum != 0)
	{
		rand = XRandom::randInt(1, probSum + 1);
		prob = 0;
		probSum = 0;
		for (UINT32 i = 0; i < GetGlobalConfig().RecastAttrRate.size(); ++i)
		{
			prob += GetGlobalConfig().RecastAttrRate[i][1];
			if (prob >= rand)
			{
				if (probSum > 100) probSum = 100;
				UINT32 tmpMin = attrMin + (UINT32)(1.0 * (attrMax - attrMin) * probSum / 100.0);
				probSum += GetGlobalConfig().RecastAttrRate[i][0];
				if (probSum > 100) probSum = 100;
				UINT32 tmpMax = attrMin + (UINT32)(1.0 * (attrMax - attrMin) * probSum / 100.0);

				pItem->changeAttr.push_back(ItemChangeAttr(attrID, XRandom::randInt(tmpMin, tmpMax + 1)));
				return;
			}
			probSum += GetGlobalConfig().RecastAttrRate[i][0];
		}
	}
	pItem->changeAttr.push_back(ItemChangeAttr(attrID, XRandom::randInt(attrMin, attrMax + 1)));
	return;
}

void ArtifactConfig::GetArtifactAttrRange(UINT32 artifactID, UINT32 index, UINT32 attrid, UINT32& attrfirst, UINT32& attrsecond)
{
	auto it = GetArtifactListConf(artifactID);
	if (it == NULL)
	{
		return;
	}
	std::vector<Sequence<UINT32, 4>> attrs;
	if (index == 0)
	{
		attrs = it->Attributes1;
	}
	else if (index == 1)
	{
		attrs = it->Attributes2;
	}
	else if (index == 2)
	{
		attrs = it->Attributes3;
	}
	else
	{
		return;
	}

	for (auto i = attrs.begin(); i != attrs.end(); i++)
	{
		if (i->seq[0] == attrid)
		{
			attrfirst = i->seq[1];
			attrsecond = i->seq[2];
			return;
		}
	}
}