#ifndef __XACTIONCURVE_H__
#define __XACTIONCURVE_H__

using namespace std;

class XActionCurve
{
public:
	XActionCurve();
	~XActionCurve() { };

	inline float GetMaxValue() { return _max_value; }
	inline float GetFirstLandValue() { return _land_value; }

	inline float GetFirstValue() { return _curve.size() > 0 ? _curve[0] : 0; }
	inline float GetLastValue() { return _curve.size() > 0 ? _curve[_curve.size() - 1] : 0; }

	inline void SetMaxValue(float v) { _max_value = v; }
	inline void SetFirstLandValue(float v) { _land_value = v; }

	float GetTimeSpan();
	float Evaluate(float time);
	void AddKey(int idx, float time, float value);

private:
	float _max_value;
	float _land_value;

	vector<float> _curve;
};

class XActionCurveMgr
{
	XActionCurveMgr();
	~XActionCurveMgr();

	DECLARE_SINGLETON(XActionCurveMgr)

public:
	bool Init();
	inline void Uninit() {};

	XActionCurve* GetCurve(const string& name);
	XActionCurve* GetCurve(UINT32 hash);

private:
	unordered_map<UINT32, XActionCurve*> _curves;
};

#endif	//__XACTIONCURVE_H__