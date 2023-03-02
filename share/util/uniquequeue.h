#ifndef __UNIQUEQUEUE_H__
#define __UNIQUEQUEUE_H__

template<typename T>
class UniqueQueue
{
public:
	UniqueQueue(){}
	~UniqueQueue(){}

	void push(const T& val);
	void pop();
	bool empty() const;

	UINT32 size() const;

	T front();
	const T& front() const;
	T back();
	const T& back() const;

	bool has(const T& val) const;

private:
	std::queue<T> m_queue;
	std::set<T> m_set;
};

template<typename T>
void UniqueQueue<T>::push(const T& val)
{
	if(m_set.find(val) == m_set.end())
	{
		m_queue.push(val);
		m_set.insert(val);
	}
}

template<typename T>
void UniqueQueue<T>::pop()
{
	if(empty()) return;
	m_set.erase(m_queue.front());
	m_queue.pop();
}

template<typename T>
bool UniqueQueue<T>::empty() const
{
	return m_queue.empty();
}

template<typename T>
unsigned UniqueQueue<T>::size() const
{
	return m_queue.size();
}

template<typename T>
T UniqueQueue<T>::front()
{
	return m_queue.front();
}

template<typename T>
const T& UniqueQueue<T>::front() const
{
	return m_queue.front();
}

template<typename T>
T UniqueQueue<T>::back()
{
	return m_queue.back();
}

template<typename T>
const T& UniqueQueue<T>::back() const
{
	return m_queue.back();
}

template<typename T>
bool UniqueQueue<T>::has(const T& val) const
{
	return m_set.find(val) != m_set.end();
}

#endif