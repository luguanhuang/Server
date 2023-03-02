#ifndef __CURVEREADER_H__
#define __CURVEREADER_H__

class XActionCurve;

class CurveReader
{
public:
	static XActionCurve *ReadCurve(const char *fileName); 
};

#endif // __CURVEREADER_H__