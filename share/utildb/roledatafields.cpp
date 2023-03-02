#include "pch.h"
#include "roledatafields.h"


CRoleDataFields::CRoleDataFields(KKSG::RoleAllInfo* poRoleInfo)
{
	if(poRoleInfo == NULL)
	{
		LogFatal("Null RoleAllInfo ptr");
		return;
	}

	//proto×Ö¶Î¶¨ÒåË³Ðò ºÍDBÖÐTable×Ö¶ÎË³Ðò Ò»ÖÂ

	m_oFields.reserve(128);

	m_oFields.push_back(poRoleInfo->mutable_brief());
	m_oFields.push_back(poRoleInfo->mutable_attributes());
	m_oFields.push_back(poRoleInfo->mutable_bag());
	m_oFields.push_back(poRoleInfo->mutable_lottery());
	m_oFields.push_back(poRoleInfo->mutable_stages());
	m_oFields.push_back(poRoleInfo->mutable_checkinrecord());
	m_oFields.push_back(poRoleInfo->mutable_activityrecord());
	m_oFields.push_back(poRoleInfo->mutable_arenarecord());
	m_oFields.push_back(poRoleInfo->mutable_rewardrecord());
	m_oFields.push_back(poRoleInfo->mutable_buyinfo());
	m_oFields.push_back(poRoleInfo->mutable_shoprecord());
	m_oFields.push_back(poRoleInfo->mutable_flowerrecord());
	m_oFields.push_back(poRoleInfo->mutable_guildrecord());
	m_oFields.push_back(poRoleInfo->mutable_pkrecord());
	m_oFields.push_back(poRoleInfo->mutable_config());
	m_oFields.push_back(poRoleInfo->mutable_tshowvoterecord());
	m_oFields.push_back(poRoleInfo->mutable_camprolerecord());
	m_oFields.push_back(poRoleInfo->mutable_findbackrecord());
	m_oFields.push_back(poRoleInfo->mutable_extrainfo());
	m_oFields.push_back(poRoleInfo->mutable_towerrecord());

	m_oFields.push_back(poRoleInfo->mutable_loginrecord());
	m_oFields.push_back(poRoleInfo->mutable_pvpdata());
	m_oFields.push_back(poRoleInfo->mutable_qarecord());
	m_oFields.push_back(poRoleInfo->mutable_dragoninfo());
	m_oFields.push_back(poRoleInfo->mutable_fashionrecord());
	m_oFields.push_back(poRoleInfo->mutable_liverecord());
	m_oFields.push_back(poRoleInfo->mutable_payv2());
	m_oFields.push_back(poRoleInfo->mutable_petsys());
	m_oFields.push_back(poRoleInfo->mutable_firstpassrecord());
	m_oFields.push_back(poRoleInfo->mutable_ibshopitems());
	m_oFields.push_back(poRoleInfo->mutable_spriterecord());
	m_oFields.push_back(poRoleInfo->mutable_atlas());
	m_oFields.push_back(poRoleInfo->mutable_riskrecord());
	m_oFields.push_back(poRoleInfo->mutable_task_record());
	m_oFields.push_back(poRoleInfo->mutable_idiprecord());
	m_oFields.push_back(poRoleInfo->mutable_spactivityrecord());
	m_oFields.push_back(poRoleInfo->mutable_designatinorecord());
	m_oFields.push_back(poRoleInfo->mutable_levelsealdata());
	m_oFields.push_back(poRoleInfo->mutable_buffrecord());
	m_oFields.push_back(poRoleInfo->mutable_pushinfo());

	m_oFields.push_back(poRoleInfo->mutable_qqvip());
	m_oFields.push_back(poRoleInfo->mutable_teamdbinfo());
	m_oFields.push_back(poRoleInfo->mutable_misc());
	m_oFields.push_back(poRoleInfo->mutable_partner());
	m_oFields.push_back(poRoleInfo->mutable_achieve());
	m_oFields.push_back(poRoleInfo->mutable_skill());
	m_oFields.push_back(poRoleInfo->mutable_chat());
	m_oFields.push_back(poRoleInfo->mutable_herobattle());
	m_oFields.push_back(poRoleInfo->mutable_reportdata());

	m_oFields.push_back(poRoleInfo->mutable_system());
	m_oFields.push_back(poRoleInfo->mutable_military());
	//m_oFields.push_back(poRoleInfo->mutable_custombattle());
	m_oFields.push_back(poRoleInfo->mutable_platformshareresult());
	m_oFields.push_back(poRoleInfo->mutable_weekend4v4data());
	m_oFields.push_back(poRoleInfo->mutable_tajiehelprecord());
    m_oFields.push_back(poRoleInfo->mutable_dragongroupdb());
    m_oFields.push_back(poRoleInfo->mutable_battlefield());
    m_oFields.push_back(poRoleInfo->mutable_npcflrec());
    m_oFields.push_back(poRoleInfo->mutable_competedragoninfo());
	m_oFields.push_back(poRoleInfo->mutable_dragonguild());
}

