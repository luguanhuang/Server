#include "pch.h"
#include <time.h>
#include "role.h"
#include "gatelink.h"
#include "rolemanager.h"
#include "calcattribute.h"
#include "combatattribute.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "entity/XRole.h"
#include "table/ProfessionMgr.h"
#include "table/PlayerLevelManager.h"
#include "gamelogic/bag.h"
#include "gamelogic/rolenethandler.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/skilllevel.h"
#include "table/CutSceneReader.h"
#include "table/globalconfig.h"
#include "gamelogic/roleeventlistener.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "gamelogic/lotteryrecord.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/item.h"
#include "gamelogic/checkinrecord.h"
#include "gamelogic/activityrecord.h"
#include "gamelogic/rolefashion.h"
#include "gamelogic/arenarecord.h"
#include "gamelogic/payrecord.h"
#include "attributewatcher.h"
#include "gamelogic/buyrecord.h"
#include "gamelogic/rewardrecord.h"
#include "component/XSkillComponent.h"
#include "component/XActionStateComponent.hpp"
#include "gamelogic/AchivementActivityManager.h"
#include "gamelogic/shoprecord.h"
#include "guild/guildrecord.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "common/ptcg2c_hintnotify.h"
#include "gamelogic/AchivementDef.h"
#include "util/XCommon.h"
#include "gamelogic/pkrecord.h"
#include "foreach.h"
#include "gamelogic/tshowrecord.h"
#include "table/VIPTable.h"
#include "gamelogic/payv2Mgr.h"
#include "camp/camprecord.h"
#include "findback/findbackrecord.h"
#include "utility/tlogger.h"
#include "gamelogic/towerrecord.h"
#include "gamelogic/designationRecord.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/levelsealMgr.h"
#include "gamelogic/levelsealRecord.h"
#include "common/ptcg2c_reconnectsyncnotify.h"
#include "scene/scenefinder.h"
#include "gamelogic/achievev2Mgr.h"
#include "gamelogic/achievev2Record.h"
#include "gamelogic/loginrecord.h"
#include "gamelogic/pvprecord.h"
#include "tsssdk/TssHandler.h"
#include "guild/guildchat.h"
#include "gamelogic/team.h"
#include "gamelogic/teammgr.h"
#include "gamelogic/teammember.h"
#include "table/globalconfig.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/chatmgr.h"
#include "gamelogic/QARecord.h"
#include "gamelogic/dragonrecord.h"
#include "live/liverecord.h"
#include "gamelogic/chatrecord.h"
#include "chat/ptcg2m_gmsendchat.h"
#include "network/mslink.h"
#include "common/ptcg2m_firstenterscenentf.h"
#include "common/ptcg2m_enterhallnotice.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/payv2Mgr.h"
#include "live/livemanager.h"
#include "ai/aionlinedebugmgr.h"
#include "pet.h"
#include "gamelogic/titlerecord.h"
#include "gamelogic/firstpassrecord.h"
#include "gamelogic/QAMgr.h"
#include "roleoutlook.h"
#include "gamelogic/ibshoprecord.h"
#include "gamelogic/atlas.h"
#include "gamelogic/spriterecord.h"
#include "scene/scenemortuary.h"
#include "gamelogic/superriskrecord.h"
#include "gamelogic/flowerrecord.h"
#include "gamelogic/FatigueMgr.h"
#include "idip/idiprecord.h"
#include "chat/ptcg2m_reportwatchntf.h"
#include "gamelogic/taskrecord.h"
#include "spactivity/spactivitymgr.h"
#include "spactivity/spactivityrecord.h"
#include "skill/ptcg2c_skillcoolntf.h"
#include "network/rslink.h"
#include "buff/buffrecord.h"
#include "mail/mailsender.h"
#include "table/expeditionconfigmgr.h"
#include "scene/scenewedding.h"
#include "network/controllink.h"
#include "gamelogic/enhanceequipmgr.h"
#include "gamelogic/rolepushinforecord.h"
#include "common/ptcg2c_serveropendayntf.h"
#include "util/gametime.h"
#include "mail/mailconfig.h"
#include "gamelogic/qqviprecord.h"
#include "gamelogic/worldlevelmgr.h"
#include "role/ptcg2c_changenamecountntf.h"
#include "scene/securitystatistics.h"
#include "gamelogic/teamrecord.h"
#include "gamelogic/rolemisc.h"
#include "gamelogic/rolepartner.h"
#include "pay/ptcg2m_payadddiamondntf.h"
#include "mslink.h"
#include "pay/ptcg2m_payconsumentf.h"
#include "gamelogic/herobattlerecord.h"
#include "utility/tlogmgr.h"
#include "gamelogic/herobattlemgr.h"
#include "role/ptcg2c_skillinitcoolntf.h"
#include "gamelogic/reportdatamgr.h"
#include "gamelogic/reportdatarecord.h"
#include "unit/systeminfo.h"
#include "table/OpenSystemMgr.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "gamelogic/military_rank_record.h"
#include "reportdata/reportdata_def.h"
#include "script/scriptmgr.h"
#include "scene/scenedragon.h"
#include "idutil.h"
//#include "custombattle/custombattlerecord.h"
#include "gamelogic/military_rank_manager.h"
#include "gamelogic/dragonconfig.h"
#include "guild/guildcampparty.h"
#include "gamelogic/abysspartymgr.h"
#include "scene/scenemorphinfo.h"
#include "gamelogic/platformshare_record.h"
#include "gamelogic/weekend4v4record.h"
#include "table/PushTableConfig.h"
#include "tablemgr/backflowtablemgr.h"
#include "config.h"
#include "chat/ptcg2c_chatnotifyg2c.h"
#include "scene/scenevsbase.h"
#include "login/ptcg2m_roleloginreconnectntf.h"
#include "gamelogic/tajiehelprecord.h"
#include "servermgr/servercrossdatamgr.h"
#include "gamelogic/dragongrouprecord.h"
#include "component/XAIComponent.h"
#include "define/spactivityiddef.h"
#include "common/ptcg2c_gserrornotify.h"
#include "spactivity/ptcg2c_backflowopenntf.h"
#include "dragonguild/dragonguildrecord.h"
#include "dragonguild/dragonguildmgr.h"
#include "transform/ptcg2c_transnotify.h"
#include "scene/ptcg2c_unitappear.h"
#include "gamelogic/battlefieldrecord.h"
#include "npcfeeling/npcfeelingrecord.h"
#include "gamelogic/competedragonrecord.h"
#include "spactivity/spactivitydatasmall.h"
#include "gamelogic/goalaward.h"
#include "common/ptcg2c_worldlevelntf2client.h"


const static bool sStateCanChange[KKSG::OutLookStateType_ARRAYSIZE][KKSG::OutLookStateType_ARRAYSIZE] =
{
	/*         Normal   Sit   Dance  RidePet  Inherit  Fish RideCopilot Trans*/
	/*Normal*/ {  1,     1,     1,      1,      1,      1,		1,		1},
	   /*Sit*/ {  1,	 1,     0,      0,      0,      1,  	0,		0},
	 /*Dance*/ {  1,	 1,     1,      1,      1,      1,  	1,		0},
   /*RidePet*/ {  1,	 1,     1,      1,      1,      1,  	0,		0},
   /*Inherit*/ {  1,	 1,     0,      0,      1,      1,  	0,		0},
      /*Fish*/ {  1,	 1,     0,      0,      0,      1,  	0,		0},
/*RideCopilot*/{  1,	 1,     1,      0,      1,      1,  	0,		0},
	/*Trans*/  {  1,	 0,     0,      0,      0,      0,  	0,		1},
};

Role::Role()
{
	m_isgm = false;
	m_sessionID = 0;
	m_pXRole = NULL;
	m_pLoadingScene = NULL;
	m_pRoleInfo = NULL;
	m_exp = 0;
	m_realexp = 0;
	m_maxExp = 0;
	m_pNet = NULL;
	m_campID = 0;
	m_collider = true;
	m_enterSceneTime = 0;
	m_onlinetime = 0;
	m_loginTime = 0;
	m_fightgroup = KKSG::FightRole;
	m_uChatID = 0;

	uSensitiveStatus = 0;
	m_ischangetosamescene = false;

	m_isWatcher = false;
	m_ischangingpro = false;
	m_isbeginchangingpro = false;
	m_nLockTime = 0;

	m_accountNumberLastDay = 0;
	m_lastAccountTime = 0;
	m_qwPrevSecTick = TimeUtil::GetFrameTime();
	m_qwPrevMinTick = TimeUtil::GetFrameTime();
	m_routerConnId = 0;
	m_serverId = 0;
	m_fromServerId = 0;
	m_PayMemberID = 0;
	m_ClientAutoFight = false;
	m_roleview = NULL;
	m_calcAddExpLevel = 0;
	m_securityStatistics = NULL;

	m_offfashionstamp = 0;

	SetLeavingScene(false);
	SetState(KKSG::OutLook_Normal);
	m_useWeddingFashion = false;

	m_tempbackflowmark = false;
	m_accountBackFlow = false;

	m_IsForceExit = false;
}

Role::~Role()
{

}

void Role::InitSerializes()
{
	m_serializes.resize(Length<ROLEINTERFACETYPES>::value);
	Set<Role>(this);
	Set<CRoleSystem>(new CRoleSystem(this));
	Set<StageMgr>(new StageMgr(this));
	Set<Bag>(new Bag(this));
	Set<LotteryRecord>(new LotteryRecord(this));
	Set<CCheckinRecord>(new CCheckinRecord());
	Set<CActivityRecord>(new CActivityRecord(this));
	Set<RoleFashion>(new RoleFashion(this));
	Set<CArenaRecord>(new CArenaRecord());
	Set<PayRecord>(new PayRecord(this));
	Set<BuyRecord>(new BuyRecord(this));
	Set<CRewardRecord>(new CRewardRecord(this));
	Set<CShopRecord>(new CShopRecord(this));
	Set<CFlowerRecord>(new CFlowerRecord(this));
	Set<CGuildRecord>(new CGuildRecord(this));
	Set<CPkRecord>(new CPkRecord(this));
	Set<RoleSkillSet>(GetRoleSkillLevelMgr());
	Set<CombatAttribute>(m_combatAttr);
	Set<CTShowRecord>(new CTShowRecord(this));
	Set<CCampRecord>(new CCampRecord(this));
	Set<MyLoginRecord>(new MyLoginRecord(this));
	Set<MyPvpRecord>(new MyPvpRecord(this));
	Set<CFindBackRecord>(new CFindBackRecord(this));
	Set<CTowerRecord>(new CTowerRecord(this));
	Set<DesignationRecord>(new DesignationRecord(this));
	Set<CLevelSealRecord>(new CLevelSealRecord(this));
	Set<AchieveV2Record>(new AchieveV2Record(this));
	Set<QARecord>( new QARecord(this) );
	Set<CDragonRecord>( new CDragonRecord(this) );
	Set<CLiveRecord>( new CLiveRecord(this) );
	Set<ChatRecord>( new ChatRecord(this) );
	Set<CPayV2Record>(new CPayV2Record(this) );
	Set<PetSys>( new PetSys(this) );
	Set<TitleRecord>(new TitleRecord(this));
	Set<CFirstPassRecord>(new CFirstPassRecord(this));
	Set<IBShopRecord>(new IBShopRecord(this));
	Set<CAtlasSys>(new CAtlasSys(this));
	Set<CSpriteRecord>(new CSpriteRecord(this));
	Set<CSuperRiskRecord>(new CSuperRiskRecord(this));
	Set<CIdipRecord>(new CIdipRecord(this));
	Set<TaskRecord>(new TaskRecord(this));
	Set<SpActivityRecord>(new SpActivityRecord(this));
	Set<BuffRecord>(new BuffRecord(this));
	Set<RolePushInfoRecord>(new RolePushInfoRecord(this));
	Set<QQVipRecord>(new QQVipRecord(this));
	Set<CTeamRecord>(new CTeamRecord(this));
	Set<CHeroBattleRecord>(new CHeroBattleRecord(this));
	Set<CReportDataRecord>(new CReportDataRecord(this));
	Set<MilitaryRankRecord>(new MilitaryRankRecord(this));
	Set<PlatformShareRecord>(new PlatformShareRecord(this));
	Set<RoleMisc>(new RoleMisc(this));
	Set<RolePartner>(new RolePartner(this));
	Set<WeekEnd4v4Record>(new WeekEnd4v4Record(this));
	Set<CTaJieHelpRecord>(new CTaJieHelpRecord(this));
    Set<DragonGroupRecord>(new DragonGroupRecord(this));
	Set<CBattleFieldRecord>(new CBattleFieldRecord(this));
	Set<CNpcFeelingRecord>(new CNpcFeelingRecord(this));
	Set<CCompeteDragonRecord>(new CCompeteDragonRecord(this));
	Set<DragonGuildRecord>(new DragonGuildRecord(this));
}

