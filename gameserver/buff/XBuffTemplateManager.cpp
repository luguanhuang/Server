#include "pch.h"
#include "XBuffTemplateManager.h"
#include "XBuff.h"
#include "foreach.h"
#include "scene/scene.h"

INSTANCE_SINGLETON(XBuffTemplateManager)

enum BuffTypeEM
{
	BUFFTYPE_CHANGE_ATTRIBUTE = 1,
	BUFFTYPE_CHANGE_STATUS = 2,
	BUFFTYPE_STUN = 3,
};

union BuffKey
{
	int value;
	struct
	{
		int buffID : 24;
		int buffLevel : 8;
	} _;
};


XBuffTemplateManager::XBuffTemplateManager()
{

}

XBuffTemplateManager::~XBuffTemplateManager()
{

}

bool XBuffTemplateManager::Init()
{
	return LoadFile();
}

void XBuffTemplateManager::Uninit()
{
	ClearFile();
}

bool XBuffTemplateManager::CheckFile()
{
	BuffTable tmpBuffTable;
	if (!tmpBuffTable.LoadFile("table/BuffList.txt"))
	{
		LogError("Load buff config file [table/BuffList.txt] failed!");
		return false;
	}
	BuffTable tmpBuffTablePVP;
	if (!tmpBuffTablePVP.LoadFile("table/BuffListPVP.txt"))
	{
		LogError("Load buff config file [table/BuffListPVP.txt] failed!");
		return false;
	}
	return true;
}

bool XBuffTemplateManager::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	//ClearFile();
	m_buff2scenesExc.clear();
	m_buff2scenesInc.clear();
	
	BuffKey key;
	BuffTable tmpBuffTable;
	tmpBuffTable.LoadFile("table/BuffList.txt");
	foreach(i in tmpBuffTable.Table)
	{
		BuffTable::RowData *poData = *i;
		key._.buffID = poData->BuffID;
		key._.buffLevel = poData->BuffLevel;

		auto j = m_BuffIndex.find(key.value);
		if (j == m_BuffIndex.end())
		{
			// new
			BuffTable::RowData* pNewRow = new BuffTable::RowData();
			pNewRow->CopyFrom(poData);
			m_BuffTable.Table.push_back(pNewRow);

			m_BuffIndex[key.value] = pNewRow;
		}
		else
		{
			// old
			j->second->CopyFrom(poData);
		}

		if (!poData->SceneEffect.empty())
		{
			for (auto i = poData->SceneEffect.begin(); i != poData->SceneEffect.end(); ++i)
			{
				if((*i) > 0)
					m_buff2scenesInc[poData->BuffID].insert(*i);
				else
					m_buff2scenesExc[poData->BuffID].insert(-(*i));
			}
		}
	}

	BuffTable tmpBuffTablePVP;
	tmpBuffTablePVP.LoadFile("table/BuffListPVP.txt");
	foreach(i in tmpBuffTablePVP.Table)
	{
		BuffTable::RowData *poData = *i;
		key._.buffID = poData->BuffID;
		key._.buffLevel = poData->BuffLevel;

		auto j = m_BuffIndexPVP.find(key.value);
		if (j == m_BuffIndexPVP.end())
		{
			BuffTable::RowData* pNewRow = new BuffTable::RowData();
			pNewRow->CopyFrom(poData);
			m_BuffTablePVP.Table.push_back(pNewRow);

			m_BuffIndexPVP[key.value] = pNewRow;
		}
		else
		{
			j->second->CopyFrom(poData);
		}

		if (!poData->SceneEffect.empty())
		{
			for (auto i = poData->SceneEffect.begin(); i != poData->SceneEffect.end(); ++i)
			{
				if((*i) > 0)
					m_buff2scenesInc[poData->BuffID].insert(*i);
				else
					m_buff2scenesExc[poData->BuffID].insert(-(*i));
			}
		}
	}

	return true;
}

void XBuffTemplateManager::ClearFile()
{
	m_BuffIndex.clear();
	m_BuffTable.Clear();
	m_BuffIndexPVP.clear();
	m_BuffTablePVP.Clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
XBuff * XBuffTemplateManager::CreateBuff(const BuffDesc &buffDesc, CombatEffectHelper* pEffectHelper)
{
	XBuff *pBuff = new XBuff();
	pBuff->Init(buffDesc, pEffectHelper);

	return pBuff;
}

BuffTable::RowData* XBuffTemplateManager::GetBuffInfo(int buffID, int BuffLevel, Scene* pScene)
{
	BuffTable::RowData *pBuffInfo = NULL;

	BuffKey key;
	key._.buffID = buffID;
	key._.buffLevel = BuffLevel;

	if(NULL != pScene && Scene::IsPVPCombat(pScene))
	{
		auto i = m_BuffIndexPVP.find(key.value);
		if(i != m_BuffIndexPVP.end())
		{
			pBuffInfo = i->second;
		}
	}
	if(pBuffInfo == NULL)
	{
		auto i= m_BuffIndex.find(key.value);
		if (i == m_BuffIndex.end())
		{
			return NULL;
		}
		else
			pBuffInfo = i->second;
	}
	// check scene invalid
	if (NULL != pScene)
	{
		auto i = m_buff2scenesInc.find(buffID);
		if (i != m_buff2scenesInc.end())
		{
			auto j = i->second.find(pScene->GetSceneType());
			if (j == i->second.end())	
			{
				return NULL;	
			}
		}

		i = m_buff2scenesExc.find(buffID);
		if (i != m_buff2scenesExc.end())
		{
			auto j = i->second.find(pScene->GetSceneType());
			if (j != i->second.end())	
			{
				return NULL;	
			}
		}
	}
	return pBuffInfo;
}

bool XBuffTemplateManager::IsScaleBuff(int buffid)
{
	BuffTable::RowData* data = GetBuffInfo(buffid, 1, NULL);
	if (NULL == data)
	{
		return false;
	}
	for (auto i = data->BuffState.begin(); i != data->BuffState.end(); ++i)
	{
		if (*i == XBuffType_Scale)
		{
			return true;
		}
	}
	return false;
}
