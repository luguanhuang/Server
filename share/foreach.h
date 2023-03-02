#ifndef __FOREACH_H__
#define __FOREACH_H__

#define in ,
#define ITERATOR(j,S) for(auto j = S.begin(); j != S.end(); ++j)

#ifdef WIN32

#define LL (
#define foreach(...)  ITERATOR LL __VA_ARGS__ )

#else

#define foreach(...)  ITERATOR(__VA_ARGS__)

#endif

template <typename T, typename F>
void LoopCall(T &t, F f)
{
	for(auto i = t.begin(); i != t.end(); ++i) 
	{
		((*i)->*f)();
	}
}

template <typename T, typename F, typename A1>
void LoopCall(T &t, F f, A1 &a1)
{
	for(auto i = t.begin(); i != t.end(); ++i) 
	{
		((*i)->*f)(a1);
	}
}

template <typename T, typename F, typename A1, typename A2>
void LoopCall(T &t, F f, A1 &a1, A2 &a2)
{
	for(auto i = t.begin(); i != t.end(); ++i) 
	{
		((*i)->*f)(a1, a2);
	}
}

template <typename T, typename F, typename A1, typename A2, typename A3>
void LoopCall(T &t, F f, A1 &a1, A2 &a2, A3 &a3)
{
	for(auto i = t.begin(); i != t.end(); ++i) 
	{
		((*i)->*f)(a1, a2, a3);
	}
}

template <typename T, typename F, typename A1, typename A2, typename A3, typename A4>
void LoopCall(T &t, F f, A1 &a1, A2 &a2, A3 &a3, A4 &a4)
{
	for(auto i = t.begin(); i != t.end(); ++i) 
	{
		((*i)->*f)(a1, a2, a3, a4);
	}
}

#endif
