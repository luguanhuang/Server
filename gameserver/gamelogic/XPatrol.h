#ifndef __PATROL_H__
#define __PATROL_H__

#include "entity/XEntity.h"

class Unit;
class XAIComponent;

class XPatrol
{
public:
	enum PathType
	{
		PT_PINGPONG = 1,
		PT_LOOP = 0,
		PT_NORMAL = 2,
	};
private:
	bool _is_reverse_nav;
	int _nav_index;
	float _nav_gap;
	UINT64 _nav_node_finish_time;
	bool _is_in_nav_gap;
	bool _is_pingpong;
	bool _is_loop;

	std::vector<Vector3> _nav_points;
	std::vector<Vector3> _tangentLines;
	std::vector<float> _nav_time;

	XAIComponent* m_pAIComponent;
public:
	XPatrol();
	void Init(XAIComponent* pAI);
public:
	float GetNavGap();
	const Vector3& GetCurNavPos();
	const Vector3& GetNextNavPos();
	const Vector3& GetNextForwardNavPos(const Vector3& entityPos);

	inline UINT64 GetNavNodeFinishTime(){return _nav_node_finish_time;}
	inline void SetNavNodeFinishTime(UINT64 time){_nav_node_finish_time = time;}
	inline bool GetIsInNavGap(){return _is_in_nav_gap;}
	inline void SetIsInNavGap(bool nav){_is_in_nav_gap = nav;}
	inline bool IsNavPingpong(){return _is_pingpong;}
	inline void ToggleNavDir() { _is_reverse_nav = !_is_reverse_nav; }

	void InitNavPath();
	void InitNavPath(const std::vector<Sequence<float, 4>>& navigation, PathType type);
};

#endif