CRoleDataFields::~CRoleDataFields()
{
}

std::string CRoleDataFields::GetFieldName(UINT32 dwIndex) const
{
	google::protobuf::MessageLite* poData = GetField(dwIndex);
	return CRoleDataFields::GetDataTypeName(poData);
}

std::string CRoleDataFields::GetDataTypeName(const google::protobuf::MessageLite* poDataField)
{
	if(poDataField == NULL)
	{
		return "";
	}

	std::string strType = poDataField->GetTypeName();
	std::string::size_type pos = strType.find_last_of('.');
	return pos == std::string::npos ? strType : strType.substr(pos + 1, strType.size() - pos - 1);
}


bool CRoleDataSaveUtil::SaveAll(const CRoleDataFields& roFields, KKSG::SaveRoleDataArg& roArg)
{
	for(UINT32 i = 0; i < roFields.GetCount(); ++i)
	{
		google::protobuf::MessageLite* poField = roFields.GetField(i);
		if(poField == NULL)
		{
			LogError("Null field");
			return false;
		}

		try
		{
			roArg.add_fieldname(CRoleDataFields::GetDataTypeName(poField));
			roArg.add_fielddata(poField->SerializeAsString());
		}
		catch(std::exception& e)
		{
			LogError("Serialize role data field=%s failed, errMsg=%s", poField->GetTypeName().c_str(), e.what());
			return false;
		}
	}
	return true;
}

bool CRoleDataSaveUtil::SaveField(const google::protobuf::MessageLite* poDataField, KKSG::SaveRoleDataArg& roArg)
{
	if(poDataField == NULL) return false;

	try
	{
		roArg.add_fieldname(CRoleDataFields::GetDataTypeName(poDataField));
		roArg.add_fielddata(poDataField->SerializeAsString());
	}
	catch(std::exception& e)
	{
		LogError("Serialize role data field=%s failed, errMsg=%s", poDataField->GetTypeName().c_str(), e.what());
		return false;
	}
	return true;
}


