#include "pch.h"
#include "XLevelScriptMgr.h"
#include "scene/sceneconfig.h"
#include "config.h"
#include "scene/scene.h"
#include "foreach.h"
#include "entity/XActionDefine.h"
#include "scene/scenefinder.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "scene/ptcg2c_executelevelscriptntf.h"
#include "unit/role.h"
#include "component/XNetComponent.h"
#include "unit/enemy.h"
#include "table/CutSceneReader.h"

using namespace std;

INSTANCE_SINGLETON(XLevelScriptMgr)

XLevelScriptMgr::XLevelScriptMgr()
{

}

XLevelScriptMgr::~XLevelScriptMgr()
{

}

const std::map<std::string, XLevelFunc>* XLevelScriptMgr::GetLevelScriptInfo(UINT32 dwSceneID)
{
	std::map< UINT32, std::map<std::string, XLevelFunc> >::iterator it = m_scripts.find(dwSceneID);
	if(it != m_scripts.end())
	{
		return &(it->second);
	}
	return NULL;

}

const std::vector<XLevelInfo>* XLevelScriptMgr::GetLevelInfos(UINT32 sceneID)
{
	auto find = m_infos.find(sceneID);
	if (find != m_infos.end())
	{
		return &find->second;
	}
	
	return NULL;
}

void XLevelScriptMgr::ParseInfo(std::stringstream& ss, UINT32 sceneID)
{
	std::vector<XLevelFunc*> funcs;
	string line = "";
	string funcName = "";
	while (!ss.eof())
	{
		std::getline(ss, line);

		if(line.size() == 0) continue;

		int offset = line[line.size() - 1] == '\r' ? 1 : 0;
		line = line.substr(0, line.size()-offset);

		if (line.find("func:") == 0)
		{
			vector<string> strInfos = Split(line, ':');
			if (strInfos.size() == 2)
			{
				funcName = strInfos[1];
			}
		}
		else if (line.find("info:") == 0)
		{
			vector<string> strInfos = Split(line, ':');
			if (strInfos.size() == 2)
			{
				AddInfo(sceneID, strInfos[1]);
			}
		}
		else if (line.find("addbuff") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			if (strInfos.size() >= 4)
			{
				AddCmd(Level_Cmd_Addbuff,sceneID, funcName, strInfos);
			}
		}
		else if (line.find("removebuff") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			if (strInfos.size() >= 3)
			{
				AddCmd(Level_Cmd_Removebuff,sceneID, funcName, strInfos);
			}
		}
		else if (line.find("killspawn") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			if (strInfos.size() >= 2)
			{
				AddCmd(Level_Cmd_KillSpawn,sceneID, funcName, strInfos);
			}
		}
		else if (line.find("killally") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_KillAlly,sceneID, funcName, strInfos);
		}
		else if (line.find("killwave") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_KillWave,sceneID, funcName, strInfos);
		}
		else if (line.find("notice") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_Notice,sceneID, funcName, strInfos);
		}
		else if (line.find("stopnotice") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_StopNotice,sceneID, funcName, strInfos);
		}
		else if (line.find("opendoorex") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_OpenDoorEx, sceneID, funcName, strInfos);
		}
		else if (line.find("opendoor") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_Opendoor,sceneID, funcName, strInfos);
		}
		else if (line.find("showcutscene") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_Cutscene,sceneID, funcName, strInfos);
		}
		else if (line.find("summon") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_Summon,sceneID, funcName, strInfos);
		}
		else if (line.find("killallspawn") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_KillAllSpawn,sceneID, funcName, strInfos);
		}
		else if (line.find("npcpopspeek") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_NpcPopSpeek,sceneID, funcName, strInfos);
		}
		else if (line.find("solocamera") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_SoloCamera,sceneID, funcName, strInfos);
		}
		else if (line.find("aicommand") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_AICommand, sceneID, funcName, strInfos);
		}
		else if (line.find("showdirection") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_SHOWDIRECTION, sceneID, funcName, strInfos);
		}
		else if (line.find("setextstring") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_SETEXTSTRING, sceneID, funcName, strInfos);
		}
		else if (line.find("bubble") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_BUBBLE, sceneID, funcName, strInfos);
		}
		else if (line.find("hidebillboard") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_HIDEBILLBOARD, sceneID, funcName, strInfos);
		}
		else if (line.find("continue_UI") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_CONTINUEUI, sceneID, funcName, strInfos);
		}
		else if (line.find("addscenespantime") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_AddScenelSpanTime, sceneID, funcName, strInfos);
		}
		else if (line.find("justfx") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_JustFx, sceneID, funcName, strInfos);
		}
		else if (line.find("playfx") == 0)
		{
			vector<string> strInfos = Split(line, ' ');
			AddCmd(Level_Cmd_PlayFx, sceneID, funcName, strInfos);
		}
	}
}

void XLevelScriptMgr::AddInfo(UINT32 sceneID, std::string& infos)
{
	vector<string> strInfos = Split(infos, ' ');
	if (strInfos.size() < 4)
		return;

	XLevelInfo info;
	info.height = FLT_MAX;
	if (strInfos.size() >= 4)
	{
		info.infoName = strInfos[0];

		vector<string> pos = Split(strInfos[1], '|');
		if (pos.size() == 4)
		{
			info.x = convert<float>(pos[0]);
			info.y = convert<float>(pos[1]);
			info.z = convert<float>(pos[2]);
			info.face = convert<float>(pos[3]);
		}

		if (strcmp(strInfos[2].c_str(), "on") == 0)
		{
			info.enable = true;
		}
		else
			info.enable = false;

		info.width = convert<float>(strInfos[3]);
	}
	if (strInfos.size() >= 5)
	{
		info.height = convert<float>(strInfos[4]);
	}
	if (strInfos.size() >= 6)
	{
		info.thickness = convert<float>(strInfos[5]);
	}
	if (strInfos.size() >= 7)
	{
		info.number = convert<int>(strInfos[6]);
	}

	m_infos[sceneID].push_back(info);
}

void XLevelScriptMgr::AddCmd(LevelCmd type,UINT32 sceneID, std::string& funcName, std::vector<string>& strInfos)
{
	XLevelCmd cmd;
	cmd.type = type;
	for (size_t i = 0; i < strInfos.size(); ++i)
	{
		cmd.params.push_back(strInfos[i]);
	}
	m_scripts[sceneID][funcName].cmds.push_back(cmd);

}

void XLevelScriptMgr::Uninit()
{
	Release();
}

void XLevelScriptMgr::Release()
{
	m_scripts.clear();
	m_infos.clear();
}

void XLevelScriptMgr::AddSceneSpawnScripts(UINT32 dwSceneID, const std::string& strLevelConfigFile)
{
	if(strLevelConfigFile.empty()) return;

	auto i = m_scripts.find(dwSceneID);
	if(i != m_scripts.end())
	{
		SSInfo<<"clear script, sceneid:"<<i->first<<END;
		i->second.clear();
	}
	auto j = m_infos.find(dwSceneID);
	if(j != m_infos.end())
	{
		SSInfo<<"clear info, sceneid:"<<j->first<<END;
		j->second.clear();
	}

	string strFile = string("table/") + strLevelConfigFile + string("_sc") + string(".txt");
	std::string data = GSConfig::Instance()->ReadFileContent(strFile.c_str());
	if (data.empty())
	{
		return;
	}
	std::stringstream ss(data);
	ParseInfo(ss, dwSceneID);
}

