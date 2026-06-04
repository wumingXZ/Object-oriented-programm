#ifndef __VEC_H
#define __VEC_H
#include <memory>

template<class T>
class Vec {
public:
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef size_t size_type;

	Vec() { elements = 0; sz = 0; space = 0; }

	Vec(const Vec& v) {
		sz = v.sz;
		space = v.sz;
		elements = new T[space];
		for(size_type i = 0; i < sz; ++i)
			elements[i] = v.elements[i];
	}

	~Vec() { delete[] elements; }

	Vec& operator=(const Vec& v) {
		if(this == &v) return *this;
		delete[] elements;
		sz = v.sz;
		space = v.sz;
		elements = new T[space];
		for(size_type i = 0; i < sz; ++i)
			elements[i] = v.elements[i];
		return *this;
	}

	size_type size() const { return sz; }

	T& operator[](size_type i) { return elements[i]; }
	const T& operator[](size_type i) const { return elements[i]; }

	iterator begin() { return elements; }
	const_iterator begin() const { return elements; }
	iterator end() { return elements + sz; }
	const_iterator end() const { return elements + sz; }

	void push_back(const T& val) {
		if(sz == space) {
			size_type new_space = (space == 0) ? 1 : space * 2;
			T* new_elements = new T[new_space];
			for(size_type i = 0; i < sz; ++i)
				new_elements[i] = elements[i];
			delete[] elements;
			elements = new_elements;
			space = new_space;
		}
		elements[sz++] = val;
	}

	void clear() {
		delete[] elements;
		elements = 0;
		sz = 0;
		space = 0;
	}

private:
	T* elements;
	size_type sz;
	size_type space;
};

#endif
