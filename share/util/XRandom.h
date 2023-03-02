#ifndef __XRANDOM_H__
#define __XRANDOM_H__

#include <numeric>


enum XRandomType
{
	CommonRandom,
	SpriteRandom,
	PandoraRandom,
};

struct XRandomImpl;

class XRandom;
class XSpriteRandom;
class XPandoraRandom;
class XRankTreeRandom;

class XBaseRandom
{
	friend class XRandom;
	friend class XSpriteRandom;
	friend class XPandoraRandom;
	friend class XRankTreeRandom;
public:
	XBaseRandom();
	XBaseRandom(UINT32 x, UINT32 y);

private:
	UINT32 RandInt();	
	UINT32 RandInt(UINT32 Min, UINT32 Max);
	double RandDouble();
	double RandDouble(double Min, double Max);

private:
	XRandomImpl *m_detail;
};

#define DeclareRandom(XName)							\
class XName												\
{														\
public:													\
	static UINT32 randInt();							\
	static UINT32 randInt(UINT32 Min, UINT32 Max);		\
	static double randDouble();							\
	static double randDouble(double Min, double Max);	\
};

///> randIntµÄ·¶Î§ÊÇ[Min,Max), ×ó±ÕÓÒ¿ª
DeclareRandom(XRandom);
DeclareRandom(XRankTreeRandom);
DeclareRandom(XSpriteRandom);
DeclareRandom(XPandoraRandom);


class DistributeRandom
{
public:

	void Init(const std::vector<int> &Density);
	void Init(const std::vector<double> &Density);
	int  GetRandom();
	void Reset();

private:

	struct Alias
	{
		double   Prob;
		int      AliasID;
	};

	std::vector<Alias> AliasTable;
};

#endif // __XRANDOM_H__