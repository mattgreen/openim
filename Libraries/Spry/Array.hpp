#ifndef SPRY_ARRAY_HPP
#define SPRY_ARRAY_HPP

#pragma warning(push)
#pragma warning(disable: 4239)	// nonstandard extension used: conversion from Array<T> to Array<T>&
								// std::auto_ptr generates the same warning.
namespace Spry
{
	// Simple array template with destructive copy semantics.
	//
	template<typename T>
	class Array
	{
	public:
		Array() :
			_array(0),
			Length(0)
		{
		}

		Array(int length) :
			_array(new T[length]),
			Length(length)
		{
		}

		Array(T* array, int length) :
			_array(array),
			Length(length)
		{
		}

		Array(Array<T>& rhs) :
			_array(rhs._array),
			Length(rhs.Length)
		{
			rhs._array = 0;
		}

		~Array()
		{
			delete[] _array;
		}

		Array<T>& operator=(Array<T>& rhs)
		{
			delete[] _array;

			_array = rhs._array;
			Length = rhs.Length;
			rhs._array = 0;

			return *this;
		}

		T& operator[](int n)
		{
			return _array[n];
		}

		int Length;

	private:
		T* _array;
	};
}

#pragma warning(pop)

#endif