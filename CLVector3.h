#ifndef _CLVECTOR3_H
#define _CLVECTOR3_H

#include <cstring>

template<typename T>
class CLVector3
{
public:
	CLVector3()
	{
		memset(_val, 0, sizeof(T)*3 );
	}

	CLVector3(T x, T y, T z) 
	{
		_val[0] = x;
		_val[1] = y;
		_val[2] = z;
	}

	CLVector3(T* data)
	{
		memcpy( _val, data, 3*sizeof(T) );
	}

	T& operator[](int i){
		return _val[i];
	}

	T operator[](int i) const{
		return _val[i];
	}

	T* asArray(){ return _val; }
	const T* asArray() const { return _val; }

	T x(){ return _val[0]; }
	T y(){ return _val[1]; }
	T z(){ return _val[2]; }
private:
	T _val[3];

};

typedef CLVector3<size_t> CLCoordinate;
typedef CLVector3<size_t> CLRegion;

#endif