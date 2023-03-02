#include "pch.h"
#include "custombattle/custombattle.h"

//////////////////////////////////////////////////////////////////////////
CustomBattleSystemPk::CustomBattleSystemPk(const KKSG::CustomBattleConfig& config, UINT64 roleid, UINT32 serverid)
:CustomBattle(config, roleid, serverid)
{
	m_data.set_uid(config.configid());
	m_data.mutable_config()->set_issystem(true);
	SetState(KKSG::CustomBattle_Going);
}

CustomBattleSystemPk::~CustomBattleSystemPk()
{

}

bool CustomBattleSystemPk::IsIn(UINT64 roleid)
{
	SSWarn<<"invalid handle, roleid:"<<roleid<<END;
	return false;
}

void CustomBattleSystemPk::AddRole(UINT64 roleid, const std::string& name)
{
	SSInfo<<"uid:"<<m_data.uid()<<" add roleid:"<<roleid<<END;
}

void CustomBattleSystemPk::DelRole(UINT64 roleid)
{
	SSInfo<<"uid:"<<m_data.uid()<<" del roleid:"<<roleid<<END;
}

void CustomBattleSystemPk::AddPoint(UINT64 roleid, int point)
{
	SSWarn<<"uid:"<<m_data.uid()<<" invalid handle, roleid:"<<roleid<<" point"<<point<<END;
}