void Role::Init(UINT32 uTemplateID, const std::string &name)
{
	m_roleview = new RoleView(this);
	m_skillLevelMgr = new RoleSkillSet(this);
	m_pNet = new RoleNetSender(this);
	m_statistics = new CombatStatistics();
	m_statistics->SetUnit(this);
	m_pRoleInfo = new KKSG::RoleAllInfo();
	m_pRoleInfo->mutable_brief()->set_name(name);
	SetProfession(uTemplateID);

	InitSerializes();

	m_uTemplateID = uTemplateID;
	m_uPresentID = XEntityInfoLibrary::Instance()->TemplateID2PresentID_Role(m_uTemplateID);
	auto PresentData = XEntityInfoLibrary::Instance()->PresentID2Data(m_uPresentID);
	float scale = PresentData->Scale;
	m_boundRadius = PresentData->BoundRadius * scale;
	m_boundHeight = PresentData->BoundHeight * scale;

	m_pXRole = new XRole(this);
	m_pObject = m_pXRole;
	m_pXRole->Initilize();
	m_pXRole->OnCreated();

	SSInfo<<"create robot:"<<GetID()<<" name:"<<name<<END;
}

void Role::SetTemplateData()
{
	m_uTemplateID = m_pRoleInfo->brief().type();
	m_uPresentID = XEntityInfoLibrary::Instance()->TemplateID2PresentID_Role(m_uTemplateID);

	auto PresentData = XEntityInfoLibrary::Instance()->PresentID2Data(m_uPresentID);
	float scale = PresentData->Scale;
	m_boundRadius = PresentData->BoundRadius * scale;
	m_boundHeight = PresentData->BoundHeight * scale;
}

void Role::Init(const KKSG::RoleAllInfo &roleAllInfo, SessionType sessionID, int index, const EnterSceneInfo& info)
{
	m_roleview = new RoleView(this);
	m_skillLevelMgr = new RoleSkillSet(this);
	m_pNet = new RoleNetSender(this);
	m_statistics = new CombatStatistics();
	m_statistics->SetUnit(this);
	m_sessionID = sessionID;
	m_entersceneinfo = info;
	m_serverId = GetServerIDByRoleID(roleAllInfo.brief().roleid());
	m_fromServerId = ::GetServerIdBySession(sessionID);

	SetFlag(RSF_ISWaitLogin, true);
	SetFlag(RSF_ISConnected, true);

	m_pRoleInfo = new KKSG::RoleAllInfo(roleAllInfo);

	if (!info.isenterhall)
	{
		m_securityStatistics = new XSecurityStatistics(this);
	}

	InitSerializes();

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	LoopCall(m_serializes, &RoleSerializeInterface::Load, m_pRoleInfo);
	UINT64 qwEnd = TimeUtil::GetMilliSecond();

	SetTemplateData();

	m_skillLevelMgr->RefreshSelfLinkedLevels(this);

	m_pXRole = new XRole(this);
	m_pObject = m_pXRole;
	m_pXRole->Initilize();
	m_pXRole->OnCreated();

	m_Timer.SetRole(this);
	m_Timer.Start();

	m_FashionTimer.SetRole(this);
	m_FashionTimer.Start();

	// reset false after construct XRole
	m_entersceneinfo.Reset();

	// 军衔
	MilitaryRankManager::Instance()->UpDateMilitaryRankMS(this);
	//m_pRoleInfo->mutable_military()->set_military_rank(Get<MilitaryRankRecord>()->GetMilitaryRank());

	SSInfo<<"create role:"<<GetID()<<" name:"<<GetName()<<" size:"<<m_pRoleInfo->ByteSize()<<" load_time:"<<(qwEnd - qwBegin)<<END;
}

void Role::Uninit()
{
	SSInfo<<"destroy role:"<<GetID()<<" name:"<<GetName()<<END;

	m_Timer.Stop();
	m_FashionTimer.Stop();

	delete m_roleview;

	m_pXRole->Uninitilize();
	delete m_pXRole;
	delete m_pRoleInfo;
	delete m_pNet;
	delete m_statistics;

	if (m_securityStatistics)
	{
		delete m_securityStatistics;
		m_securityStatistics = NULL;
	}

	foreach (i in m_serializes)
	{
		if ((*i)->AutoDestory())
		{
			delete (*i);
		}
	}
	m_serializes.clear();
}

SessionType Role::GetSessionID() const
{
	return m_sessionID;
}

UINT64 Role::GetRoleID() const
{
	return m_pRoleInfo->brief().roleid();
}
const std::string & Role::GetName() const
{
	return m_pRoleInfo->brief().name();
}

const KKSG::RoleAllInfo & Role::GetRoleAllInfo()
{
	return *m_pRoleInfo;
}

bool Role::CanSendTo()
{
	if (GetFlag(RSF_ISLoadingScene))
	{
		return false;
	}

	// 首次初始化数据
	if (GetFlag(RSF_ISFirstInit))
	{
		return false;
	}

	// 断线重连中
	if (GetFlag(RSF_ISReconnecting))
	{
		return false;
	}

	if (!GetFlag(RSF_ISConnected))
	{
		return false;
	}
	return true;
}

void Role::Send(const CProtocol &roPtc, bool force)
{
	if (m_sessionID == 0)
	{
		return;
	}

	if (!force && !CanSendTo())
	{
		return;
	}

	roPtc.m_sessionID = m_sessionID;
	if (GSConfig::Instance()->IsCrossGS())
	{
		RSLink::Instance()->Send(m_routerConnId, roPtc);
	}
	else
	{
		UINT32 dwGateLine = GetGateLine(m_sessionID);
		GatetLink::Instance()->SendToLine(dwGateLine, roPtc);
	}

	if (roPtc.m_dwType == PTCG2C_ENTERSCENENTF_TYPE)
	{
		SetFlag(RSF_ISLoadingScene, true);
	}
}

bool Role::SendToMS(const CProtocol& protocol)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		return MSLink::Instance()->SendTo(protocol);
	}
	else
	{
		protocol.m_sessionID = m_sessionID;
		return RSLink::Instance()->Send(m_routerConnId, protocol);
	}
	return false;
}

bool Role::SendToMS(CRpc& rpc)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		return MSLink::Instance()->SendTo(rpc);
	}
	else
	{
		rpc.m_sessionID = m_sessionID;
		return RSLink::Instance()->Send(m_routerConnId, rpc);
	}
	return false;
}

void Role::SendToCtrl(const CProtocol& protocol)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		ControlLink::Instance()->SendTo(protocol);
	}
	else
	{
		protocol.m_sessionID = m_sessionID;
		RSLink::Instance()->Send(m_routerConnId, protocol);
	}
}

void Role::SendToCtrl(CRpc& rpc)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		ControlLink::Instance()->SendTo(rpc);
	}
	else
	{
		rpc.m_sessionID = m_sessionID;
		RSLink::Instance()->Send(m_routerConnId, rpc);
	}
}

void Role::SendToRS(const CProtocol& protocol)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		LogWarn("not cross gs, cant send to rs");
		return;
	}
	RSLink::Instance()->Send(m_routerConnId, protocol);
}

void Role::SendToRS(CRpc& rpc)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		LogWarn("not cross gs, cant send to rs");
		return;
	}
	RSLink::Instance()->Send(m_routerConnId, rpc);
}

std::ostream & operator<<(std::ostream &stream, Role *role)
{
	stream << "{Role id: " << role->GetID() << ", name: " << role->GetName() << ", account: "
		<< role->m_pRoleInfo->brief().accountid() << ", level: " << role->GetLevel() << " " << (role->IsDead() ? " Dead" : " Alive") << "}";
	return stream;
}

int Role::GetLoginSceneID(UINT32 roleNum)
{
	if (m_pRoleInfo->brief().has_completeguidestage())
	{
		// 回流活动开启的关卡
		SpActBackFlowData* backflow = static_cast<SpActBackFlowData*>(Get<SpActivityRecord>()->GetSpActData(SpActivity_BackFlow));
		if (backflow != NULL && !backflow->IsFinishBackFlowScene())
		{
			return GetGlobalConfig().BackFlowActivitySceneID;
		}
		return GetGlobalConfig().InitialReturnSceneID;
	}
	else
	{
		if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
		{
			return GetGlobalConfig().BraveReturnServerSceneID;	
		}
		// 新手关卡
		return GetGlobalConfig().InitialSceneID;
	}
}

void Role::OnLogin()
{
	SSDebug << this <<  " Login!" << END;

	m_loginTime = GameTime::GetTime();
	GetRoleAllInfoPtr()->mutable_extrainfo()->set_lastlogintime((UINT32)m_loginTime);

	LoopCall(m_eventListener, &RoleEventListener::OnLogin, this);

	SetFlag(RSF_ISWaitLogin, false);
	SetFlag(RSF_ISFirstEnterScene, true);

	m_onlinetime = (UINT32)time(NULL);
	Get<MyLoginRecord>()->Update();
	DesignationMgr::Instance()->OnTotalLogin(this);
	AchieveV2Mgr::Instance()->OnTotalLogin(this);
	TssHandler::Instance()->anti_add_user(this);
	Get<CTeamRecord>()->RecoverTeamCostInfo();
	SpActivityMgr::Instance()->UpdateSpActivityData(this);

	//首次登陆时强制保存一次数据
	if(GetRoleAllInfoPtr()->extrainfo().logintimes() == 1)
	{
		SetFlag(RSF_ISAccountBriefChanged, true);
		m_pNet->SendRoleDataToDB(0, true);
	}
}

void Role::OnLogout(const char *msg, UINT32 dwParam)
{
	SSDebug << this <<  " Logout!" << END;

	CScriptMgr::Instance()->CallGlobalFunc(NULL, "OnBeforeRoleLogout", "u", this, "Role");
	SetFlag(RSF_ISConnected, false);

	if (m_currScene)
	{
		m_currScene->RemoveUnitAndNotify(this);
		m_currScene = NULL;
	}

	if (m_pLoadingScene)
	{
		m_pLoadingScene->RemoveWaitingRole(this);
		m_pLoadingScene = NULL;
	}

	LoopCall(m_eventListener, &RoleEventListener::OnLogout, this, msg);

	UpdateOnlineTime();
	GetRoleAllInfoPtr()->mutable_extrainfo()->set_lastlogouttime(GameTime::GetTime());
	Get<CSuperRiskRecord>()->OnLogout();

	// logout 的时候更新账号里面的角色信息
	SetFlag(RSF_ISAccountBriefChanged, true);
	m_pNet->SendRoleDataToDB(dwParam, true);

	DesignationMgr::Instance()->OnTotalLogin(this);
	AchieveV2Mgr::Instance()->OnTotalLogin(this);
	TssHandler::Instance()->anti_del_user(this);
}

