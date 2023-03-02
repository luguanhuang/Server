#include "pch.h"
#include "battle/ptcc2g_battlelogreport.h"
#include "utility/tloggerbattle.h"
#include "utility/loghelper.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/scenestatistics.h"
#include "scene/securitystatistics.h"

// generate by ProtoGen at date: 2016/11/2 20:50:37

void PtcC2G_BattleLogReport::Process(UINT32 dwConnID)
{
	if (m_Data.key_size() != m_Data.value_size())
	{
		SSWarn<<"key size:"<<m_Data.key_size()<<" value size:"<<m_Data.value_size()<<" session:"<<m_sessionID<<END;
		return;
	}

	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"session invalid:"<<m_sessionID<<END;
		return;
	}
	// record
	Scene* scene = role->GetCurrScene();
	if (NULL == scene)
	{
		SSWarn<<"current scene is NULL, role:"<<role->GetID()<<END;
		return;
	}
	SceneStatistics* statistics = scene->Statistics();
	if (NULL == statistics)
	{
		SSWarn<<"current scene statistics NULL, role:"<<role->GetID()<<END;
		return;
	}
	RoleBattleRecord* record = statistics->GetRecord(role->GetID());
	if (NULL == record)
	{
		SSWarn<<"statistic is NULL, role:"<<role->GetID()<<END;
		return;
	}

	///> 战斗数据
	if (role->GetSecurityStatistics() != NULL)
		role->GetSecurityStatistics()->SendRoleData(scene, record);

	record->m_clientreceived = true;

	std::map<std::string, std::string>* key2value = NULL;
	if (1 == m_Data.type())
	{
		key2value = &record->start.m_data;
	}
	else if (2 == m_Data.type())
	{
		key2value = &record->end.m_data;
	}
	else if (3 == m_Data.type())
	{
		key2value = &record->endcount.m_data;
	}
	else
	{
		SSWarn<<"invalid type:"<<m_Data.type()<<" session:"<<m_sessionID<<END;
		return;
	}

	for (int i = 0; i < m_Data.key_size(); ++i)
	{
		(*key2value)[m_Data.key(i)] = m_Data.value(i);
	}

	//LogHelper log;
	//log.SetDBType(KKSG::LOG_DB_TENCENT);
	//log.SetTableName(flow->GetTableName());

	//std::map<std::string, std::string> tempkey2value;
	//for (int i = 0; i < m_Data.key_size(); ++i)
	//{
	//	const std::string& value = m_Data.value(i);
	//	const std::string& key = m_Data.key(i);
	//	std::pair<std::map<std::string, std::string>::iterator, bool> ret = tempkey2value.insert(std::make_pair(key, value));
	//	if (!ret.second)
	//	{
	//		SSError<<"client conflict key:"<<key<<" value:"<<value<<END;
	//	}
	//}

	////for (auto i = fields->begin(); i != fields->end(); ++i)
	//{
	//	auto j = key2handler->find(*i);
	//	if (j == key2handler->end())
	//	{
	//		SSError<<"invalid key:"<<*i<<END;
	//	}
	//	else
	//	{
	//		///> client fill
	//		if (j->second > 10)
	//		{
	//			auto k = tempkey2value.find(*i);
	//			if (k == tempkey2value.end())
	//			{
	//				SSError<<"client miss key:"<<*i<<END;
	//			}
	//			else
	//			{
	//				if (11 == j->second)
	//				{
	//					//log.AddKeyValue(key.c_str(), value);
	//				}
	//				else if (12 == j->second)
	//				{

	//				}
	//				else
	//				{
	//					SSError<<"invalid handler:"<<j->second<<END;
	//				}
	//			}
	//		}
	//		///> server fill
	//		else 
	//		{
	//			if (1 == j->second)
	//			{
	//				//log.AddKeyValue(key.c_str(), value);
	//			}
	//			else if (2 == j->second)
	//			{
	//				//time_t time = convert<UINT32>(value.c_str());
	//				//log.AddKeyValueTime(key.c_str(), time);
	//			}
	//			else
	//			{
	//				SSError<<"invalid handler:"<<j->second<<END;
	//			}
	//		}
	//	}
	//}
	//log.Send();
}