CRoleDataFieldCleanup::CRoleDataFieldCleanup()
{
	KKSG::RoleAllInfo oTemp;

	RegisterFunc(oTemp.mutable_brief(), &KKSG::RoleAllInfo::clear_brief);
	RegisterFunc(oTemp.mutable_attributes(), &KKSG::RoleAllInfo::clear_attributes);
	RegisterFunc(oTemp.mutable_bag(), &KKSG::RoleAllInfo::clear_bag);
	RegisterFunc(oTemp.mutable_lottery(), &KKSG::RoleAllInfo::clear_lottery);
	RegisterFunc(oTemp.mutable_stages(), &KKSG::RoleAllInfo::clear_stages);
	RegisterFunc(oTemp.mutable_checkinrecord(), &KKSG::RoleAllInfo::clear_checkinrecord);
	RegisterFunc(oTemp.mutable_activityrecord(), &KKSG::RoleAllInfo::clear_activityrecord);
	RegisterFunc(oTemp.mutable_arenarecord(), &KKSG::RoleAllInfo::clear_arenarecord);
	RegisterFunc(oTemp.mutable_rewardrecord(), &KKSG::RoleAllInfo::clear_rewardrecord);
	RegisterFunc(oTemp.mutable_buyinfo(), &KKSG::RoleAllInfo::clear_buyinfo);
	RegisterFunc(oTemp.mutable_shoprecord(), &KKSG::RoleAllInfo::clear_shoprecord);
	RegisterFunc(oTemp.mutable_flowerrecord(), &KKSG::RoleAllInfo::clear_flowerrecord);
	RegisterFunc(oTemp.mutable_guildrecord(), &KKSG::RoleAllInfo::clear_guildrecord);
	RegisterFunc(oTemp.mutable_pkrecord(), &KKSG::RoleAllInfo::clear_pkrecord);
	RegisterFunc(oTemp.mutable_config(), &KKSG::RoleAllInfo::clear_config);
	RegisterFunc(oTemp.mutable_tshowvoterecord(), &KKSG::RoleAllInfo::clear_tshowvoterecord);
	RegisterFunc(oTemp.mutable_camprolerecord(), &KKSG::RoleAllInfo::clear_camprolerecord);
	RegisterFunc(oTemp.mutable_findbackrecord(), &KKSG::RoleAllInfo::clear_findbackrecord);
	RegisterFunc(oTemp.mutable_extrainfo(), &KKSG::RoleAllInfo::clear_extrainfo);
	RegisterFunc(oTemp.mutable_towerrecord(), &KKSG::RoleAllInfo::clear_towerrecord);

	RegisterFunc(oTemp.mutable_loginrecord(), &KKSG::RoleAllInfo::clear_loginrecord);
	RegisterFunc(oTemp.mutable_pvpdata(), &KKSG::RoleAllInfo::clear_pvpdata);
	RegisterFunc(oTemp.mutable_qarecord(), &KKSG::RoleAllInfo::clear_qarecord);
	RegisterFunc(oTemp.mutable_dragoninfo(), &KKSG::RoleAllInfo::clear_dragoninfo);
	RegisterFunc(oTemp.mutable_fashionrecord(), &KKSG::RoleAllInfo::clear_fashionrecord);
	RegisterFunc(oTemp.mutable_liverecord(), &KKSG::RoleAllInfo::clear_liverecord);
	RegisterFunc(oTemp.mutable_payv2(), &KKSG::RoleAllInfo::clear_payv2);
	RegisterFunc(oTemp.mutable_petsys(), &KKSG::RoleAllInfo::clear_petsys);
	RegisterFunc(oTemp.mutable_firstpassrecord(), &KKSG::RoleAllInfo::clear_firstpassrecord);
	RegisterFunc(oTemp.mutable_ibshopitems(), &KKSG::RoleAllInfo::clear_ibshopitems);
	RegisterFunc(oTemp.mutable_spriterecord(), &KKSG::RoleAllInfo::clear_spriterecord);
	RegisterFunc(oTemp.mutable_atlas(), &KKSG::RoleAllInfo::clear_atlas);
	RegisterFunc(oTemp.mutable_riskrecord(), &KKSG::RoleAllInfo::clear_riskrecord);
	RegisterFunc(oTemp.mutable_task_record(), &KKSG::RoleAllInfo::clear_task_record);
	RegisterFunc(oTemp.mutable_idiprecord(), &KKSG::RoleAllInfo::clear_idiprecord);
	RegisterFunc(oTemp.mutable_spactivityrecord(), &KKSG::RoleAllInfo::clear_spactivityrecord);
	RegisterFunc(oTemp.mutable_designatinorecord(), &KKSG::RoleAllInfo::clear_designatinorecord);
	RegisterFunc(oTemp.mutable_levelsealdata(), &KKSG::RoleAllInfo::clear_levelsealdata);
	RegisterFunc(oTemp.mutable_buffrecord(), &KKSG::RoleAllInfo::clear_buffrecord);
	RegisterFunc(oTemp.mutable_pushinfo(), &KKSG::RoleAllInfo::clear_pushinfo);

	RegisterFunc(oTemp.mutable_qqvip(), &KKSG::RoleAllInfo::clear_qqvip);
	RegisterFunc(oTemp.mutable_teamdbinfo(), &KKSG::RoleAllInfo::clear_teamdbinfo);
	RegisterFunc(oTemp.mutable_misc(), &KKSG::RoleAllInfo::clear_misc);
	RegisterFunc(oTemp.mutable_partner(), &KKSG::RoleAllInfo::clear_partner);
	RegisterFunc(oTemp.mutable_achieve(), &KKSG::RoleAllInfo::clear_achieve);
	RegisterFunc(oTemp.mutable_skill(), &KKSG::RoleAllInfo::clear_skill);
	RegisterFunc(oTemp.mutable_chat(), &KKSG::RoleAllInfo::clear_chat);
	RegisterFunc(oTemp.mutable_herobattle(), &KKSG::RoleAllInfo::clear_herobattle);
	RegisterFunc(oTemp.mutable_reportdata(), &KKSG::RoleAllInfo::clear_reportdata);
	RegisterFunc(oTemp.mutable_system(), &KKSG::RoleAllInfo::clear_system);
	RegisterFunc(oTemp.mutable_military(), &KKSG::RoleAllInfo::clear_military);
	//RegisterFunc(oTemp.mutable_custombattle(), &KKSG::RoleAllInfo::clear_custombattle);
	RegisterFunc(oTemp.mutable_platformshareresult(),&KKSG::RoleAllInfo::clear_platformshareresult);
	RegisterFunc(oTemp.mutable_weekend4v4data(),&KKSG::RoleAllInfo::clear_weekend4v4data);
	RegisterFunc(oTemp.mutable_tajiehelprecord(),&KKSG::RoleAllInfo::clear_tajiehelprecord);
    RegisterFunc(oTemp.mutable_dragongroupdb(),&KKSG::RoleAllInfo::clear_dragongroupdb);
	RegisterFunc(oTemp.mutable_battlefield(), &KKSG::RoleAllInfo::clear_battlefield);
	RegisterFunc(oTemp.mutable_npcflrec(), &KKSG::RoleAllInfo::clear_npcflrec);
    RegisterFunc(oTemp.mutable_competedragoninfo(),&KKSG::RoleAllInfo::clear_competedragoninfo);
	RegisterFunc(oTemp.mutable_dragonguild(),&KKSG::RoleAllInfo::clear_dragonguild);
}