bool Role::OnFirstInit(UINT32 roleNum)
{
	UINT32 dwTimes = m_pRoleInfo->extrainfo().logintimes();
	++dwTimes;
	m_pRoleInfo->mutable_extrainfo()->set_logintimes(dwTimes);
	if(dwTimes > 1)
	{
		return false;
	}

	// INIT Role Brief
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(GetGlobalConfig().InitialReturnSceneID);
	m_pRoleInfo->mutable_brief()->set_sceneid(GetGlobalConfig().InitialReturnSceneID);
	m_pRoleInfo->mutable_brief()->mutable_position()->set_x(pSceneInfo->m_pConf->StartPos[0][0]);
	m_pRoleInfo->mutable_brief()->mutable_position()->set_y(pSceneInfo->m_pConf->StartPos[0][1]);
	m_pRoleInfo->mutable_brief()->mutable_position()->set_z(pSceneInfo->m_pConf->StartPos[0][2]);
	m_pRoleInfo->mutable_brief()->set_face(pSceneInfo->m_pConf->StartRot[1]);
	m_PosKeeper.InitFromBrief(m_pRoleInfo->brief());

	// INIT Attribute
	CalcRoleAttribute InitRoleAttribute;
	CombatAttribute* pComAttr = Get<CombatAttribute>();
	InitRoleAttribute.SetupBasicAttribute(1, GetProfession(), pComAttr);
	InitRoleAttribute.CalcAttributeConvert(pComAttr, GetProfession());
	InitRoleAttribute.CalcPowerPoint(pComAttr, 0, NULL, GetProfession());

	m_pRoleInfo->mutable_brief()->set_inittime((UINT32)time(NULL));
	m_pRoleInfo->mutable_extrainfo()->set_lastleveluptime(TimeUtil::GetTime());
	m_pRoleInfo->mutable_extrainfo()->set_lastfatiguerecovertime(TimeUtil::GetTime());

	// 推送配置
	const std::map<UINT32, std::string>& pushCfg = PushTableConfig::Instance()->GetTypeKey();
	for (auto it = pushCfg.begin(); it != pushCfg.end(); ++it)
	{
		m_pRoleInfo->mutable_config()->add_type(it->second);
		m_pRoleInfo->mutable_config()->add_value("1");
	}

	// 账号回流礼包
	if (IsAccountBackFlow() && roleNum == 1)
	{
		const MailConf* pMail = MailConfig::Instance()->GetMailConf(MailConf_AccountBackFlow);
		if (pMail)
		{
			MailSender sender;
			sender.SetContent(Mail_System, pMail->m_title, pMail->m_content);
			sender.SetReason(ItemFlow_Mail_Give);
			sender.AddConfigItem(pMail);
			sender.Send(GetID());
		}
	}

	LoopCall(m_serializes, &RoleSerializeInterface::FirstInit, roleNum);
	// 特殊调用一下
	OnLevelUp();
	OpenSystemMgr::Instance()->OnRoleFirstEnterScene(this);
	return true;
}

void Role::OnEnterGs()
{
	SSDebug << this <<  " Enter gs_" << GSConfig::Instance()->GetLine() << END;
	LoopCall(m_eventListener, &RoleEventListener::OnEnterGs, this);

	SetFlag(RSF_ISWaitLogin, false);
	m_onlinetime = (UINT32)time(NULL);

	SpActivityMgr::Instance()->UpdateSpActivityData(this);
}

void Role::OnLeaveGs(UINT32 dwParam)
{
	SSDebug << this <<  " Leave gs_" << GSConfig::Instance()->GetLine() << END;

	if (m_currScene)
	{
		m_currScene->RemoveUnitAndNotify(this);
		m_currScene = NULL;
	}
	if (m_pLoadingScene)
	{
		m_pLoadingScene->RemoveWaitingRole(this);
		m_pLoadingScene = NULL;
	}

	LoopCall(m_eventListener, &RoleEventListener::OnLeaveGs, this);

	UpdateOnlineTime();

	// 离开的时候更新账号里面的角色信息
	SetFlag(RSF_ISAccountBriefChanged, true);
	m_pNet->SendRoleDataToDB(dwParam);

	SetFlag(RSF_ISConnected, false);
}

void SetFashionAppear(XItem *pItem, int pos, void *parg)
{
	KKSG::UnitAppearance *pAppearance = (KKSG::UnitAppearance *)parg;
	if (pItem != NULL)
	{
		pAppearance->add_fashion(pItem->itemID);
	}
	else
	{
		pAppearance->add_fashion(0);
	}
}

void Role::GetOutLook(KKSG::OutLook& outlook)
{
	// guild
	RoleOutLook::FillGuild(&outlook, this);
	// designation
	RoleOutLook::FillDesignation(&outlook, this);
	// equip
	RoleOutLook::FillEquip(&outlook, this, NULL);
	// title
	RoleOutLook::FillTitle(&outlook, this);
	// op
	RoleOutLook::FillOp(&outlook, this);
	// sprite
	RoleOutLook::FillSprite(&outlook, this);
	// state
	RoleOutLook::FillState(&outlook, this);
	// military
	RoleOutLook::FillMilitaryRank(&outlook, this);
	// display
	if (m_useWeddingFashion == false)
	{
		RoleOutLook::FillDisplayFashion(&outlook,this);
	}
	else
	{
		RoleOutLook::FillWeddingDisplayFash(&outlook, this);
	}
	RoleOutLook::FillPayScore(&outlook, this);
}

void Role::GetAppearance(KKSG::UnitAppearance &Appearance, bool isbrief)
{
	Unit::GetAppearance(Appearance, isbrief);
	GetOutLook(*Appearance.mutable_outlook());

	Appearance.set_category(KKSG::Category_Role);
	Appearance.set_nickid(GetNickId());
	Appearance.set_unitname(m_pRoleInfo->brief().name());
	Appearance.set_unittype(m_pRoleInfo->brief().type());
	Appearance.set_isservercontrol(true);

	/*
	const FashionBrief &fb = Get<RoleFashion>()->GetFashion();
	fb.SaveToAppearance(&Appearance);
	*/

	if (NULL == m_currScene || (NULL != m_currScene && !SceneConfig::Instance()->IsBattleSceneExcept(m_currScene->GetSceneType())))
	{
		SkillSetSyncToOtherPlayer SyncSkill = Get<RoleSkillSet>()->GetSyncSkill();
		SyncSkill.SaveToUnitAppearance(&Appearance);
	}
}

void Role::AddListener(RoleEventListener *listener)
{
	m_eventListener.push_back(listener);
}

void Role::RemoveListener(RoleEventListener *listener)
{
	m_eventListener.remove(listener);
}

void Role::OnPreEnterScene(Scene *scene)
{
	m_pNet->SendEnterScene(scene);
	m_pLoadingScene = scene;
	m_rewardString = "";
}

void Role::OnLoginReconnect(Scene* scene)
{
	PtcG2M_RoleLoginReconnectNtf ntf;
	ntf.m_Data.set_roleid(GetID());
	SendToMS(ntf);

	BeforeNotifyClientReconnect(scene->GetSceneID());
	AfterNotifyClientReconnect(scene->GetSceneID());

	LoopCall(m_eventListener, &RoleEventListener::OnLoginReconnect, this, scene);
}

void Role::OnFirstTimeEnterScene(Scene *scene)
{
	LoopCall(m_eventListener, &RoleEventListener::OnFirstTimeEnterScene, this, scene);
	// notify ms
	PtcG2M_FirstEnterSceneNtf ntf;
	ntf.m_Data.set_roleid(GetID());
	SendToMS(ntf);
	CScriptMgr::Instance()->CallGlobalFunc(NULL, "OnRoleFirstEnterScene", "u", this, "Role");
}

void Role::OnEnterScene(Scene *scene)
{
	auto Pos = m_PosKeeper.GetEnterScenePosition(scene, this);
	m_pXRole->SetPosition(Pos.position);
	m_pXRole->SetFace(Pos.face);
	m_PosKeeper.SetInitFace(Pos.face);

	UINT64 qwEventTime1 = 0;
	UINT64 qwEventTime2 = 0;
	if (!IsWatcher())
	{
		RecalcuateAttributeAndNotify(RecalAttr_EnterScene);

		EnterSceneResetAttribute();

		///> 登录后第一次进入场景
		if (GetFlag(RSF_ISFirstEnterScene))
		{
			OnFirstTimeEnterScene(scene);
			SetFlag(RSF_ISFirstEnterScene, false);

			RecalcuateAttributeAndNotify(RecalAttr_EnterScene);
		}	

		///> 在主城GS构造Role后
		if (GSConfig::Instance()->IsHallGS())
		{
			UINT64 qwBegin1 = TimeUtil::GetMilliSecond();
			LoopCall(m_eventListener, &RoleEventListener::OnHallGsEnterScene, this, scene);
			UINT64 qwEnd1 = TimeUtil::GetMilliSecond();
			qwEventTime1 = qwEnd1 - qwBegin1;

			// notify ms
			Scene* pScene = GetCurrScene();
			if (pScene != NULL && pScene->GetSceneTemplateID() == HALL_MAP_ID)
			{
				PtcG2M_EnterHallNotice msg;
				msg.m_Data.set_roleid(GetID());
				SendToMS(msg);
			}
		}

		UINT64 qwBegin2 = TimeUtil::GetMilliSecond();
		m_enterSceneTime = time(NULL);
		LoopCall(m_eventListener, &RoleEventListener::OnEnterScene, this, scene);
		UINT64 qwEnd2 = TimeUtil::GetMilliSecond();
		qwEventTime2 = qwEnd2 - qwBegin2;

		/////> 进场景重新计算EffectData
		//Get<Bag>()->ArrangeEffectData();

		///> 场景buff
		const std::vector<BuffDesc>* buffs = SceneConfig::Instance()->GetEnterSceneBuff(scene->GetSceneTemplateID());
		if (NULL != buffs)
		{
			XAddBuffActionArgs args;
			for(auto i = buffs->begin(); i != buffs->end(); ++i)
			{
				args.vecBuffs.push_back(*i);
				AddReviveBuff(*i);
			}
			args.Caster = GetID();
			args.bClientOnly = !scene->IsSyncScene();
			GetXObject()->DispatchAction(&args);
		}

		m_statistics->Reset();
		SetFlag(RSF_ISGiveReward, false);
		SetFlag(RSF_ISSelectChest, false);
		SetFlag(RSF_ISUseSupplement, false);

		// update guild view
		CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(GetID());
		if (NULL != guild)
		{
			for (auto i = guild->getAllMember().begin(); i != guild->getAllMember().end(); ++i)
			{
				m_roleview->Add(i->first, View_Guild);
			}
		}
	}
	DoChangeSceneLog(TXEnterSceneType);

	m_pLoadingScene = NULL;

	///> 进场景下马
	UINT32 petid = Get<PetSys>()->GetFollowConfId();
	if(petid != 0)
	{
		Get<PetSys>()->OPFollow(Get<PetSys>()->GetFollowId(), false);
	}
	else
	{
		SetState(KKSG::OutLook_Normal);
	}
	///> 在下马后处理，里面可能涉及变身
	if (!IsWatcher())
	{
		Get<BuffRecord>()->OnEnterScene(scene);
	}

	SSInfo<<"role "<<this<<" enter scene:"<<scene<<" posx:"<<Pos.position.x<<" posz:"<<Pos.position.z<<" watcher:"<<m_isWatcher<<" enter_hall_time:"<<qwEventTime1<<" enter_scene_time:"<<qwEventTime2<<END;
}

void Role::OnLeaveScene()
{
	SSInfo << "role " << this << " leave scene " << m_currScene << END;

	m_PosKeeper.OnLeaveScene(m_currScene, m_pXRole->GetPosition(), m_pXRole->GetFaceDegree());

	if (m_pLoadingScene)
	{
		m_pLoadingScene->RemoveWaitingRole(this);
		m_pLoadingScene = NULL;
	}

	if (m_IsDead)
	{
		ReviveOnServer();
		ResetHpMp();
	}

	XLeaveSceneEventArgs args;
	m_pXRole->DispatchAction(&args);

	if (!IsWatcher())
	{
		LoopCall(m_eventListener, &RoleEventListener::OnLeaveScene, this, m_currScene);
	}

	DoChangeSceneLog(TXLeaveSceneType);

	m_pNet->SendLeaveScene();

	SetFlag(RSF_ISGiveReward, false);
	SetFlag(RSF_ISSelectChest, false);
}

void Role::Update(float delta)
{
	if (m_currScene && m_currScene->SyncMode() == SERVERMODE)
	{
		Unit::Update(delta);

		m_pXRole->Update(delta);
		m_statistics->Update();
	}

	if (m_currScene && GetFlag(RSF_ISLoginReconnectFinally))
	{
		OnLoginReconnect(m_currScene);
		SetFlag(RSF_ISLoginReconnectFinally, false);
	}

	UINT64 qwTickNow = TimeUtil::GetFrameTime();
	if(qwTickNow > m_qwPrevSecTick && qwTickNow - m_qwPrevSecTick >= 1000)
	{
		UpdatePerSecond();
		m_qwPrevSecTick = qwTickNow;
	}
	if(qwTickNow > m_qwPrevMinTick && qwTickNow - m_qwPrevMinTick >= 1000 * 60)
	{
		UpdatePerMinute();
		m_qwPrevMinTick = qwTickNow;
	}
}

