#ifndef __XBULLET_H__
#define __XBULLET_H__

#include "math/XVector3.h"

class XBulletCore;
class XEntity;
class Unit;
class Scene;

struct XBulletTarget
{
public:

	XBulletTarget()
		:ID(0),
		 TimerToken(INVALID_HTIMER),
		 Hurtable(true),
		 HurtCount(0)
	{}

	UINT64 ID;
	HTIMER TimerToken;
	bool Hurtable;
	int HurtCount;
};

class XBullet
{
public:
	XBullet(bool client, UINT64 id, XBulletCore* data);
	~XBullet();

	inline UINT64 ID() const { return _id; }
	inline UINT64 ExtraID() const { return _extra_id; }

	inline void SetExtraID(UINT64 ex) { _extra_id = ex; }

	bool IsExpired();
	void Destroy();
	void Result(XEntity* target, bool cycle = true);
	void Update(float fDeltaT);

	void OnResult(UINT64 id);
	bool IsHurtEntity(UINT64 id);

	inline XBulletCore* GetBulletCore() const { return _data; }
	inline void DeActive() { _active = false; }
	inline bool IsClient(){ return _client; }

protected:
	static void RingBulletCollideUnit(Scene* pScene, XEntity* firer, float _ir, float _or, XBullet* bullet);
	static void BulletCollideUnit(Scene* pScene, XEntity* firer, const Vector3& rectcenter, float hlen, float rotation, float r, XBullet* bullet);
	static void PlaneBulletCollideUnit(Scene* pScene, XEntity* firer, const Vector3& origin, const Vector3& move, float r, XBullet* bullet);

private:
	void OnOnceTriggered();
	void OnRefined();
	void OnPingPong();

	bool OnRefined(IArgs*, void* param);
	bool OnTailResult(IArgs*, void* param);

	void TailResult();

private:
	UINT32 _magic_num;

	UINT64 _id;
	UINT64 _extra_id;

	bool _client;
	bool _active;
	bool _pingpong;
	bool _hit_triggered;

	int _layer_mask;
	float _elapsed;

	XBulletCore* _data;

	Vector3 _origin;
	Vector3 _position;

	int _tail_results;
	HTIMER _tail_results_token;

	std::map<UINT64, XBulletTarget> _hurt_target;
};

#endif	//__XBULLET_H__
