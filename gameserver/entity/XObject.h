#ifndef __XOBJECT_H__
#define __XOBJECT_H__

#include "XActionDefine.h"
#include "unit/unit.h"
#include "physics/XPhysics.h"
#include "component/XComponentDefine.hpp"

using namespace std;

class XComponent;
class Scene;
class Unit;
class XAIComponent;
class XStateMachine;
class XSkillComponent;
class XNetComponent;
class XBuffComponent;
class XPresentComponent;
class XRotationComponent;
class XBeHitComponent;
class XQuickTimeEventComponent;
class XNavigationComponent;
class XFlyComponent;

class XObject
{
	friend class Unit;
	friend class XRotationComponent;

protected:
	XObject(Unit* pUnit);
	virtual ~XObject();

	virtual bool Initilize();
	virtual void Uninitilize();

public:
	virtual void OnCreated();

	virtual void OnEnterScene(Scene *scene);
	virtual void OnSceneReady();
	virtual void OnLeaveScene();

	inline UINT64 GetID() const { return _pSoul->GetID(); }
	inline Unit* GetUnit() const { return _pSoul; }

	virtual void Update(float fDeltaT);

	inline XComponent* GetXComponent(UINT32 uuid) const 
	{
		return (uuid < XComponent_Max) ? _components_idx[uuid] : NULL;
	}

	XComponent* AttachComponent(XComponent* pComponentObject);
	void DetachComponent(UINT32 uuid);

	void AttachAction(XActionDefine action, UINT32 componentid);
	void DetachAction(XActionDefine action, UINT32 componentid);

	HTIMER DelayAction(IArgs* pargs, float delay);
	bool DispatchAction(XActionArgs* args);

	virtual void Idled(){ }
	void TransferLocation(const Vector3& pos, float face);
	void CorrectLocation();
	void CorrectLocation(const Vector3& pos, float face, bool toall);

	virtual bool Has_H_Movement(){ return false; }
	/*
	 * set with position check
	 */
	void SetPosition_s(const Vector3& pos);
	/*
	 * set directly
	 */
	inline void SetPosition(const Vector3& pos){ _position = pos; }
	inline void SetPosition(float x, float y, float z)
	{ 
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	inline void SetPositionY(float y)
	{ 
		_position.y = y;
	}

	Vector3 GetHistoryPosition(float backdating) const;
	float GetHistoryDirecton(float backdating) const;

	inline bool IsMobShieldable() const { return _mob_shieldable; }
	inline void MobShieldable(){ _mob_shieldable = true; }

	inline Vector3 GetPosition() const { return _position; }
	/*
	 * get faster
	 */
	inline const Vector3& GetPosition_p() const { return _position; }
	/*
	 *face angle according to Vector3::forward
	 *anti-clockwise is negative
	 */
	void SetFace(float degree);
	void SetFace(const Vector3& face);
	inline const Vector3& GetFace() const { return _face; }
	inline Vector3 GetBack() const { return -_face; }
	//range: 0 ~ XRound
	inline float GetFaceDegree() const { return _face_degree; }
	/*
	 * get faster
	 */
	inline const Vector3& GetFace_p() const { return _face; }

	inline XStateMachine* StateMachine() const { return _pmachine; }
	inline XBeHitComponent* HitComponent() const { return _phit; }
	inline XSkillComponent* SkillComponent() const { return _pskill; }
	inline XNetComponent* NetComponent() const { return _pnet; }
	inline XBuffComponent* BuffComponent() const { return _pbuff; }
	inline XPresentComponent* PresentComponent() const { return _ppresent; }
	inline XRotationComponent* RotateComponent() const { return _protate; }
	inline XQuickTimeEventComponent* QuickTimeEventComponent() const { return _pqte; }
	inline XAIComponent* AIComponent() const {return _ai;}
	inline XNavigationComponent* NavigationComponent() const { return _pnavigation; }
	inline XFlyComponent* Fly() const { return _pfly; }

	inline void SetAIComponent(XAIComponent* pAIComp) { _ai = pAIComp;}
protected:
	bool OnAction(IArgs* pargs, void *);

	void InnerSetFace(float degree);
	void InnerSetFace(const Vector3& face);

protected:
	/*
	 * unit
	 */
	Unit* _pSoul;

	XStateMachine* _pmachine;
	XSkillComponent* _pskill;
	XBuffComponent*  _pbuff;
	XNetComponent* _pnet;
	XPresentComponent* _ppresent;
	XRotationComponent* _protate;
	XBeHitComponent* _phit;
	XQuickTimeEventComponent* _pqte;
	XAIComponent* _ai;
	XNavigationComponent* _pnavigation;
	XFlyComponent* _pfly;

private:
	Vector3 _position;
	/*
	 * clockwise to Vector3::forward
	 * anti-clockwise is negative
	 * range: 0 ~ XRound
	 */
	float _face_degree;
	Vector3 _face;
	/**/
	bool _mob_shieldable;
	/*
	 * first-> ActionType
	 * second-> Component list
	 */
	UINT32 _actions[XAction_Max][XComponent_Max];

	XComponent* _components[XComponent_Max];
	XComponent* _components_idx[XComponent_Max];
};

#endif // __XOBJECT_H__