void Role::UpdatePerSecond()
{
	if (GetFlag(RSF_ISRoleDataNeedSave))
	{
		if (!GSConfig::Instance()->IsCrossGS())
		{
			m_pNet->SendRoleDataToDB();
		}
		SetFlag(RSF_ISRoleDataNeedSave, false);
	}

	DesignationMgr::Instance()->UpdateExpireDesignation( this );
	FatigueMgr::Instance()->CheckFatigueRecover(this);
	Get<LotteryRecord>()->UpdatePerSecond();
	PayV2Mgr::Instance()->CheckMemberExpire(this);
	Get<CFindBackRecord>()->SlowUpdate();
	Get<StageMgr>()->Update();
	Get<CTeamRecord>()->Update();

	if (IsWatcher())
	{
		m_watchLiveInfo.Update();
	}

	UINT32 nowtime = (UINT32)time(NULL);
	if ((0 != m_offfashionstamp) && (nowtime >= m_offfashionstamp + 1))
	{
		RecalcuateAttributeAndNotify(RecalAttr_Fashion);
		m_offfashionstamp = 0;
	}

	CheckAntiAddictionHg();
}


void Role::SetLockTime()
{
	 m_nLockTime = TimeUtil::GetTime() + 5;
}

void Role::UpdatePerMinute()
{
	Get<MyLoginRecord>()->Update();
	Get<CPayV2Record>()->Update();
	Get<SpActivityRecord>()->UpdatePerMinute();
	Get<Bag>()->UpdatePerMinute();
	SendServerOpenDay();
}

void Role::SaveDataOnGsShutdown()
{
	m_pNet->SendRoleDataToDB(0, true);
}

std::string Role::GetAIString()
{
	return  GetGlobalConfig().GetConfig("PlayerAutoFight");
}

void Role::GetAIArgs(AIDataArgs &args)
{
	args.mSpeed = GetRunSpeed();
	args.mNormalAttackProb = 1.0f;
	args.mEnterFightRange = 20.0f;
	args.mIsWander = false;
	args.mActionGap = 1.5f;
	args.mAIStartTime = 1.5f;
	args.mIsFixedInCd = false;
	args.mFightTogetherDis = 30.0f;

	args.mRatioLeft = 0.2f;
	args.mRatioRight = 0.2f;
	args.mRatioDistance = 0.2f;
	args.mRatioIdle = 0.2f;
	args.mRatioExp = 0.2f;
	args.mRatioSkill = 0.2f;
}


bool Role::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	Unit::SetID(poRoleAllInfo->brief().roleid());
	m_level = poRoleAllInfo->brief().level();
	m_exp = poRoleAllInfo->brief().exp();
	m_maxExp = PlayerLevelManager::Instance()->GetLevelupExp(m_level);
	m_PosKeeper.InitFromBrief(poRoleAllInfo->brief());
	m_campID = poRoleAllInfo->brief().campid();
	m_accountNumberLastDay = poRoleAllInfo->brief().accountnumberlastday();
	m_lastAccountTime = poRoleAllInfo->brief().lastaccounttime();
	m_PayMemberID = poRoleAllInfo->brief().paymemberid();

	return true;
}

void Role::FirstInit(UINT32 roleNum)
{
	UINT32 initLevel = 1;
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		initLevel = GSConfig::Instance()->GetBackFlowLevel();
		//m_pRoleInfo->mutable_brief()->set_completeguidestage(true);

		BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfig(GetProfession(), GSConfig::Instance()->GetBackFlowLevel());
		if (pData)
		{
			// 新手引导
			for (auto it = pData->Tutorial.begin(); it != pData->Tutorial.end(); ++it)
			{
				UpdateTutorial(*it);
			}
			// 邮件奖励
			for (auto it = pData->MailIDs.begin(); it != pData->MailIDs.end(); ++it)
			{
				const MailConf* pConf = MailConfig::Instance()->GetMailConf(*it);
				if (pConf == NULL)
				{
					continue;
				}
				MailSender sender;
				sender.SetContent(Mail_System, pConf->m_title, pConf->m_content);
				sender.SetReason(ItemFlow_Mail_Give);
				sender.AddConfigItem(pConf);
				sender.Send(GetID());
			}
		}
	}

	m_level = initLevel;
	m_exp = 0;
	m_maxExp = PlayerLevelManager::Instance()->GetLevelupExp(m_level);

	m_pRoleInfo->mutable_brief()->set_offlinetime(0);
}

void Role::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	KKSG::RoleBrief *pBrief = poRoleAllInfo->mutable_brief();
	pBrief->set_level(m_level);
	pBrief->set_exp(m_exp);
	pBrief->set_maxexp(m_maxExp);
	pBrief->set_offlinetime((UINT32)GameTime::GetTime());
	pBrief->set_campid(m_campID);
	pBrief->set_accountnumberlastday(m_accountNumberLastDay);
	pBrief->set_lastaccounttime(m_lastAccountTime);
	pBrief->set_paymemberid(m_PayMemberID);

	m_PosKeeper.SaveToBrief(pBrief);

	roChanged.insert(pBrief);


	KKSG::RoleExtraInfo* pExtraInfo = poRoleAllInfo->mutable_extrainfo();
	roChanged.insert(pExtraInfo);
}

void Role::CheckAddExp()
{
	if (m_exp >= m_maxExp)
	{
		// 能升多少级;
		while (m_level < PlayerLevelManager::Instance()->MaxLevel())
		{
			if (GetLevelSealStatus())
			{
				break;
			}
			PlayerLevelTable::RowData *row = PlayerLevelManager::Instance()->GetNextLevelInfo(m_level);
			if (NULL == row)
			{
				LogError("invalid config level=%d", m_level+1);
				break;
			}
			if (m_exp < (UINT64)row->Exp)
			{
				m_maxExp = (UINT64)row->Exp;
				break;
			}
			m_exp -= row->Exp;
			Levelup();
		}
	}
}
void Role::CheckExp()
{
	if (m_level >= PlayerLevelManager::Instance()->MaxLevel())
	{
		return;
	}
	CheckAddExp();
}

void Role::AddExp(UINT64 exp)
{
	RoleAttrChangeNotify attrchange(this);
	CheckExp();
	DealWithAddExp dealWithAddExp(this, m_level);
	UINT64 realexp = dealWithAddExp.GetRealExp(exp);

	AddRealExp(realexp);
	attrchange.Send();
}

void Role::AddRealExp(UINT64 uExp)
{
	RoleAttrChangeNotify attrchange(this);
	SetFlag(RSF_ISRoleDataNeedSave, true);
	CheckExp();

	m_realexp = uExp;
	m_exp += uExp;
	CheckAddExp();
	attrchange.Send();
}

void Role::DelExp(UINT64 exp)
{
	SetFlag(RSF_ISRoleDataNeedSave, true);
	if (m_exp >= exp)
	{
		m_exp -= exp;
		return ;
	}
	while (m_level > 1 && m_exp < exp)
	{
		PlayerLevelTable::RowData *row  = PlayerLevelManager::Instance()->GetLevelInfo(m_level);
		if (NULL == row)
		{
			LogError("invalid config level=%d", m_level);
			break;
		}
		m_maxExp = row->Exp;
		m_exp += row->Exp;
		if (exp > m_exp)
		{
			exp -= m_exp;
			m_exp = 0;
		}
		else
		{
			m_exp -= exp;
			exp = 0;
		}
		m_level--;
	}
	return ;
}

void Role::SetCalcAddExpLevel(UINT32 level)
{
	m_calcAddExpLevel = level;
}

bool Role::GetLevelSealStatus()
{
	//跨服;
	if (GSConfig::Instance()->IsCrossGS())
	{
		auto pCrossData = ServerCrossDataMgr::Instance()->GetDataByRoleID(GetID());
		if (pCrossData && pCrossData->seallevel())
		{
			return pCrossData->seallevel() <= (UINT32)GetLevel()?true:false;
		}
		SSError << "gs have not cross data roleid = " << GetID() << END;
		return false;
	}

	if (CLevelSealMgr::Instance()->GetLevelSealInfo().m_uLevel)
	{
		return CLevelSealMgr::Instance()->GetLevelSealInfo().m_uLevel <= (UINT32)GetLevel() ? true:false;
	}
	return false;
}

void Role::CheckLevelSealStatus()
{
	CLevelSealRecord* record = Get<CLevelSealRecord>();
	if( NULL == record )
	{
		return ;
	}
	if (GSConfig::Instance()->IsCrossGS() && NULL == ServerCrossDataMgr::Instance()->GetDataByRoleID(GetID()))
	{
		SSError << "no cross data roleid=" << GetID() << END;
		return;
	}
	if (record->GetButtonStatus() == KKSG::BUTTON_STATUS_LOCKED && !GetLevelSealStatus())
	{
		//玩家上一次封印未点击，现在解封了
		record->SetButtonStatus(KKSG::BUTTON_STATUS_UNLOCKED);//解封未点击
	}
	else if (record->GetButtonStatus() == KKSG::BUTTON_STATUS_UNLOCKED && GetLevelSealStatus())
	{
		//玩家上一次解封未点击，现在封印了
		record->SetButtonStatus(KKSG::BUTTON_STATUS_LOCKED);
	}
	if (record->GetLastStatus() && !GetLevelSealStatus())
	{
		//上次被封印，这次没被封印（针对下线的玩家）
		record->SetLastStatus(false);
		record->SetButtonStatus(KKSG::BUTTON_STATUS_UNLOCKED);
	}
	else if (!record->GetLastStatus() && GetLevelSealStatus())
	{
		//上次未被封印，这次被封印
		record->SetLastStatus(true);
		record->SetButtonStatus(KKSG::BUTTON_STATUS_LOCKED);
	}
	return ;
}

void Role::Levelup()
{
	++m_level;
	SetFlag(RSF_ISAccountBriefChanged, true);
	m_maxExp = PlayerLevelManager::Instance()->GetLevelupExp(m_level);
	OnLevelUp();
}

void Role::OnLevelUp()
{
	RecalcuateAttributeAndNotify(RecalAttr_LevelUp);
	LoopCall1(m_eventListener, &RoleEventListener::OnLevelup, this);
	DesignationMgr::Instance()->OnLevelUp(this);
	AchieveV2Mgr::Instance()->OnLevelUp(this);
    GoalAwardsConfig::Instance()->OnLevelUp(this);
	Get<TaskRecord>()->OnLevelUp(m_level - 1, m_level);
	Get<CShopRecord>()->OnLevelUp();

	bool islocked = false;
	if (GSConfig::Instance()->IsCrossGS())
	{
		auto pData = ServerCrossDataMgr::Instance()->GetDataByRoleID(GetID());
		if (pData && pData->seallevel() == (UINT32)m_level)
		{
			islocked = true;
		}
	}
	else if (CLevelSealMgr::Instance()->GetLevelSealInfo().m_uLevel == (UINT32)m_level)
	{
		islocked = true;
	}
	if (islocked)
	{
		Get<CLevelSealRecord>()->SetButtonStatus(KKSG::BUTTON_STATUS_LOCKED);
		Get<CLevelSealRecord>()->SetLastStatus(true);
	}
}

void Role::LevelUpRapidly(UINT32 level)
{
	UINT32 maxlevel = 0;
	if (GSConfig::Instance()->IsCrossGS())
	{
		auto pData = ServerCrossDataMgr::Instance()->GetDataByRoleID(GetID());
		if (pData == NULL)
		{
			SSError << "no cross data,roleid=" << GetID() << END; 
			return;
		}
		maxlevel = pData->seallevel();
	}
	else
	{
		maxlevel = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uLevel;
	}

	if (level > maxlevel) level = maxlevel;
	
	// exp to level up after levelseal
	CheckExp();

	if (m_level >= level || level > PlayerLevelManager::Instance()->MaxLevel())
	{
		return;
	}

	m_exp = 0;

	SetFlag(RSF_ISRoleDataNeedSave, true);
	RoleAttrChangeNotify attrchange(this);
	UINT32 levelDiff = level - m_level;
	for (UINT32 i = 0; i < levelDiff; ++i)
	{
		Levelup();
	}
	attrchange.Send();

	BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfig(GetProfession(), m_level);
	if (pData)
	{
		LogInfo("role[%llu] prof[%u] level[%u] finish backflow task and stage", GetID(), GetProfession(), m_level);
		Get<TaskRecord>()->FinishBackFlowTask(pData);
		Get<StageMgr>()->FinishBackFlowStage(pData);
	}
	else
	{
		LogError("role[%llu] prof[%u] level[%u] has no backflow config", GetID(), GetProfession(), m_level);
	}
}

