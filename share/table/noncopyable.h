#ifndef __Table_NONCOPYABLE_H__
#define __Table_NONCOPYABLE_H__

class noncopyable {
public:
	noncopyable(){};
	virtual ~noncopyable(){};
private:
	noncopyable(const noncopyable&);
	const noncopyable& operator= (const noncopyable&);
};

#endif
