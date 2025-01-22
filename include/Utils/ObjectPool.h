#pragma once

#include <array>
#include <cassert>
#include <list>

// Careful!!! this collection is not thread-safe

template <typename T, size_t N>
class ObjectPool
{
	public:

		ObjectPool();
		~ObjectPool();


		size_t getCapacity() const { return m_elements.size(); }  // max original number of elements in the pool
		size_t getSize() const { return  m_elements.size() - m_availableElements.size(); } // number of elements currently in use

		T& get();
		void release(T& element);

	private:

		std::array<T, N> m_elements;
		std::list<T*> m_availableElements;
};

template<typename T, size_t N>
ObjectPool < T, N>::ObjectPool()
{
	for (T& element : m_elements)
	{
		m_availableElements.emplace_back(&element);
	}
}

template<typename T, size_t N>
ObjectPool < T, N>::~ObjectPool()
{
	assert(m_elements.size() == m_availableElements.size() && "We are about to leak memory, missing elements in the pool prior to its destruction");
}

template<typename T, size_t N>
T& ObjectPool<T, N>::get()
{
	assert(m_availableElements.size() > 0 && "There are no elements avialable in the pool, please increase its size");
	T* availableElement = m_availableElements.front();
	m_availableElements.pop_front();
	return *availableElement;
}

template<typename T, size_t N>
void ObjectPool<T, N>::release(T& element)
{
	assert(m_availableElements.size() < m_elements.size() && "Pool is already full");
	m_availableElements.push_back(&element);
}