void Role::UpdateTutorial(UINT32 tutid)
{
	//LogInfo("update role tutorial %d", tutid);
	if (tutid >= 2048)
	{
		LogError("tutorial id %d out of range 2048!", tutid);
		return;
	}
	else
	{
		UINT64 tutbits = m_pRoleInfo->brief().tutorialbits();
		UINT64 mask = 1;
		mask <<= tutid;
		tutbits |= mask;
		m_pRoleInfo->mutable_brief()->set_tutorialbits(tutbits);

		TGuideFlowLog log(this);
		log.m_nGuideID = tutid;
		log.Do();
	}

	//****** 新的代码 ******
	UINT32 dwNum = (tutid - 1) / 8 + 1;
	if(m_pRoleInfo->brief().tutorialbitsarray().size() < dwNum)
	{
		m_pRoleInfo->mutable_brief()->mutable_tutorialbitsarray()->resize(dwNum, 0);
	}

	INT32 nIndex1 = (tutid - 1) / 8;
	INT32 nIndex2 = (tutid - 1) % 8;
	char& rVal = (*m_pRoleInfo->mutable_brief()->mutable_tutorialbitsarray())[nIndex1];
	rVal |= (1 << nIndex2);
}

int Role::GetProfession()
{
	return m_pRoleInfo->brief().type();
}

void Role::SetProfession(int prof)
{
	SetFlag(RSF_ISAccountBriefChanged, true);
	m_pRoleInfo->mutable_brief()->set_type((KKSG::RoleType)prof);
}

int Role::GetSkillPoint()
{
	int levelUpCostType = Get<RoleSkillSet>()->GetSkillPointType();
	return (int)Get<Bag>()->CountVirtualItem(levelUpCostType);
}

void Role::AddSkillPoint(UINT32 type, int n)
{
	Get<Bag>()->IncVirtualItemRaw(type, n);
}

void Role::SetSkillPoint(UINT32 type, int n)
{
	Get<Bag>()->SetVirtualItem(type, n, false, NULL);
}

void Role::SkillCoolDown()
{
	GetXRole()->CoolDownAllSkills();
	PtcG2C_SkillCoolNtf ptc;
	Send(ptc);
}

void Role::SkillCDAtLaunch()
{
    GetXRole()->CoolDownAtLaunch();
	PtcG2C_SkillInitCoolNtf ptc;
	Send(ptc);
}

bool Role::ResetProfession()
{
	int Prof = GetProfession();
	int BasicProf = ProfessionMgr::GetBasicProfession(Prof);
	if (Prof == BasicProf)
	{
		return false;
	}

	DoSetProfession((KKSG::RoleType)BasicProf);
	return true;
}

int Role::ChooseProfession(int Prof)
{
	int ret = ProfessionMgr::CheckProtmote(GetProfession(), Prof, GetLevel());
	if (ret == KKSG::ERR_SUCCESS)
	{
		DoSetProfession(Prof);
	}
	return ret;
}

void Role::DoSetProfession(int prof)
{
	SetProfession(prof);
	LoopCall1(m_eventListener, &RoleEventListener::OnSetProfession, this);
	SetFlag(RSF_ISRoleDataNeedSave, true);
	AchieveV2Mgr::Instance()->OnChangeJob(this);
	EnhanceEquipMgr::Instance()->OnRoleChangeProfession(this);
	///> 转职刷商店
	for (auto i = GetGlobalConfig().ProfessionChangeShop.begin(); i != GetGlobalConfig().ProfessionChangeShop.end(); ++i)
	{
		Get<CShopRecord>()->Refresh(*i, GetLevel(), GetProfession(), false, false, true);
	}
	RecalcuateAttributeAndNotify(RecalAttr_SetProfession);
}

void Role::RecalcuateAttributeAndNotify(RecalAttrReason reason)
{
	UINT32 scenetype = (NULL == m_currScene) ? 0 : m_currScene->GetSceneType();
	if (reason != RecalAttr_EnterScene && SceneConfig::Instance()->IsPVPAttr(scenetype))
	{
		SSWarn<<"can not recalculate attr, reason:"<<reason<<" roleid:"<<GetID()<<" scene type:"<<scenetype<<END;
		return;
	}
	///>重新计算属性
	CombatAttributeSnapshot m_snapshot(m_combatAttr);
	CalcRoleAttribute InitRoleAttribute;
	InitRoleAttribute.Execute(this);
	m_snapshot.NotifyAttributeChange(m_combatAttr, this);

	// LoopCall(m_eventListener, &RoleEventListener::OnChangeAttr, this);

	LoopCall1(m_eventListener, &RoleEventListener::OnChangeAttr, this);

	SSInfo<<"recalculate attr, reason:"<<reason<<" roleid:"<<GetID()<<" scene type:"<<scenetype<<END;
}

RoleSkillSet * Role::GetRoleSkillLevelMgr()
{
	return dynamic_cast<RoleSkillSet *>(m_skillLevelMgr);
}

void Role::SaveToTeamMember(KKSG::TeamMember *pMemberData)
{
	pMemberData->set_memberid(GetID());
	pMemberData->set_name(GetName());
	pMemberData->set_level(m_level);
	pMemberData->set_profession((KKSG::RoleType)GetProfession());
	pMemberData->set_powerpoint((int)GetAttr(TOTAL_POWERPOINT));
	if (m_currScene)
	{
		pMemberData->set_sceneid(m_currScene->GetSceneTemplateID());
	}
	else
	{
		pMemberData->set_sceneid(1);
	}

	const FashionBrief &fashion = Get<RoleFashion>()->GetFashion();
	for (UINT32 i = 0; i < fashion.Size(); ++i)
	{
		pMemberData->add_fashion(fashion.GetFashionPos(i));
	}
	GetOutLook(*pMemberData->mutable_outlook());
}

UINT64 Role::GetCreateTime()
{
	if (NULL == m_pRoleInfo)
	{
		return 0;
	}
	return m_pRoleInfo->brief().inittime();
}

void Role::UpdateOnlineTime()
{
	UINT32 nowtime = (UINT32)GameTime::GetTime();
	if (nowtime > m_onlinetime)
	{
		if (!GetFlag(RSF_ISReconnecting))
		{
			UINT32 lasttime = m_pRoleInfo->mutable_brief()->onlimetime();
			m_pRoleInfo->mutable_brief()->set_onlimetime(lasttime + nowtime - m_onlinetime);
			Get<CReportDataRecord>()->AddOnlineTime(nowtime - m_onlinetime);
		}
		m_onlinetime = nowtime;
	}
}

UINT32 Role::GetOnlineTimeFromLogin()
{
	UINT32 dwTimeNow   = GameTime::GetTime();
	UINT32 dwTimeLogin = GetRoleAllInfo().extrainfo().lastlogintime();
	return dwTimeNow > dwTimeLogin ? dwTimeNow - dwTimeLogin : 0;
}

void Role::HintNotify(UINT32 systemid, bool isremove)
{
	if (!isremove && !Get<CRoleSystem>()->IsSystemOpened(systemid))
	{
		return;
	}
	PtcG2C_HintNotify ntf;
	ntf.m_Data.add_systemid(systemid);
	ntf.m_Data.set_isremove(isremove);
	Send(ntf);
}

void Role::ResetFightGroup()
{
	m_fightgroup = KKSG::FightRole;
}

bool Role::SetConfig(const std::string& type, const std::string& value)
{
	KKSG::RoleConfig* config = m_pRoleInfo->mutable_config();
	if (config->type_size() != config->value_size())
	{
		return false;
	}
	if (config->type_size() >= 100)
	{
		return false;
	}
	for (int i = 0; i < config->type_size(); ++i)
	{
		if (config->type(i) == type)
		{
			config->set_value(i, value);
			return true;
		}
	}
	config->add_type(type);
	config->add_value(value);
	return true;
}

void Role::DelConfig(const std::string& type)
{
	KKSG::RoleConfig* config = m_pRoleInfo->mutable_config();
	for (int i = 0; i < config->type_size(); ++i)
	{
		if (config->type(i) == type)
		{
			*config->mutable_type(i) = "";
			*config->mutable_value(i) = "";
			break;
		}
	}
	ReConfig();
}

void Role::ReConfig()
{
	std::map<std::string, std::string> type2value;
	KKSG::RoleConfig* config = m_pRoleInfo->mutable_config();
	for (int i = 0; i < config->type_size(); ++i)
	{
		if (!config->type(i).empty())
		{
			type2value[config->type(i)] = config->value(i);
		}
	}
	config->Clear();
	for (auto j = type2value.begin(); j != type2value.end(); ++j)
	{
		config->add_type(j->first);
		config->add_value(j->second);
	}
}

const std::string Role::GetConfig(const std::string& type) const
{
	KKSG::RoleConfig roInfo = m_pRoleInfo->config();
	if (roInfo.type_size() != roInfo.value_size())
	{
		return "";
	}
	for (INT32 i = 0; i < roInfo.type_size(); ++ i)
	{
		if (roInfo.type(i) == type)
		{
			return roInfo.value(i);
		}
	}
	return "";
}
void Role::SetFlag(RoleStateFlag e, bool flag)
{
	m_flags.set(e, flag);
}

bool Role::GetFlag(RoleStateFlag e)
{
	return m_flags[e];
}

void Role::DoAllSave()
{
	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	LoopCall(m_serializes, &RoleSerializeInterface::Save, m_pRoleInfo, m_oFieldChanged);
	UINT64 qwEnd = TimeUtil::GetMilliSecond();

	SSInfo<<"save role:"<<GetID()<<" name:"<<GetName()<<" size:"<<m_pRoleInfo->ByteSize()<<" save_time:"<<(qwEnd - qwBegin)<<END;
}

void Role::DownLevel(int level)
{
	m_exp = 0;
	m_level = level;
	PlayerLevelTable::RowData *row = PlayerLevelManager::Instance()->GetNextLevelInfo(m_level);
	if (row != NULL)
	{
		m_maxExp = row->Exp;
	}
}

void Role::UpdateOnDayPass()
{
	auto record = Get<CFindBackRecord>();
	if (!record)
	{
		return;
	}
	record->OnDayPass();

	CDragonRecord* dragonRecord = Get<CDragonRecord>();
	if (dragonRecord)
	{
		dragonRecord->Update();
	}

	CFlowerRecord* flowerRecord = Get<CFlowerRecord>();
	if (flowerRecord)
	{
		flowerRecord->OnDayPass();
	}

	CGuildRecord* pCGuildRecord = Get<CGuildRecord>();
	pCGuildRecord->Update();

	Get<CSuperRiskRecord>()->OnDayPass();

	Get<MyLoginRecord>()->Update();
	Get<TaskRecord>()->OnDayPass();
	Get<QARecord>()->OnDayPass();

	Get<ChatRecord>()->OnDayPass();
	Get<RoleMisc>()->OnDayPass();
	Get<RolePartner>()->OnDayPass();

	Get<CTaJieHelpRecord>()->OnDayPass();
	Get<CBattleFieldRecord>()->OnDayPass();

	Get<CNpcFeelingRecord>()->Update();

	Get<DragonGuildRecord>()->OnDayPass();

	SendServerOpenDay();
}

