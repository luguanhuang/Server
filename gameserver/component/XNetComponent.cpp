#include "pch.h"
#include "XNetComponent.h"
#include "skill/XSkill.h"
#include "skill/XSkillCore.h"
#include "timeutil.h"
#include "scene/scene.h"
#include "XRotationComponent.h"
#include "skill/ptcg2c_skillresult.h"
#include "action/ptcg2c_correctposition.h"
#include "action/ptcg2c_syncmovenotify.h"
#include "skill/ptcg2c_cdcall.h"
#include "skill/ptcg2c_warningrandomntf.h"
#include "XMoveComponent.h"
#include "XBeHitComponent.h"
#include "XFreezeComponent.h"
#include "XSkillComponent.h"
#include "skill/XSkill.h"
#include "config.h"
#include "LogInit.h"
#include "unit/pet.h"

using namespace KKSG;

XNetComponent::XNetComponent(void)
	:_passive_move(false),
	 _correct_protected(false),
	 _last_sync_pos(Vector3::zero),
	 _last_sync_face(0),
	 _last_sync_sequence(0),
	 _elapsed(0)
{
	_last_passive_move = _passive_move;
}

XNetComponent::~XNetComponent(void)
{

}

void XNetComponent::Update(float fDeltaT)
{
	if(_correct_protected)
	{
		_elapsed += fDeltaT;
		if(_elapsed > 1) _correct_protected = false;
	}
}

bool XNetComponent::Broadcast(const CProtocol& proto, bool pExludeSelf)
{
	Scene* pScene = GetHost()->GetUnit()->GetCurrScene();
	if(pScene != NULL) pScene->Broadcast(GetHost()->GetUnit(), proto, pExludeSelf ? GetHost()->GetUnit() : NULL);

	return pScene != NULL;
}

bool XNetComponent::BroadcastToself(const CProtocol& proto)
{
	GetHost()->GetUnit()->Send(proto);
	return true;
}

bool XNetComponent::BroadcastSkillResult(PtcG2C_SkillResult& oPtc)
{
	Broadcast(oPtc, false);
	return true;
}

bool XNetComponent::BroadcastMoveAction(const PtcG2C_SyncMoveNotify& oPtc)
{
	/*
	 * 主城里将this的移动协议同步给其他玩家
	 * 相关策略在这里实现，不要发给this(自己)
	 */

	///> 广播给可以看到该玩家的人
	Scene* pScene = GetHost()->GetUnit()->GetCurrScene();
	if(pScene != NULL) pScene->BroadCastCanBeSee(GetHost()->GetUnit(), oPtc);

	//Broadcast(oPtc, true);

	return pScene != NULL;
}

bool XNetComponent::BroadcastRandomWarningPos(uint skill, const vector<vector<XSkillCore::XWarningRandomPackage> >& set)
{
	PtcG2C_WarningRandomNtf wr;

	wr.m_Data.set_firer(GetEntity()->GetID());
	wr.m_Data.set_skill(skill);

	for(size_t i = 0; i < set.size(); i++)
	{
		WarningItemSet* pwis = wr.m_Data.add_warningitems();

		auto it = set[i].begin();
		while(it != set[i].end())
		{
			WarningPackage* pwp = pwis->add_warningitem();
			pwp->set_id(it->ID);

			auto is = it->Pos.begin();
			while (is != it->Pos.end())
			{
				pwp->add_warningpos(*is);
				++is;
			}

			++it;
		}
	}

	return Broadcast(wr, false);
}

bool XNetComponent::BroadcastCorrectLocation(const Vector3& pos, float face, bool transfer, bool toall)
{
	PtcG2C_CorrectPosition cp;

	cp.m_Data.set_uid(GetHost()->GetID());

	cp.m_Data.set_face((int)(face * 10));

	cp.m_Data.set_pos_x((int)(pos.x * 100));
	cp.m_Data.set_pos_y((int)(pos.y * 100));
	cp.m_Data.set_pos_z((int)(pos.z * 100));

	cp.m_Data.set_btransfer(transfer);

	_correct_protected = true;
	_elapsed = 0;

	if (toall)
	{
		return Broadcast(cp, false);
	}
	else
	{
		return (transfer || !GetEntity()->GetUnit()->IsRole()) ? Broadcast(cp, false) : BroadcastToself(cp);
	}
}

