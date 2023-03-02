#include "pch.h"
#include "custombattlegenerator.h"
#include "custombattle.h"
#include "custombattleconfig.h"

CustomBattleGenerator::CustomBattleGenerator()
{

}

CustomBattleGenerator::~CustomBattleGenerator()
{

}

CustomBattle* CustomBattleGenerator::Generate(const KKSG::CustomBattleConfig& config, UINT64 roleid, bool issystem, UINT32 serverid)
{
	CustomBattle* battle = NULL;
	switch (config.fighttype())
	{
	case KKSG::CustomBattle_PK_Normal:
		{
			if (issystem)
			{
				battle = new CustomBattleSystemPk(config, 0, 0);
			}
			else
			{
				battle = new CustomBattlePk(config, roleid, serverid);
				battle->SetChanged();
			}
			break;
		}
	case KKSG::CustomBattle_PKTwo_Normal:
		{
			battle = new CustomBattlePk(config, roleid, serverid);
			battle->SetChanged();
			break;
		}
	default:
		break;
	}


	if (NULL != battle)
	{
		SSInfo<<"new battle type:"<<config.fighttype()<<" configid:"<<config.configid()<<" scale:"<<config.scalemask()<<" role:"<<roleid<<" uid:"<<battle->GetID()<<END;
	}
	else
	{
		SSWarn<<"generate battle failed, config:"<<config.configid()<<" roleid:"<<roleid<<END;
	}
	return battle;
}