void Role::OnClientReconnect(UINT32 sceneid)
{
	BeforeNotifyClientReconnect(sceneid);
	SetFlag(RSF_ISLoadingScene, false);

	DoAllSave();

	PtcG2C_ReconnectSyncNotify oPtc;
	oPtc.m_Data.mutable_scene()->set_sceneid(1);
	*oPtc.m_Data.mutable_self() = *m_pRoleInfo;
	OpenSystemMgr::Instance()->ExceptCloseSystem(this, oPtc.m_Data.mutable_self());
	GetAppearance(*oPtc.m_Data.mutable_selfappearance());

	UINT32 currentid = (NULL == m_currScene) ? 0 : m_currScene->GetSceneTemplateID();
	UINT32 loadingid = (NULL == m_pLoadingScene) ? 0 : m_pLoadingScene->GetSceneTemplateID();
	if (m_currScene)
	{
		oPtc.m_Data.mutable_scene()->set_sceneid(currentid);
		oPtc.m_Data.mutable_scene()->set_isready(m_currScene->IsSceneReady());

		oPtc.m_Data.set_isautofight(m_ClientAutoFight);

		///> 死亡信息
		PackDeathNotfiy(oPtc.m_Data.mutable_deathinfo());

		///> 周围人信息
		std::vector<Role*> roles;
		m_currScene->GetRolesCanSeeSorted(this, roles);
		for (UINT32 i = 0; i < roles.size(); ++i)
		{
			if (roles[i] != this)
			{
				roles[i]->GetAppearance(*oPtc.m_Data.add_units());
			}
		}
		std::vector<Unit*> units;
		m_currScene->GetUnitsCanSee(this, units);
		for (UINT32 i = 0; i < units.size(); ++i)
		{
			units[i]->GetAppearance(*oPtc.m_Data.add_units());
		}

		Send(oPtc);

		///> 服务器和客户端场景不一致，拉客户端进场景
		if (m_currScene->GetSceneTemplateID() != sceneid)
		{
			DoEnterNtf(m_currScene, currentid);
		}
	}
	else if (m_pLoadingScene)
	{
		oPtc.m_Data.mutable_scene()->set_sceneid(loadingid);
		Send(oPtc);

		///> 服务器和客户端场景不一致，拉客户端进场景
		if (loadingid != sceneid || m_ischangetosamescene)
		{
			m_ischangetosamescene = false;
			DoEnterNtf(m_pLoadingScene, loadingid);
		}
	}
	else
	{
		Send(oPtc);
	}

	AfterNotifyClientReconnect(sceneid);

	SSInfo<<"reconnect roleid:"<<GetID()<<" client sceneid:"<<sceneid<<" current scene:"<<currentid<<" loading scene:"<<loadingid<<END;
}

void Role::BeforeNotifyClientReconnect(UINT32 sceneid)
{
	if (IsWatcher())
	{
		LiveManager::Instance()->EnterSceneNotifyWatchData(this, this->GetCurrScene());
	}
	if (GetState().statetype() == KKSG::OutLook_RidePetCopilot)
	{
		SetState(KKSG::OutLook_Normal);

		RoleOutLook outlook(this);
		outlook.Fill(SUMMARY_STATE, NULL);
		outlook.BroadCast();
	}
}

void Role::AfterNotifyClientReconnect(UINT32 sceneid)
{
	if (GetCurrScene())
	{
		GetCurrScene()->OnRoleReconnect(this);
	}

	SpActivityMgr::Instance()->SendActivityOffsetDayPtc(this);
	CGuildCampPartyMgr::Instance()->UpdatePartyStauts(this,sceneid);
}

void Role::RecoverBuff()
{
	if (NULL == m_currScene)
	{
		return;
	}
	bool buffclient = !m_currScene->IsSyncScene();
	XAddBuffActionArgs args;
	///> 恢复buff
	UINT32 nowtime = (UINT32)time(NULL);
	const std::vector<BuffDesc>& recoverbuffs = GetReviveBuff();
	for (auto i = recoverbuffs.begin(); i != recoverbuffs.end(); ++i)
	{
		///> buff有时效性
		if (nowtime < i->effecttime)
        {
			BuffDesc buff = *i;
			if (buff.IsTimeSpecified())
			{
				buff.effecttime = buff.effecttime - nowtime;
			}
		    args.vecBuffs.push_back(buff);
        }
	}
	args.Caster = GetID();
	args.EffectImm = true;
	args.bClientOnly = buffclient;
	GetXObject()->DispatchAction(&args);
}

void Role::OnRevive()
{
	if (NULL == m_currScene)
	{
		return;
	}
	bool buffclient = !m_currScene->IsSyncScene();
	XAddBuffActionArgs args;
	///> 死亡复活buff
	DeathRecord* record = m_currScene->GetMortuary()->GetDeathRecord(GetID());
	if(record && record->isaddbuff)
	{
		const std::vector<BuffDesc>& revivebuffs = m_currScene->GetMortuary()->GetReviveBuffs();
		for (auto i = revivebuffs.begin(); i != revivebuffs.end(); ++i)
		{
			args.vecBuffs.push_back(*i);
		}
	}
	///> 无敌buff
	if ( GetGlobalConfig().ReviveBuff[0] != 0 &&
		 GetGlobalConfig().ReviveBuff[1] != 0)
	{
		args.vecBuffs.push_back(BuffDesc(GetGlobalConfig().ReviveBuff[0], GetGlobalConfig().ReviveBuff[1]));
	}
	args.Caster = GetID();
	args.EffectImm = true;
	args.bClientOnly = buffclient;
	GetXObject()->DispatchAction(&args);

	m_currScene->GetSceneMorphInfo()->OnRevive(this);

	///> 复活恢复之前buff
	RecoverBuff();

	if (NULL != m_currScene)
	{
		m_currScene->OnRoleRevive(this);
	}
}

void Role::SetChatInfo(const KKSG::ChatInfo& chatinfo)
{
	m_achat.insert(std::make_pair(m_uChatID++, chatinfo));
}

//聊天内容发往安全sdk处理后，异步回来后，找到对应的那条信息再发送出去
void Role::SendChatToMS(unsigned char* msg_, unsigned int msg_len_, UINT32 uChatID)
{
	auto iter = m_achat.find(uChatID);
	if (iter != m_achat.end())
	{
		KKSG::ChatInfo& chatinfo = iter->second;
		chatinfo.set_info((char*)msg_);
		ChatMgr::Instance()->FillSourceData(chatinfo.mutable_source());

		switch (chatinfo.channel())
		{
		case KKSG::SpectateChannel:
			{
				Scene* pScene = GetCurrScene();
				if (pScene != NULL)
				{
					PtcG2C_ChatNotifyG2C oPtc;
					oPtc.m_Data.mutable_chatinfo()->CopyFrom(chatinfo);

					const std::unordered_map<UINT64, Role*>& watchers = pScene->GetWatchers();
					for (auto i = watchers.begin(); i != watchers.end(); ++i)
					{
						i->second->Send(oPtc);
					}
				}
				break;
			}
		case KKSG::CurrentChannel:
			{
				Scene* pScene = GetCurrScene();
				if (pScene != NULL)
				{
					std::vector<Role*> watchers;
					///> 最多给100个人
					pScene->GetRolesCanBeSee(this, watchers, 100);
					watchers.push_back(this);

					PtcG2C_ChatNotifyG2C oPtc;
					oPtc.m_Data.mutable_chatinfo()->CopyFrom(chatinfo);
					for (auto i = watchers.begin(); i != watchers.end(); ++i)
					{
						(*i)->Send(oPtc);
					}
				}
				break;
			}
		case KKSG::BattleChannel:
			{
				if (GetCurrScene() != NULL && GetCurrScene()->GetVsHandler() != NULL)
				{
					SceneVsBase* vsHandler = GetCurrScene()->GetVsHandler();;
					vsHandler->FillChatSource(chatinfo.mutable_source());
					std::vector<Role*> sameGroupRole;
					vsHandler->GetSameTeamRole(chatinfo.source().roleid(), sameGroupRole);

					PtcG2C_ChatNotifyG2C oPtc;
					oPtc.m_Data.mutable_chatinfo()->CopyFrom(chatinfo);
					for (auto i = sameGroupRole.begin(); i != sameGroupRole.end(); ++i)
					{
						(*i)->Send(oPtc);
					}
				}
				break;
			}
		default:
			{
				PtcG2M_GMSendChat oSendChatPtc;
				oSendChatPtc.m_Data.mutable_chatinfo()->CopyFrom(chatinfo);
				SendToMS(oSendChatPtc);
				break;
			}
		}

		m_achat.erase(iter);
	}
}

void Role::FillTeamInviteRoleInfo(KKSG::TeamInvRoleInfo* info, int ttype)
{
	if(NULL == info)
	{
		return;
	}
	info->set_userid(GetID());
	info->set_username(GetName());
	info->set_userlevel((UINT32)GetLevel());
	info->set_uservip((UINT32)Get<CPayV2Record>()->GetVipLevel());
	info->set_userpowerpoint((UINT32)GetAttr(TOTAL_POWERPOINT));
	info->set_profession(GetProfession());
	info->set_roleguildid(getGuildId());
	info->set_guildname(GuildSimpleMgr::Instance()->GetGuildName(info->roleguildid()));
	info->set_wanthelp(Get<CTeamRecord>()->IsWantHelp(ttype));

	info->set_roledragonguildid(DragonGuildSimpleMgr::Instance()->GetDragonGuildId(GetID()));
}

bool Role::IsInBattle()
{
	if(NULL == GetCurrScene())
	{
		return false;
	}
	int sceneType = GetCurrScene()->GetSceneType();

	if (sceneType != KKSG::SCENE_HALL &&
		sceneType != KKSG::SCENE_GUILD_HALL &&
		sceneType != KKSG::SCENE_FAMILYGARDEN)
	{
		return true;
	}
	return false;
}

UINT64 Role::getGuildId()
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return m_crossData.guild().gid();//跨服GS使用同步过来的数据
	}
	return GuildSimpleMgr::Instance()->GetGuildId(GetID());
}

UINT32 Role::GetGuildLevel()
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return m_crossData.guild().level();//跨服GS使用同步过来的数据
	}
	auto pGuild = GuildSimpleMgr::Instance()->GetGuildByRoleId(GetID());
	if (pGuild)
	{
		return pGuild->GetGuildLvl();
	}
	return 0;
}

/*
void Role::CheckGuildDareRewardNtf()
{
	CGuildSimple* pGuild = GuildSimpleMgr::Instance()->GetGuildByRoleId(GetID());
	if (pGuild == NULL)
	{
		return;
	}

	RpcG2M_AskGuildDareReward* pMsg = RpcG2M_AskGuildDareReward::CreateRpc();
	pMsg->m_oArg.set_roleid(GetID());
	MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), *pMsg);
}
*/

UINT32 WatchLiveInfo::GetLeftCommendTime()
{
	if (m_commendCount == 0)
	{
		return -1;
	}
	int now = time(0);
	if (now - m_commendTime >=  (int)GetGlobalConfig().WatchCommendTimeSpan)
	{
		m_commendCount = 0;
		return -1;
	}
	return  GetGlobalConfig().WatchCommendTimeSpan - (now - m_commendTime);
}

bool WatchLiveInfo::AddCommendCount(int count)
{
	if (m_commendCount + count >  (int)GetGlobalConfig().WatchCommendTotalCount)
	{
		return false;
	}
	if (m_commendCount == 0)
	{
		m_commendTime = time(0);
	}

	m_commendCount += count;

	return true;
}

void WatchLiveInfo::Update()
{
	if (!m_commendCount)
	{
		return;
	}

	int now = time(0);

	if (now - m_commendTime >=  (int)GetGlobalConfig().WatchCommendTimeSpan)
	{
		m_commendCount = 0;
	}
}

UINT32 Role::GetAccountToday()
{
	if (0 == m_accountNumberLastDay)
	{
		m_lastAccountTime = (UINT64)time(NULL);
		return 0;
	}
	time_t preTime = (time_t)m_lastAccountTime;
	tm* pre = localtime(&preTime);
	int year = pre->tm_year, month = pre->tm_mon, day = pre->tm_mday;
	time_t nowTime = time(NULL);
	tm* now = localtime(&nowTime);
	if(year == now->tm_year && month == now->tm_mon && day == now->tm_mday)
	{
		return m_accountNumberLastDay;
	}
	else
	{
		m_accountNumberLastDay = 0;
		return 0;
	}
}

UINT32 Role::AddOneAccountNow()
{
	GetAccountToday();
	m_lastAccountTime = time(NULL);
	return ++m_accountNumberLastDay;
}

