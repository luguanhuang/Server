#include "pch.h"
#include "TableCmd.h"
#include "PlayerLevelManager.h"
#include "XSkillEffectMgr.h"
#include "gamelogic/itemconfig.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "scene/sceneconfig.h"
#include "gamelogic/AchivementMgr.h"
#include "table/globalconfig.h"
#include "gamelogic/ibshopmgr.h"

INSTANCE_SINGLETON(TableCmd)

TableCmd::TableCmd()
{

}

TableCmd::~TableCmd()
{

}

struct ReloadTable
{
	std::string name;
	void (*fun)(const ReloadTable &);

	ReloadTable(const char *name_, void (*fun_)(const ReloadTable &))
	{
		name = name_;
		fun = fun_;
	}
};

template <typename T>
void ReloadCMD(const ReloadTable &table)
{
	if (T::Instance()->Init())
	{
		SSInfo << "load " << table.name << " config ok!" << END;
	}
	else
	{
		SSInfo << "load " << table.name << " config failed!" << END;
	}
}

static std::list<ReloadTable> Handlers;

bool TableCmd::Init()
{
	CmdLine::RegistCmd("table", CmdLineTable);

	Handlers.push_back(ReloadTable("player", ReloadCMD<PlayerLevelManager>));
	Handlers.push_back(ReloadTable("skill", ReloadCMD<XSkillEffectMgr>));
	Handlers.push_back(ReloadTable("item", ReloadCMD<ItemConfig>));
	Handlers.push_back(ReloadTable("entity", ReloadCMD<XEntityInfoLibrary>));
	//Handlers.push_back(ReloadTable("scene", ReloadCMD<SceneConfig>));
	Handlers.push_back(ReloadTable("achive", ReloadCMD<AchivementMgr>));
	Handlers.push_back(ReloadTable("ibshop", ReloadCMD<IBShopMgr>));
	return true;
}

void TableCmd::Uninit()
{

}

bool TableCmd::CmdLineTable(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() == 1)
	{
		SSInfo << "table cmds :\n";
		for (auto i = Handlers.begin(); i != Handlers.end(); ++i)
		{
			SSInfo << "table reload " << i->name << "\n";
		}
		SSInfo << END;
		return true;
	}

	if (args[1] == "reload")
	{
		if (args.size() < 3)
		{
			return false;
		}

		for (auto i = Handlers.begin(); i != Handlers.end(); ++i)
		{
			if (i->name == args[2])
			{
				i->fun(*i);
				return true;
			}
		}

		SSInfo << "table cmd not found " << args[2] << END;
		return false;
	}

	return false;
}

