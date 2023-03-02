#include "pch.h"
#include "InspireConfig.h"
#include "util/XRandom.h"

INSTANCE_SINGLETON(InspireConfig)

bool InspireConfig::Init()
{
	if (!m_ITable.LoadFile("table/Inspire.txt"))
	{
		SSWarn<<"load file table/Inspire.txt failed"<<END;
		return false;
	}

	return true;
}

void InspireConfig::Uninit()
{

}

void InspireConfig::GetAddAttr(UINT32 scenetype, std::vector<Sequence<int,2>>& addattr)
{
	auto pconf = m_ITable.GetByscenetype(scenetype);
	if(pconf)
	{
		if(0 == pconf->addtype)
		{
			addattr = pconf->AddAttr;
		}
		else if(1 == pconf->addtype)
		{
			if(!pconf->AddAttr.empty())	
			{
				int index = XRandom::randInt(0, pconf->AddAttr.size());	
				addattr.push_back(pconf->AddAttr[index]);
			}
		}
	}
}

bool InspireConfig::HaveSceneType(UINT32 scenetype)
{
	return (NULL != m_ITable.GetByscenetype(scenetype));
}

InspireConfig::InspireConfig()
{

}

InspireConfig::~InspireConfig()
{

}
