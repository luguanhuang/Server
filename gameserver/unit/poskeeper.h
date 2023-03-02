#ifndef __POSKEEPER_H__
#define __POSKEEPER_H__

#include "math/XVector3.h"

struct EnterPosition
{
	Vector3 position;
	float   face;
};

class Scene;
class Role;
class PositionKeeper
{
public:
	PositionKeeper();
	~PositionKeeper();

	inline UINT32 GetLastHallSceneID() { return m_lastHallTemplateID; }
	void ResetHall();
	void ResetPos(UINT32 sceneid, Vector3 &position, float& face);

	void InitFromBrief(const KKSG::RoleBrief &brief);
	void SaveToBrief(KKSG::RoleBrief *pBrief);
	void OnLeaveScene(Scene *pScene, const Vector3 &position, float face);
	EnterPosition GetEnterScenePosition(Scene *pScene, Role* pRole);
	void SetInitFace(float face) { m_initFace = face; }
	float GetInitFace() { return m_initFace; }

private:

	UINT32      m_lastHallTemplateID;
	float       m_lastHallFace;
	Vector3     m_lastHallPosition;
	float		m_lastguildface;
	Vector3		m_lastguildpos;
	float		m_initFace;
};

#endif // __POSKEEPER_H__