#ifndef __XLEVEL_SCRIPT_MGR__
#define __XLEVEL_SCRIPT_MGR__

class Scene;

enum LevelCmd
{
	Level_Cmd_Invalid,
	Level_Cmd_Notice,
	Level_Cmd_Opendoor,
	Level_Cmd_Cutscene,
	Level_Cmd_Addbuff,
	Level_Cmd_Removebuff,
	Level_Cmd_KillSpawn,
	Level_Cmd_KillAlly,
	Level_Cmd_Summon,
	Level_Cmd_KillAllSpawn,
	Level_Cmd_StopNotice,
	Level_Cmd_NpcPopSpeek,
	Level_Cmd_SoloCamera,
	Level_Cmd_AICommand,
	Level_Cmd_SHOWDIRECTION,
	Level_Cmd_SETEXTSTRING,
	Level_Cmd_BUBBLE,
	Level_Cmd_HIDEBILLBOARD,
	Level_Cmd_CONTINUEUI,
	Level_Cmd_AddScenelSpanTime,
	Level_Cmd_KillWave,
	Level_Cmd_OpenDoorEx,
	Level_Cmd_JustFx,
	Level_Cmd_PlayFx,
};
struct XLevelCmd
{
	LevelCmd type;
	std::vector<std::string> params;
	XLevelCmd()
	{
		type = Level_Cmd_Invalid;
	}

};

struct XLevelFunc
{
	std::string funcName;
	std::vector<XLevelCmd> cmds;
};

struct XLevelInfo
{
	XLevelInfo()
	{
		infoName.clear();
		x = y = z = face = width = height = thickness = 0;
		number = 0;
		enable = false;
	}

	std::string infoName;
	float x,y,z,face,width,height,thickness;
	int number;
	bool enable;
};

class XLevelScriptMgr
{
public:
	XLevelScriptMgr();
	~XLevelScriptMgr();
	DECLARE_SINGLETON(XLevelScriptMgr)

	bool Init(){return true;}
	void Uninit();
	void Release();

	void AddSceneSpawnScripts(UINT32 dwSceneID, const std::string& strLevelConfigFile);

	const std::vector<XLevelInfo>* GetLevelInfos(UINT32 sceneID);
	const std::map<std::string, XLevelFunc>* GetLevelScriptInfo(UINT32 dwSceneID);

protected:
	void AddCmd(LevelCmd type,UINT32 sceneID, std::string& funcName, std::vector<std::string>& strInfos);
	void ParseInfo(std::stringstream& ss, UINT32 sceneID);

	void AddInfo(UINT32 sceneID, std::string& info);

private:
	std::map<UINT32, std::vector<XLevelInfo>> m_infos;
	std::map<UINT32, std::map<std::string, XLevelFunc>> m_scripts;
};
#endif // __XLEVEL_SCRIPT_MGR__