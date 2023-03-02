#include "pch.h"
#include "XActionCurve.h"
#include "util/XCommon.h"
#include "XPath.h"
#include "table/CurveReader.h"
#include <math.h>

#define frameStep	(0.0333333f)

XActionCurve::XActionCurve()
	:_max_value(0), _land_value(0)
{

}

void XActionCurve::AddKey(int idx, float time, float value)
{
	_curve.push_back(value);
}

float XActionCurve::GetTimeSpan()
{
	float span = _curve.size() * frameStep - frameStep;
	return span > 0 ? span : 0;
}

float XActionCurve::Evaluate(float time)
{
	float key = time / frameStep;
	
	size_t up = (size_t) ceil(key);
	size_t low = (size_t) floor(key);

	if(up >= _curve.size()) up = _curve.size() - 1;
	if(low >= _curve.size()) low = _curve.size() - 1;

	return _curve[low] + ((_curve[up] - _curve[low]) / frameStep) * (time - (low * frameStep));
}

INSTANCE_SINGLETON(XActionCurveMgr)

XActionCurveMgr::XActionCurveMgr()
{

}

XActionCurveMgr::~XActionCurveMgr()
{
	unordered_map<UINT32, XActionCurve*>::iterator it = _curves.begin();
	while (it != _curves.end())
	{
		delete it->second;
		++it;
	}

	_curves.clear();
}

bool XActionCurveMgr::Init()
{
	return true;
}

XActionCurve* XActionCurveMgr::GetCurve(const string& name)
{
	UINT32 hash = XCommon::XHash(name.c_str());

	unordered_map<UINT32, XActionCurve*>::iterator it = _curves.find(hash);
	if(it != _curves.end())
		return it->second;
	else
	{
		if(name.length() == 0) return NULL;

		XActionCurve* curve = CurveReader::ReadCurve((name + ".txt").c_str());
		if(curve != NULL) _curves[hash] = curve;
		
		return curve;
	}
}

XActionCurve* XActionCurveMgr::GetCurve(UINT32 hash)
{
	unordered_map<UINT32, XActionCurve*>::iterator it = _curves.find(hash);
	return (it != _curves.end()) ? it->second : NULL;
}