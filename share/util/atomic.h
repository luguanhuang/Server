#ifndef _UTIL_ATOMIC_H_
#define _UTIL_ATOMIC_H_

inline UINT64 atomic_increase(UINT64* val)
{
#ifdef WIN32
	return InterlockedIncrement(val);
#else
	return __sync_add_and_fetch(val, 1);
#endif
}

inline UINT64 atomic_decrease(UINT64* val)
{
#ifdef WIN32
	return InterlockedDecrement(val);
#else
	return __sync_sub_and_fetch(val, 1);
#endif
}

#endif