#include "pch.h"
#include "XPatrol.h"
#include "unit/unit.h"
#include "component/XAIComponent.h"
#include "tablemgr/XEntityInfoLibrary.h"

XPatrol::XPatrol()
{
	m_pAIComponent = NULL;
	_is_reverse_nav = false;
	_nav_index = 0;
	_is_pingpong = false;
	_is_loop = false;
	_is_in_nav_gap = false;
	_nav_node_finish_time = 0;
	_nav_gap = 0;
}

void XPatrol::Init(XAIComponent* pAI)
{
	m_pAIComponent = pAI;
	if (m_pAIComponent == NULL)
		return;
	InitNavPath();
}

void XPatrol::InitNavPath(const std::vector<Sequence<float, 4>>& navigation, PathType type)
{
	_nav_index = 0;
	for (UINT32 i=0; i<navigation.size(); i++)
	{
		_nav_points.push_back(Vector3(navigation[i][0], navigation[i][1], navigation[i][2]));
		_nav_time.push_back(navigation[i][3]);
	}
	_is_pingpong = (type == PT_PINGPONG);
	_is_loop = (type == PT_LOOP);

	if (_nav_points.size() > 1)
	{
		std::vector<Vector3> _vectors;

		if (_is_loop)
		{
			///> 第一个点之前的点，就是最后一个点
			_vectors.push_back(Vector3::Normalized(_nav_points[0] - _nav_points[_nav_points.size() - 1]));
		}
		else
		{
			///> 第一个点之前的向量 和 第一个点一样
			_vectors.push_back(Vector3::Normalized(_nav_points[1] - _nav_points[0]));
		}

		for (uint i = 1; i < _nav_points.size(); ++i)
		{
			_vectors.push_back(Vector3::Normalized(_nav_points[i] - _nav_points[i - 1]));
		}

		if (_is_loop)
		{
			///> 最后一个点之后的点，就是第一个点
			_vectors.push_back(Vector3::Normalized(_nav_points[0] - _nav_points[_nav_points.size() - 1]));
		}
		else
		{
			///> 最后一个点之后的向量 和最后一个点一样
			_vectors.push_back(*_vectors.rbegin());
		}

		for (uint i = 1; i < _vectors.size(); ++i)
		{
			const Vector3& a = _vectors[i - 1];
			const Vector3& b = _vectors[i];

			Vector3 c = a + b;

			///> 两者太相反，取b的垂线
			if (Vector3::sqrtMagnitude(c) < 0.01f)
			{
				c = Vector3::Cross(b, Vector3::up);
			}

			_tangentLines.push_back(Vector3::Normalized(c));
		}
	}

	_nav_gap = GetGlobalConfig()._AINavGap;
	_nav_node_finish_time = 0;
	_is_in_nav_gap = false;
}

void XPatrol::InitNavPath()
{
	if (!m_pAIComponent->GetEntity()->GetUnit()->IsRole())
	{
		const XEntityStatistics::RowData* enemy = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(m_pAIComponent->GetEntity()->GetUnit()->GetTemplateID());

		if (enemy)
		{
			InitNavPath(enemy->navigation, (PathType)enemy->IsNavPingpong);
		}
	}
}

const Vector3& XPatrol::GetCurNavPos()
{
	if (_nav_points.size() == 0)
		return Vector3::zero;

	if (_nav_index >= (int)_nav_points.size())
		return _nav_points[(int)_nav_points.size()-1];
	else
		return _nav_points[_nav_index];
}

float XPatrol::GetNavGap()
{
	if (_nav_time.size() == 0)
		return 0.0f;

	if (_nav_index >= (int)_nav_time.size())
		return _nav_time[(int)_nav_time.size()-1];
	else
		return _nav_time[_nav_index];
}

const Vector3& XPatrol::GetNextNavPos()
{
	if (_nav_points.size() == 0)
		return Vector3::zero;

	if (_is_pingpong)
	{
		if (_is_reverse_nav)
		{
			if(_nav_index == 0)
			{
				_is_reverse_nav = false;
				_nav_index = 1;
			}
			else
				_nav_index -= 1;
		}
		else
		{
			if (_nav_index == (int)_nav_points.size() -1)
			{
				_is_reverse_nav = true;
				_nav_index = _nav_index -1;
			}
			else
				_nav_index += 1;
		}
	}
	else
	{
		if (!_is_reverse_nav)
		{
			if (_nav_index >= (int)_nav_points.size() -1)
			{
				if (_is_loop)
					_nav_index = 0;
			}
			else
				++_nav_index;
		}
		else
		{
			if (_nav_index <= 0)
			{
				if (_is_loop)
					_nav_index = (int)_nav_points.size() -1;
			}
			else
				--_nav_index;
		}
	}

	if (_nav_index < 0) _nav_index = 0;
	if (_nav_index >= (int)_nav_points.size()) _nav_index = (int)_nav_points.size() -1;

	return GetCurNavPos();
}

const Vector3& XPatrol::GetNextForwardNavPos(const Vector3& entityPos)
{
	if (_tangentLines.size() == 0)
		return GetNextNavPos();

	int old_index = _nav_index;
	int init_index = _nav_index;
	while(true)
	{
		const Vector3& curNavPos = GetCurNavPos();
		const Vector3& vec = entityPos - curNavPos;
		///> 非常接近此点，直接返回
		if (Vector3::sqrtMagnitude(vec) < 0.01)
			return curNavPos;
		float dot = (_is_reverse_nav ? -1 : 1) * Vector3::Dot( _tangentLines[_nav_index], vec);
		if (dot <= 0.0f)
			return curNavPos;

		if (!_is_reverse_nav)
		{
			if (_nav_index >= (int)_nav_points.size() -1)
			{
				if (_is_loop)
					_nav_index = 0;
			}
			else
				++_nav_index;
		}
		else
		{
			if (_nav_index <= 0)
			{
				if (_is_loop)
					_nav_index = (int)_nav_points.size() -1;
			}
			else
				--_nav_index;
		}

		if (_nav_index < 0) _nav_index = 0;
		if (_nav_index >= (int)_nav_points.size()) _nav_index = (int)_nav_points.size() -1;

		if (old_index == _nav_index || _nav_index == init_index)
		{
			return curNavPos;
		}
		old_index = _nav_index;
	}

	return Vector3::zero;
}