void XNetComponent::ReportMoveAction(const Vector3& des, bool stoppage, const Vector3& face)
{
	/*
	 * another sync strategy
	 * only send sync protocol in Hall Stage
	 */

	Scene* pScene = GetEntity()->GetUnit()->GetCurrScene();

	if(NULL != pScene && pScene->IsViewGrid())
	{
		//set pos x & z (loc within 500.0)
		int posxz = (int)(floor(des.z * 100 + 0.5f));
		posxz |= ((int)(floor(des.x * 100 + 0.5f)) << 16);

		PtcG2C_SyncMoveNotify smn;
		smn.m_Data.set_entityid(GetEntity()->GetID());
		smn.m_Data.set_posxz(posxz);
		smn.m_Data.set_stoppage(stoppage);
		if(stoppage) smn.m_Data.set_face(int(Vector3::AngleToFloat(face) * 10));

		BroadcastMoveAction(smn);
	}
}

bool XNetComponent::BroadcastCDCall(uint skillid, int runtime, bool onsyntonic)
{
	PtcG2C_CdCall cd;
	cd.m_Data.set_skillid(skillid);
	cd.m_Data.set_leftrunningtime(runtime);
	if(onsyntonic) cd.m_Data.set_onsyntonic(true);

	return BroadcastToself(cd);
}

bool XNetComponent::NeedBroadCast()
{
	if(GetEntity()->StateSynced()) return true;
	
	Role* role = dynamic_cast<Role*>(GetEntity()->GetUnit());
	if(role)
	{
		PetSys* pPetSys = role->Get<PetSys>();
		if(pPetSys && pPetSys->IsCopilot()) return false;
	}

	//if(IsPassive() || (GetEntity()->SyncSequence() != _last_sync_sequence))
	{
		const Vector3& pos = GetEntity()->GetPosition_p();
		float face = GetEntity()->GetFaceDegree();

		bool need = 
			GetEntity()->StateTransfered() || GetEntity()->StateCasted() ||
			GetEntity()->SyncSequence() != _last_sync_sequence || 
			_passive_move != _last_passive_move ||
			pos != _last_sync_pos || 
			face != _last_sync_face;

		_last_sync_pos = pos;
		_last_sync_face = face;
		_last_sync_sequence = GetEntity()->SyncSequence();
		_last_passive_move = _passive_move;

		if(need) GetEntity()->SetStateSynced();

		return need;
	}
	//else
		//return false;
}

void XNetComponent::ResetSyncData()
{
	_sync_data.Clear();
	_sync_data.set_entityid(0);
}