bool Role::IsFamiliar(UINT64 roleID)
{
	if(0 == roleID)
	{
		return false;
	}
	if (IsFriend(roleID))
	{
		return true;
	}

	UINT64 guildOne = GuildSimpleMgr::Instance()->GetGuildId(GetID());
	UINT64 guildTwo = GuildSimpleMgr::Instance()->GetGuildId(roleID);
	if (guildOne != INVALID_GUILDID && guildOne == guildTwo)
	{
		return true;
	}
	return false;
}

void Role::SetAllTuturial()
{
	m_pRoleInfo->mutable_brief()->set_tutorialbits(-1);
	m_pRoleInfo->mutable_brief()->mutable_tutorialbitsarray()->clear();
	m_pRoleInfo->mutable_brief()->mutable_tutorialbitsarray()->resize(100, (char)0xFF);
}

void Role::SetEnterNtf(PtcG2C_EnterSceneNtf& ntf, UINT32 sceneid, UINT32 sceneuid)
{
	m_enterntf.ntf.m_Data = ntf.m_Data;
	m_enterntf.isfill = true;
	m_enterntf.sceneid = sceneid;
	m_enterntf.sceneuid = sceneuid;
}

PtcG2C_EnterSceneNtf* Role::GetEnterNtf(UINT32 sceneid)
{
	if (sceneid != m_enterntf.sceneid)
	{
		return NULL;
	}
	return m_enterntf.isfill ? &m_enterntf.ntf : NULL;
}

bool Role::DoEnterNtf(Scene* scene, UINT32 destid)
{
	PtcG2C_EnterSceneNtf* ntf = GetEnterNtf(scene->GetSceneTemplateID());
	if (NULL != ntf)
	{
		Send(*ntf);
		return true;
	}
	else
	{
		SSWarn<<"empty enter ntf"<<END;
	}
	SSWarn<<"enter scene:"<<destid<<" loading scene:"<<scene->GetSceneTemplateID()<<END;
	return false;
}

void Role::AddFriend(UINT64 friendid)
{
	m_friendlist.insert(friendid);
}

void Role::DelFriend(UINT64 friendid)
{
	auto i = m_friendlist.find(friendid);
	if (i != m_friendlist.end())
	{
		m_friendlist.erase(i);
	}
}

void Role::FillFriendList(const KKSG::FriendList& list)
{
	m_friendlist.clear();
	for (int i = 0; i < list.friends_size(); ++i)
	{
		m_friendlist.insert(list.friends(i));
	}
}

bool Role::IsSystemRewardTaken(KKSG::SystemRewardType type)
{
	CRewardRecord* record = Get<CRewardRecord>();
	SRewardInfo* info = record->GetRewardInfobyId(type);
	return NULL == info ? false : info->isget;
}

void Role::OnAddScenePlayCount(KKSG::SceneType type, int num)
{
	if (type == KKSG::SCENE_DRAGON_EXP)
	{
		Get<CFindBackRecord>()->AddItemUsedCount(KKSG::DRAGONEXP_BACK, 1, num);
	}
}

void Role::OnTakenSystemReward(KKSG::SystemRewardType type)
{
	if (type == KKSG::RewardDinner || type == KKSG::RewardSupper)
	{
		Get<CFindBackRecord>()->AddItemUsedCount(KKSG::FATIGUE_GET, 1, 1);
	}
}
void Role::OnAddTeamCount(int teamType, int num)
{
	if(ABYSS_TEAM_SCENE == teamType)
	{
		Get<CFindBackRecord>()->AddUsedCount(KKSG::EXPBACK_ABYSSS, 1);
		ReportDataMgr::Instance()->Common(this, Tx_Reportdata_AbysssDayTimes, ToString(Get<CTeamRecord>()->GetTeamFinishCountToday(ABYSS_TEAM_SCENE)));
	}
	else if(NEST_TEAM_SCENE == teamType)
	{
		Get<CFindBackRecord>()->AddUsedCount(KKSG::EXPBACK_NEST, 1);
	}
	else if (teamType == TEAM_TOWER)
	{
		Get<CFindBackRecord>()->AddItemUsedCount(KKSG::TOWER, 1, 1);
	}
	else if (teamType == GODDESS_TEAM_SCENE)
	{
		Get<CFindBackRecord>()->AddItemUsedCount(KKSG::NVSHENSHILIAN, 1, 1);
	}
	else if (teamType == ENDLESSABYSS_SCENE)
	{
		Get<CFindBackRecord>()->AddItemUsedCount(KKSG::WUJINSHENYUAN_BACK, 1, 1);
		ReportDataMgr::Instance()->Common(this, Tx_Reportdata_EndlessAbyss, ToString(Get<CTeamRecord>()->GetTeamFinishCountToday(ENDLESSABYSS_SCENE)));
	}
	else if (teamType == GUILDCAMP_TEAM_SCENE)
	{
		Get<CFindBackRecord>()->AddItemUsedCount(KKSG::GUILDACTIVITY, 1, 1);
	}

}

void Role::OnQAFinish()
{
	Get<CFindBackRecord>()->AddItemUsedCount(KKSG::GUILD_VOICE, 1, 1);
}

void Role::OnThroughTower(int floor)
{
	AchieveV2Mgr::Instance()->OnTowerFloor(this, floor);

	Get<CFindBackRecord>()->AddItemUsedCount(KKSG::TOWER, 1, 0);

	ReportDataMgr::Instance()->TowerChange(this, floor);

	auto towerInfo = Get<CTowerRecord>()->GetTowerInfo(DEFAULT_HARDLEVEL);
	if (towerInfo)
	{
		int towerLevel = towerInfo->reachTopFloor > towerInfo->curFloor ? towerInfo->reachTopFloor : towerInfo->curFloor;
		EventMgr::Instance()->AddEvent(GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_TowerFloor, 0, towerLevel);
	}
}

void Role::OnGetActivityChest(int index)
{
	Get<CFindBackRecord>()->AddItemUsedCount(KKSG::DayActiveBack, 1, 1);
}

bool Role::CanReviveLimit()
{
	Scene* scene = GetCurrScene();
	if (NULL == scene)
	{
		return false;
	}
	SceneMortuary *pMortuary = scene->GetMortuary();
	SceneInfo* pSceneInfo = SceneConfig::Instance()->FindSceneInfo(scene->GetSceneTemplateID());
	UINT32 reviveCount = pMortuary->GetReviveLimitCount(GetID());
	return (reviveCount < pSceneInfo->m_pConf->ReviveNumb);
}

bool Role::CanRoleRevive()
{
	///> 场景是否能复活
	if (!CanSceneRevive())
	{
		return false;
	}
	///> 角色身上有没有复活标记，即将自动复活
	if (IsReviving())
	{
		return true;
	}
	///> 场景复活次数是否已满
	if (CanReviveLimit())
	{
		return true;
	}
	return false;
}

bool Role::CanSceneRevive()
{
	if (NULL == m_currScene)
	{
		return false;
	}
	if (KKSG::SCENE_DRAGON == m_currScene->GetSceneType() && NULL != m_currScene->GetSceneDragon())
	{
		return m_currScene->GetSceneDragon()->CanRevive();
	}
	else
	{
		return GetCurrScene()->GetSceneInfo()->m_pConf->CanRevive;
	}
}

bool Role::CheckIdipAndNotify(KKSG::PunishType type)
{
	return Get<CIdipRecord>()->CheckIdipAndNotify(type);
}

void Role::SendServerOpenDay()
{
	if (GSConfig::Instance()->IsCrossGS()) return;

	PtcG2C_ServerOpenDayNtf oPtc;
	oPtc.m_Data.set_openday(GSConfig::Instance()->GetGameServerOpenDays());

	time_t nowTime = GameTime::GetTime();
	oPtc.m_Data.set_nowtime(nowTime);

	time_t openTime = GSConfig::Instance()->GetGameServerOpenTimeStamp();
	UINT32 nDiff = (GameTime::GetWeekBeginTime(nowTime) - GameTime::GetWeekBeginTime(openTime)) / (7*24*3600);
	oPtc.m_Data.set_week(nDiff + 1);

	UINT32 dayBeginSecDiff = nowTime - GameTime::GetDayBeginTime(nowTime, false);
	oPtc.m_Data.set_daybeginsecdiff(dayBeginSecDiff);

	Send(oPtc);
}

void Role::SetStateRaw(KKSG::OutLookStateType stateType, UINT32 param, UINT64 paramother)
{
	if(!StateCanChangeTo(stateType))
	{
		return;
	}
	m_state.set_statetype(stateType);
	m_state.set_param(0);
	m_state.set_paramother(0);
}

void Role::SetState(KKSG::OutLookStateType stateType, UINT32 param, bool isForce, UINT64 paramother)
{
	if(!StateCanChangeTo(stateType))
	{
		return;
	}
	switch(m_state.statetype())
	{
	case KKSG::OutLook_RidePet:
		{
			///> 骑马转换到非骑马状态，都要先下马
			if(stateType != KKSG::OutLook_RidePet)
			{
				///> 下马, 注意这个函数内也会调用SetState, 设置为OutLook_Normal, 小心无限递归, 要保证函数内调用isForce为true，才不会再次进入
				if(!isForce)
				{
					Get<PetSys>()->OPFollow(Get<PetSys>()->GetFollowId());
				}
			}
			break;
		}
	case KKSG::OutLook_RidePetCopilot:
		{
			Get<PetSys>()->ResetPairRide(true);
			break;
		}
	case KKSG::OutLook_Fish:
		{
			if(stateType == KKSG::OutLook_Fish)
			{
				param += m_state.param() / 2 * 2 + 2; ///> 每个阶段是 (0,1) (2,3) (3,4)...
			}
			break;
		}
	case KKSG::OutLook_Trans:
		{
			if (KKSG::OutLook_Trans != stateType)
			{
				Get<BuffRecord>()->DelTransBuff(2);
			}
			break;
		}
	default:
		{
			break;
		}
	}
	m_state.set_statetype(stateType);
	m_state.set_param(param);
	m_state.set_paramother(paramother);
}

bool Role::StateCanChangeTo(KKSG::OutLookStateType nextState)
{
	///> 变大小后，不能骑马
	if (IsSerializesInited() && Get<BuffRecord>()->IsSmalling())
	{
		if (KKSG::OutLook_RidePet == nextState || KKSG::OutLook_RidePetCopilot == nextState)
		{
			LogWarn("role [%llu]'s state error, nextState:%d is invalid when be small or big", GetID(), nextState);
			return false;
		}
	}
	if(!KKSG::OutLookStateType_IsValid(nextState))
	{
		LogError("role [%llu]'s state error, nextState:%d is invalid", GetID(), nextState);
		return false;
	}
	return sStateCanChange[m_state.statetype()][nextState];
}

const std::string& Role::SetBattleStamp()
{
	m_battleStamp = "";
	for(UINT32 i = 0; i < 20; ++i)
	{
		UINT32 random = XRandom::randInt(0, 62);
		char c;
		if(random < 10) c = random + '0';
		else if(random < 36) c = random - 10 + 'a';
		else c = random - 36 + 'A';
		m_battleStamp += c;
	}
	return m_battleStamp;
}

bool Role::IsSameBattleStamp(const std::string& stamp)
{
	return m_battleStamp == stamp;
}

DealWithAddExp::DealWithAddExp(Role* role, UINT32 level)
{
	m_pRole = role;
	m_useLevelSealBuff = true;
	m_useWorldLevelBuff = true;
	if(m_pRole)
	{
		m_pRole->SetCalcAddExpLevel(level);
	}
}

DealWithAddExp::~DealWithAddExp()
{
	if (m_pRole)
	{
		m_pRole->SetCalcAddExpLevel(0);
	}
}