CRoleDataFieldCleanup::~CRoleDataFieldCleanup()
{
}

CRoleDataFieldCleanup& CRoleDataFieldCleanup::Instance()
{
	static CRoleDataFieldCleanup soInstance;
	return soInstance;
}

bool CRoleDataFieldCleanup::ClearField(KKSG::RoleAllInfo* poRoleInfo, const google::protobuf::MessageLite* poField)
{
	if(poRoleInfo == NULL || poField == NULL) return false;

	std::string strFieldName = CRoleDataFields::GetDataTypeName(poField);
	CFieldClearFuncMap::iterator it = m_oFuncMap.find(strFieldName);
	if(it == m_oFuncMap.end())
	{
		LogError("Can't find field [%s] 's clear function", strFieldName.c_str());
		return false;
	}

	(poRoleInfo->*(it->second))();
	return true;
}

void CRoleDataFieldCleanup::RegisterFunc(const google::protobuf::MessageLite* poField, PField_ClearFunc pFunc)
{
	if(poField == NULL || pFunc == NULL) return;

	std::string strFieldName = CRoleDataFields::GetDataTypeName(poField);
	CFieldClearFuncMap::iterator it = m_oFuncMap.find(strFieldName);
	if(it != m_oFuncMap.end())
	{
		LogFatal("Field [%s] 's clear function has been registered", strFieldName.c_str());
		LogMgr::Instance()->Flush();
		assert(false);
		return;
	}

	m_oFuncMap.insert(std::make_pair(strFieldName, pFunc));
}
