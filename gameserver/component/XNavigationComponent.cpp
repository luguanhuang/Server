#include "pch.h"
#include "XNavigationComponent.h"
#include "entity/XObject.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "scene/waypoint.h"
#include "math/XVector3.h"


XNavigationComponent::XNavigationComponent()
{
	m_onUse= false;
	m_nextIndex = 0;
	m_destIndex = 0;
	m_nextUpTime = (UINT32)(-1);
}

XNavigationComponent::~XNavigationComponent()
{

}

void XNavigationComponent::SetUseState( bool onUse , const Vector3& destPos/* = Vector3::zero*/)
{
	if(onUse)
	{
		m_nextUpTime = time(NULL);
		m_destPos = destPos;
		InitData();
	}
	else
	{
		m_nextUpTime = (UINT32)(-1);
	}
	m_onUse = onUse;

	if(m_onUse)
	{
		CheckReachThrough();
	}
}

bool XNavigationComponent::CheckMoveDest(const Vector3& destPos)
{
	if (destPos == m_destPos)
		return true;
	else
		return false;
}

void XNavigationComponent::Update( float fDeltaT )
{
	if(m_onUse)
	{
		XObject *obj = GetHost();
		if(!obj)
		{
			return ;
		}
		Unit* unit = obj->GetUnit();
		if(!unit || !unit->GetCurrScene())
		{
			return ;
		}
		WayPoint* wayPoint = unit->GetCurrScene()->GetWayPoint();
		if(!wayPoint)
		{
			return ;
		}

		const Vector3& myPos = obj->GetPosition_p();

		bool reachNextIndex = wayPoint->OnIndexPoint(myPos , m_nextIndex);

		if(time(NULL) <= m_nextUpTime && !reachNextIndex)
		{
			return ;
		}

		Vector3 destPos = myPos;

		if(reachNextIndex)//已到达 需要去下一个点
		{
			if(unit->GetCurrScene()->CheckMoveReach(unit , myPos , m_destPos - myPos))
			{//可以直达
				SetUseState(false);//可以关闭导航了
				destPos = m_destPos;			
			}else
			{
				if(m_nextIndex == m_destIndex)
				{
					if(!m_destPos.IsZero())
					{
						SetUseState(false);
						destPos = m_destPos;
					}
				}else
				{
					m_nextIndex = wayPoint->GetNextIndex(m_nextIndex , m_destIndex);
					if(wayPoint->HavePoint(m_nextIndex))
					{
						destPos = wayPoint->GetPoint(m_nextIndex);
					}
				}
			}
		}else
		{
			if(wayPoint->HavePoint(m_nextIndex))
			{
				destPos = wayPoint->GetPoint(m_nextIndex);
			} 
		}
		
		MoveTo(myPos , destPos);
		m_nextUpTime += 2;
	}
}

void XNavigationComponent::MoveTo( const Vector3& myPos , const Vector3& destPos )
{
	XObject *obj = GetHost();
	if(obj && obj->GetUnit())
	{
		Vector3 dir = destPos - myPos;
		
		if(!obj->GetUnit()->GetCurrScene()->TryMove(obj->GetUnit() , dir))
		{
			SSWarn << myPos.x << " " << myPos.y << " " << myPos.z << END;
			SSWarn << destPos.x << " " << destPos.y << " " << destPos.z << END;
			SSWarn << m_destIndex << END; 
			return;
		}

		XMoveActionArgs args;
		dir.y = 0;
		dir.Normalize();
		args.Speed = obj->GetUnit()->GetRunSpeed();
		args.Destination = destPos;
		args.StopTowards = dir;
		obj->DispatchAction(&args);
	}
} 

void XNavigationComponent::InitData()
{
	if(GetHost() && GetHost()->GetUnit() && GetHost()->GetUnit()->GetCurrScene())
	{
		WayPoint * wayPoint = GetHost()->GetUnit()->GetCurrScene()->GetWayPoint();
		if(wayPoint)
		{
			if(!wayPoint->NavInited())
			{
				wayPoint->CreateNavData(GetHost()->GetUnit());
			}
			m_nextIndex = wayPoint->GetClosePointIndex(GetHost()->GetPosition_p() , GetHost()->GetUnit());
			if(m_destPos.IsZero())
			{
				m_destIndex = wayPoint->GetLastPointIndex();		
				m_destPos = wayPoint->GetPoint(m_destIndex);	
			}else
			{
				m_destIndex = wayPoint->GetClosePointIndex(m_destPos , GetHost()->GetUnit());
			}

			wayPoint->CorrectPathIndex(m_nextIndex , m_destIndex , GetHost()->GetUnit());
		}
	
	}
}

void XNavigationComponent::StopMove()
{
	if(GetHost())
	{
		const Vector3& myPos = GetHost()->GetPosition_p();
		MoveTo(myPos, myPos);
	}
}

//检测直达
void XNavigationComponent::CheckReachThrough()
{
	XObject *obj = GetHost();
	if(!obj)
	{
		return ;
	}
	Unit* unit = obj->GetUnit();
	if(!unit || !unit->GetCurrScene())
	{
		return ;
	}
	
	const Vector3 &myPos = obj->GetPosition_p();
	if(unit->GetCurrScene()->CheckMoveReach(unit , myPos , m_destPos - myPos))
	{
		MoveTo(myPos , m_destPos);
		SetUseState(false);
	}
}