UINT64 DealWithAddExp::GetRealExp(UINT64 exp)
{
	if (NULL == m_pRole || m_pRole->GetLevel() >= PlayerLevelManager::Instance()->MaxLevel())
	{
		return 0;
	}

	UINT32 level = 0;
	if (GSConfig::Instance()->IsCrossGS())
	{
		auto pData = ServerCrossDataMgr::Instance()->GetDataByRoleID(m_pRole->GetID());
		if (pData == NULL)
		{
			SSError << "no cross data,roleid=" << m_pRole->GetID() << ",exp=" << exp << END; 
			return 0;
		}
		level = pData->seallevel();
	}
	else
	{
		level = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uLevel;
	}

	UINT32 calcLevel = m_pRole->GetCalcAddExpLevel();
	double expAdd = PlayerLevelManager::Instance()->GetExpAddition(calcLevel);
	double expBuff = WorldLevelMgr::Instance()->GetExpBuff(m_useWorldLevelBuff ? calcLevel : 0, m_pRole->GetID());
	double backExpBuff = 1.0;
	if (m_pRole->IsBackFlowRole())
	{
		backExpBuff = WorldLevelMgr::Instance()->GetBackExpBuff(calcLevel, m_pRole->GetID());
	}
	float expLevelsealBuff = 1.0;
	if (m_useLevelSealBuff)
	{
		expLevelsealBuff = CLevelSealMgr::Instance()->GetPlayerExtraBuff(calcLevel, m_pRole->GetID()) + 1.0;
	}
	UINT64 uExp  = (UINT64)(1.0 * exp * expAdd * expBuff * backExpBuff * expLevelsealBuff);

	LogInfo("roleid=%llu, exp=%llu, after buf playerlevelbuf=%.2lf,worldbuf=%.2lf,worldbackbuf=%.2lf,,levelsealbuf=%.2f,exp=%llu"
		, m_pRole->GetID(), exp, expAdd, expBuff, backExpBuff, expLevelsealBuff, uExp);

	UINT64 uExtraExp = uExp;
	if (m_pRole->GetLevelSealStatus())
	{
		return UINT64(uExp * GetGlobalConfig().LevelSealExpRate);
	}
	UINT64 mexp = m_pRole->GetExp() +  uExp;
	UINT32 mlevel = m_pRole->GetLevel();
	UINT64 mmaxExp = m_pRole->GetMaxExp();
	if (mexp >= mmaxExp)
	{
		while (mlevel < (UINT32)PlayerLevelManager::Instance()->MaxLevel())
		{
			//if (m_pRole->GetLevelSealStatus())
			if (level && level <= mlevel)
			{
				if (uExp > uExtraExp)
				{
					return uExp - uExtraExp + UINT64(uExtraExp * GetGlobalConfig().LevelSealExpRate);//实际中增加的经验，用于notify前端;
				}
				else
				{
					return UINT64(uExp * GetGlobalConfig().LevelSealExpRate);
				}
			}
			PlayerLevelTable::RowData *row = PlayerLevelManager::Instance()->GetNextLevelInfo(mlevel);
			if (mexp < (UINT64)row->Exp)
			{
				mmaxExp = (UINT64)row->Exp;
				break;
			}
			mexp -= row->Exp;
			uExtraExp = mexp;
			mlevel ++;
			mmaxExp = PlayerLevelManager::Instance()->GetLevelupExp(mlevel);
		}
	}

	return uExp;
}

void DealWithAddExp::SetUseLevelSealBuff(bool use)
{
	m_useLevelSealBuff = use;
}

void DealWithAddExp::SetUseWorldLevelBuff(bool use)
{
	m_useWorldLevelBuff = use;
}

void Role::NoticeNameChange()
{
	PtcG2C_ChangeNameCountNtf ntf;
	ntf.m_Data.set_count(GetRoleAllInfo().brief().changenamecount());
	Send(ntf);
}

void Role::SetClientConf(const KKSG::ClientInfo& roConfInfo)
{
	m_cliConf = roConfInfo;
	//KKSG::RoleExtraInfo* pExtraInfo =  GetRoleAllInfoPtr()->mutable_extrainfo();
	//if (!roConfInfo.loginchannel().empty() && pExtraInfo->fristchannel().empty())
	//	pExtraInfo->set_fristchannel(roConfInfo.loginchannel());
	//UINT32 nTime = GameTime::GetTime();
	//if (pExtraInfo->has_lastlogintime()&&nTime-m_pRoleInfo->extrainfo().lastlogouttime()>=GetGlobalConfig().ChannelMaskDays*24*3600)
	//{
	//	LogInfo("time modify data ModifyChannel old=%s new=%s", pExtraInfo->fristchannel().c_str(),roConfInfo.loginchannel().c_str());
	//	pExtraInfo->set_fristchannel(roConfInfo.loginchannel());
	//	pExtraInfo->set_channelmask(false);
	//}

	//if (pExtraInfo->has_channelmask()&&pExtraInfo->channelmask() == true)
	//{
	//	pExtraInfo->set_channelmask(false);
	//	LogInfo("modify data ModifyChannel old=%s new=%s", pExtraInfo->fristchannel().c_str(),roConfInfo.loginchannel().c_str());
	//	pExtraInfo->set_fristchannel(roConfInfo.loginchannel());
	//}
	Get<RoleMisc>()->SetStartUpType(m_cliConf.starttype());
}

KKSG::StartUpType Role::GetStartUpType()
{
	return Get<RoleMisc>()->GetStartUpType();
}

void Role::SetPicUrl(std::string strPicUrl)
{
	m_pRoleInfo->mutable_idiprecord()->set_picurl(strPicUrl);
}

bool Role::AddDiamond(INT32 iCount, INT32 reason, INT32 subreason, const std::string &param)
{
	if (iCount <= 0)
	{
		return false;
	}

	PtcG2M_PayAddDiamondNtf ptc;
	ptc.m_Data.set_roleid(GetID());
	ptc.m_Data.mutable_data()->set_amt(iCount);
	ptc.m_Data.mutable_data()->set_reason(reason);
	ptc.m_Data.mutable_data()->set_subreason(subreason);
	ptc.m_Data.mutable_data()->set_billno(TTag::Instance()->GetTagString());
	ptc.m_Data.mutable_data()->set_ts(INT32(GameTime::GetTime()));
	ptc.m_Data.mutable_data()->set_extparam(param);
	return SendToMS(ptc);
}
bool Role::ConsumeDiamond(KKSG::PayConsumeBase& info, std::vector<ItemDesc>& item)
{
	if (info.amt() <= 0)
	{
		return false;
	}
	//虚拟货币中的钻石，只用来显示和简单逻辑判断用;
	if ((INT32)Get<Bag>()->CountVirtualItem(DIAMOND) < info.amt())
	{
		return false;
	}
	PtcG2M_PayConsumeNtf ptc;
	ptc.m_Data.set_roleid(GetID());
	KKSG::Payconsumelist oInfo;
	oInfo.set_amt(info.amt());
	oInfo.set_reason(info.reason());
	oInfo.set_subreason(info.subreason());
	oInfo.set_type(info.type());
	oInfo.set_index(info.index());
	oInfo.set_count(info.count());
	oInfo.set_billno(TTag::Instance()->GetTagString());
	oInfo.set_ts(INT32(GameTime::GetTime()));
	for (UINT32 i = 0; i < item.size(); i ++)
	{
		KKSG::ItemBrief* pItem = oInfo.add_item();
		item[i].ToKKSG(pItem);
	}
	ptc.m_Data.mutable_list()->CopyFrom(oInfo);
	return SendToMS(ptc);
}

UINT32 Role::GetSkillCasterTypeID()
{
	return (m_pXRole == NULL || !m_pXRole->IsTransform()) ? 0 : m_pXRole->GetTransformer();
}

bool Role::Transform(UINT32 id, UseHeroType type)
{
	OverWatchConf* conf = HeroBattleMgr::Instance()->GetHeroConf(id);
	if (NULL == conf)
	{
		SSWarn<<"invalid id:"<<id<<END;
		return false;
	}
	if (type >= conf->BuffID.size())
	{
		SSWarn<<"UseHeroType:"<<type<<" >= OverWatch's Buff size, can't transform"<<END;
		return false;
	}

	XAddBuffActionArgs args;
	args.singleBuff.buffID = conf->BuffID[type];
	args.singleBuff.BuffLevel = 1;
	args.Caster = GetID();
	args.EffectImm = true;
	GetXObject()->DispatchAction(&args);

	SSInfo<<"role trans buff:"<<conf->BuffID[type]<<END;

	return true;
}

void Role::FillCrossData(KKSG::RoleCrossData& data)
{
	if (data.roleid() && data.roleid() != GetID())
	{
		SSError << " roleid not match data_roleid = " << data.roleid() << " Role_roleid = " << GetID() << END;
		return;
	}
	if (GSConfig::Instance()->IsCrossGS())
	{
		data.MergeFrom(m_crossData);
	}
	else
	{
		data.set_roleid(GetID());
		auto pGuild = GuildSimpleMgr::Instance()->GetGuildByRoleId(GetID());
		if (pGuild)
		{
			pGuild->FillGuildBrief(data.mutable_guild());
		}
	}
}

void Role::SetCrossData(const KKSG::RoleCrossData& data)
{
	if (data.roleid() != GetID())
	{
		SSError << " roleid not match data_roleid = " << data.roleid() << " Role_roleid = " << GetID() << END;
		return;
	}
	m_crossData = data;
	SSInfo << " cross data roleid = " << m_crossData.roleid() << END;
	const auto& guild = m_crossData.guild();
	SSInfo << " guildid = " << guild.gid() << " guildname = " << guild.name() << " level = " << guild.level() << END;
}

void Role::DoChangeSceneLog(INT32 type)
{
	if (GetCurrScene() == NULL)
	{
		return;
	}
	TSceneFlow oLog(this);
	oLog.m_iSceneType = (INT32)GetCurrScene()->GetSceneType();
	oLog.m_iStatus = type;// TXEnterSceneType;
	oLog.m_iRoleNum = GetCurrScene()->GetRoleCount();
	oLog.m_iWatch = IsWatcher()?1:0;
	UINT32 dwCurrTime = TimeUtil::GetTime();
	if (type == TXLeaveSceneType)
	{
		oLog.m_iTotalTime = ((dwCurrTime >= (UINT32)GetEnterSceneTime() &&GetEnterSceneTime()) ? dwCurrTime - (UINT32)GetEnterSceneTime() : 0);
	}
	oLog.Do();
}

bool Role::IsBackFlowRole()
{
	return Get<SpActivityRecord>()->GetSpActData(SpActivity_BackFlow) != NULL;
}

bool Role::IsBackFlowSmallRole()
{
	return Get<SpActivityRecord>()->GetSpActData(SpActivity_Argenta) != NULL;
}

void Role::OnTakeBackFlow()
{
	///> 当开启回流活动
	Get<BuyRecord>()->GetBackFlow().Reset();
	///> delay notify client use
	m_tempbackflowmark = true;
}

void Role::NotifyBackFlow()
{
	if (m_tempbackflowmark)
	{
		m_tempbackflowmark = false;

		PtcG2C_BackFlowOpenNtf ntf;
		ntf.m_Data.set_isopen(true);
		Send(ntf);
	}
}

UINT32 Role::GetBackFlowEndTime()
{
	if (!IsBackFlowRole())
	{
		return 0;
	}
	UINT32 lastBackFlowStartTime = Get<SpActivityRecord>()->GetLastBackFlowStartTime();
	return lastBackFlowStartTime + GetGlobalConfig().BackFlowActivityDayTime * 24 * 3600;
}

void Role::ErrorCodeNtf(UINT32 errorno, bool istip)
{
	PtcG2C_GSErrorNotify ptc;
	ptc.m_Data.set_errorno(errorno);
	ptc.m_Data.set_istip(istip);
	Send(ptc);
}

void Role::OnOtherAppear(UINT64 otherroleid)
{
	//if (otherroleid == GetID())
	//{
	//	return;
	//}
	//if (NULL == GetCurrScene())
	//{
	//	return;
	//}
	//if (!SceneConfig::Instance()->IsTransScene(GetCurrScene()->GetSceneType()))
	//{
	//	return;
	//}
	//Role* other = GetCurrScene()->IsInScene(otherroleid);
	//if (NULL == other)
	//{
	//	return;
	//}
	//PtcG2C_TransNotify notify;
	//notify.m_Data.set_roleid(ToString(otherroleid));
	//notify.m_Data.set_istrans(other->Get<BuffRecord>()->IsTransforming());
	//Send(notify);
}

void Role::SendWorldLevelNtf()
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return;
	}
	PtcG2C_WorldLevelNtf2Client oPtc;
	oPtc.m_Data.set_worldlevel(WorldLevelMgr::Instance()->GetLevel());
	Send(oPtc);
}

bool Role::IsKingBack()
{
	//if(GetAccount() == "171" || GetAccount() == "173")
	//{
	//	return true;//for test
	//}
	return IsBackFlowRole();
}
