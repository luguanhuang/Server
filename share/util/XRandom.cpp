#include "pch.h"
#include "XRandom.h"
#include <limits.h>
#include "gettimeofday.h"
#include <time.h>

/*
* 梅森旋转算法（Mersenne twister）是一个伪随机数发生算法。
* 可以快速产生高质量的伪随机数， 修正了古典随机数发生算法的很多缺陷。
* http://zh.wikipedia.org/wiki/梅森旋转算法
* 
*/

#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

struct XRandomImpl
{
	enum {ARRAY_SIZE = 624};
	UINT32 m_seed;
	UINT32 m_index;
	UINT64 m_MT[ARRAY_SIZE];

	XRandomImpl()
	{
		InitSeed((UINT32)time(NULL));
	}

	XRandomImpl(UINT32 x, UINT32 y)
	{
		timeval now;
		gettimeofday(&now, NULL);
		UINT64 millisec = now.tv_sec * 1000 + now.tv_usec / 1000;
		UINT32 seed = millisec & 0xffffffff;
		InitSeed(seed * x + y);
	}

	void InitSeed(UINT32 seed)
	{
		int mti;
		m_seed = seed;
		m_MT[0]= m_seed & 0xffffffffUL;
		for (mti=1; mti<N; mti++) {
			m_MT[mti] = (1812433253UL * (m_MT[mti-1] ^ (m_MT[mti-1] >> 30)) + mti);
			m_MT[mti] &= 0xffffffffUL;
		}
		m_index = mti;
	}

	UINT32 RandInt()
	{
		UINT64 y;
		static UINT64 mag01[2]={0x0UL, MATRIX_A};

		if (m_index >= N) { /* generate N words at one time */
			int kk;

			for (kk=0;kk<N-M;kk++) {
				y = (m_MT[kk]&UPPER_MASK)|(m_MT[kk+1]&LOWER_MASK);
				m_MT[kk] = m_MT[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
			}
			for (;kk<N-1;kk++) {
				y = (m_MT[kk]&UPPER_MASK)|(m_MT[kk+1]&LOWER_MASK);
				m_MT[kk] = m_MT[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
			}
			y = (m_MT[N-1]&UPPER_MASK)|(m_MT[0]&LOWER_MASK);
			m_MT[N-1] = m_MT[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

			m_index = 0;
		}

		y = m_MT[m_index++];
		y ^= (y >> 11);
		y ^= (y << 7) & 0x9d2c5680UL;
		y ^= (y << 15) & 0xefc60000UL;
		y ^= (y >> 18);
		return (UINT32)y;
	}
};

XBaseRandom::XBaseRandom()
{
	m_detail = new XRandomImpl();
}

XBaseRandom::XBaseRandom(UINT32 x, UINT32 y)
{
	m_detail = new XRandomImpl(x, y);
}


UINT32 XBaseRandom::RandInt()
{
	return m_detail->RandInt();
}

UINT32 XBaseRandom::RandInt(UINT32 Min, UINT32 Max)
{
	if (Max == Min)
		return Min;

	if (Max < Min)
		return Min;

	UINT32 Diff = Max - Min;
	UINT32 R = RandInt();
	return Min + (R % Diff);
}

double XBaseRandom::RandDouble()
{
	UINT64 a= RandInt()>>5, b=RandInt()>>6;
	return (a*67108864.0+b)*(1.0/9007199254740992.0);
}

double XBaseRandom::RandDouble(double Min, double Max)
{
	return Min + (Max - Min) * RandDouble();
}

#define InitRandom(XName, X, Y)						\
static XBaseRandom static##XName(X, Y);				\
UINT32 XName::randInt()								\
{													\
	return static##XName.RandInt();					\
}													\
UINT32 XName::randInt(UINT32 Min, UINT32 Max)		\
{													\
	return static##XName.RandInt(Min, Max);			\
}													\
double XName::randDouble()							\
{													\
	return static##XName.RandDouble();				\
}													\
double XName::randDouble(double Min, double Max)	\
{													\
	return static##XName.RandDouble(Min, Max);		\
}

///> common random
InitRandom(XRandom, 1, 0);

///> RankTree random
InitRandom(XRankTreeRandom, 191, 59);

///> Sprite Egg random
InitRandom(XSpriteRandom, 137, 233);

///> Pandora random
InitRandom(XPandoraRandom, 271, 491);

#undef InitRandom

void DistributeRandom::Reset()
{
	AliasTable.clear();
}

int DistributeRandom::GetRandom()
{
	int i = XRandom::randInt(0, AliasTable.size());
	const Alias &al = AliasTable[i];
	if (XRandom::randDouble(0, 1) < al.Prob)
	{
		return i;
	}
	else
	{
		return al.AliasID;
	}
}

void DistributeRandom::Init(const std::vector<double> &Density)
{
	std::vector<int> LargeIDX;
	std::vector<int> SmallIDX;
	unsigned n = Density.size();
	AliasTable.resize(n);
	for (unsigned i = 0; i < n; ++i)
	{
		double t = Density[i] * n;
		AliasTable[i].Prob = t;
		AliasTable[i].AliasID = -1;
		if (t >= 1)
		{
			LargeIDX.push_back(i);
		}
		else
		{
			SmallIDX.push_back(i);
		}
	}

	while (!SmallIDX.empty() && !LargeIDX.empty())
	{
		int l = SmallIDX.back();
		SmallIDX.pop_back();
		int g = LargeIDX.back();
		LargeIDX.pop_back();

		Alias &al = AliasTable[l];
		al.AliasID = g;

		Alias &ag = AliasTable[g];
		ag.Prob -= (1 - al.Prob);

		if (ag.Prob < 1)
		{
			SmallIDX.push_back(g);
		}
		else
		{
			LargeIDX.push_back(g);
		}
	}

	while (!SmallIDX.empty())
	{
		int l = SmallIDX.back();
		SmallIDX.pop_back();
		Alias &al = AliasTable[l];
		al.AliasID = l;
		al.Prob = 1;
	}

	while (!LargeIDX.empty())
	{
		int l = LargeIDX.back();
		LargeIDX.pop_back();
		Alias &al = AliasTable[l];
		al.AliasID = l;
		al.Prob = 1;
	}
}

struct divide
{
	divide(double div)
	{
		m_div = div;
	}
	double m_div;
	double operator()(double i)
	{
		return i / m_div;
	}
};

void DistributeRandom::Init(const std::vector<int> &Density)
{
	std::vector<double> doubleDensity;
	doubleDensity.resize(Density.size());
	double total = std::accumulate(Density.begin(), Density.end(), 0);
	std::transform(Density.begin(), Density.end(), doubleDensity.begin(), divide(total));
	Init(doubleDensity);
}