void XNetComponent::PackageSyncData(KKSG::StepSyncData* pdata)
{
	if(!_sync_data.entityid())
	{
		const Vector3& loc = GetEntity()->GetPosition_p();

		//set id
		_sync_data.set_entityid(GetEntity()->GetID());
		//common info
		int common = 0;
		//positive face
		float face = XRoundToCircle(GetEntity()->GetFaceDegree());
		if(face < 0) face += 360;
		//set face
		common = (int)(floor(face + 0.5f));

		//set pos x & z (loc within 500.0)
		int posxz = (int)(floor(loc.z * 100 + 0.5f));
		posxz |= ((int)(floor(loc.x * 100 + 0.5f)) << 16);
		_sync_data.set_posxz(posxz);

		//action state
		int state = 0;

		if(GetEntity()->SkillComponent()->IsCasting())
		{
			XSkill* skill = GetEntity()->SkillComponent()->CurrentSkill();
			XEntity* target = skill->Target();

			_sync_data.set_skillid(skill->MainCore()->ID());
			if(target) _sync_data.set_opposerid(target->GetID());

			//skill common info
			int skill_common = 0;

			skill_common = (skill->Slot() < 0) ? 0xFF : skill->Slot();

			int percentage = GetEntity()->StateCasted() ? 0 : (int)(skill->TimeElapsed() / skill->MainCore()->GetSoul()->Time * 100);
			skill_common |= (percentage << 8);

			int attackspeedratio = (int)(skill->MainCore()->GetTimeScale() * 10);
			skill_common |= (attackspeedratio << 16);

			if(skill->MainCore()->GetSoul()->MultipleAttackSupported)
			{
				int multipledirectionfactor = (int)(skill->MainCore()->GetMultipleDirectionFactor() * 100);
				skill_common |= (multipledirectionfactor << 24);
			}

			_sync_data.set_skillcommon(skill_common);
			state = XState_Skill;
		}
		else
		{
			IXStateTransform* pState = GetEntity()->StateMachine()->GetCurrent();

			switch(pState->SelfState())
			{
			case XState_Move:
				{
					XMoveComponent* pmove = static_cast<XMoveComponent*>(pState);
					_sync_data.set_velocity((int)(floor(pmove->GetSpeed() * 10 + 0.5f)));
				}break;
			case XState_BeHit:
				{
					XBeHitComponent* phit = static_cast<XBeHitComponent*>(pState);

					_sync_data.set_skillid(phit->SkillFrom());
					_sync_data.set_opposerid(phit->HitFrom());
					_sync_data.set_hitidx(phit->ResultIdx());
					_sync_data.set_hitparalyzefactor((int)(phit->ParalyzeFactor() * 100));
					_sync_data.set_hitforcetofly(phit->ForceToFly());
				}break;
			case XState_Freeze:
				{
					XFreezeComponent* pfreeze = static_cast<XFreezeComponent*>(pState);

					_sync_data.set_freezedfromhit(pfreeze->IsFromHit());
					_sync_data.set_opposerid(pfreeze->HitFrom());

					if(pfreeze->HitFrom())
					{
						_sync_data.set_skillid(pfreeze->SkillFrom());
						_sync_data.set_hitidx(pfreeze->ResultIdx());
					}
					else
						_sync_data.set_presentinfreezed(pfreeze->IsPresent());
				}break;
			case XState_Death:
				{

				}break;
			default:
				break;
			}

			state = pState->SelfState();
		}

		//set action state ( < 16 )
		common |= (state << 12);
		//set action sequence
		common |= (GetEntity()->SyncSequence() << 16);
		//set common
		_sync_data.set_common(common);

		_sync_data.set_passive(IsPassive());
	}

	//required
	pdata->set_entityid(_sync_data.entityid());
	pdata->set_posxz(_sync_data.posxz());
	pdata->set_common(_sync_data.common());

	//optional
	if(_sync_data.has_skillid()) pdata->set_skillid(_sync_data.skillid());
	if(_sync_data.has_hitidx()) pdata->set_hitidx(_sync_data.hitidx());
	if(_sync_data.has_opposerid()) pdata->set_opposerid(_sync_data.opposerid());
	if(_sync_data.has_hitforcetofly()) pdata->set_hitforcetofly(_sync_data.hitforcetofly());
	if(_sync_data.has_hitparalyzefactor()) pdata->set_hitparalyzefactor(_sync_data.hitparalyzefactor());
	if(_sync_data.has_presentinfreezed()) pdata->set_presentinfreezed(_sync_data.presentinfreezed());
	if(_sync_data.has_freezedfromhit()) pdata->set_freezedfromhit(_sync_data.freezedfromhit());
	if(_sync_data.has_passive()) pdata->set_passive(_sync_data.passive());
	if(_sync_data.has_velocity()) pdata->set_velocity(_sync_data.velocity());
	if(_sync_data.has_skillcommon()) pdata->set_skillcommon(_sync_data.skillcommon());
}