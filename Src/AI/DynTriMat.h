/**
@File DynTriMat.h

Implementación de una clase para representar matrices 
triangulares dinámicas.

@see CDynTriMat

@author Gonzalo Flórez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_DynTriMat_H
#define __AI_DynTriMat_H

#include <vector>
#include <assert.h>

/**
Clase que implementa una matriz triangular. Además, su implementación 
es dinámica, en el sentido de que se pueden añadir más elementos a la 
matriz en cualquier momento.
*/
template <class T>
class CDynTriMat
{
public:
	/**
	Constructor.
	*/
	CDynTriMat(void) : _maxSize(0) {};
	/**
	Destructor.
	*/
	~CDynTriMat(void) {};
	/**
	Devuelve el elemento en la posición de la matriz [a, b]. 
	Es el mismo que el que se encuentra en [b, a].
	<p>
	Si la fila o columna indicad por a o b es mayor o igual que 
	el tamaño de la matriz, falla.
	
	@return Elemento en [a, b].
	*/
	T getElement(int a, int b) const;
	/**
	Establece el valor del elemento en la posición [a, b].
	Al ser una matriz triangular, el elemento de la posición 
	[b, a] tomará el mismo valor. 
	<p>
	Si el tamaño de la matriz no es lo suficientemente grande 
	como para llegar a esta posición la matriz crecerá.
	@param a Columna de la matriz.
	@param b Fila de la matriz.
	@param value Valor del elemento.
	*/
	void setElement(int a, int b, T value);
	/**
	Devuelve las dimensiones de la matriz. Al ser una matriz cuadrada 
	sólo devuelve un valor.

	@return Dimensión de la matriz.
	*/
	int getCurrentSize();

private:
	/**
	Vector donde se almacenarán los datos.
	*/
	std::vector<T> _data;
	/**
	Dimensión de la matriz.
	*/
	int _maxSize;
	/**
	Esta función transforma un valor de [columna, fila] en la posición 
	correspondiente del vector _data
	*/
	int getPosition(int a, int b) const;
};

template <class T>
inline T CDynTriMat<T>::getElement(int a, int b) const {
	assert(a < _maxSize && b < _maxSize);
	int pos = getPosition(a, b);
	return _data[pos];
}

template <class T>
inline void CDynTriMat<T>::setElement(int a, int b, T value) {
	// Nos aseguramos que a >= b
	if (a < b) setElement(b, a, value); 
	int pos = getPosition(a, b);
	// Si es una posición nueva generamos la fila y columna entera
	if ((int)_data.size() < pos + 1) {
		// Obtenemos la posición del último elemento de la nueva fila
		int ultPos = getPosition(a, a);
		_data.resize(ultPos + 1);
		_maxSize = a + 1;
	}
	_data[pos] = value;
}

template <class T>
inline int CDynTriMat<T>::getPosition(int a, int b) const {
	if (a < b) return getPosition(b, a);
	return a * (a + 1) / 2 + b;
}

template <class T>
inline int CDynTriMat<T>::getCurrentSize() { 
	return _maxSize; 
}

/*
Por si es necesario en el futuro:
Para invertir la operación de acceso y sacar los índices de la matriz
a partir de una posición del vector, las fórmulas que hay que aplicar son:
a(v) = floor(-1 + sqrt(1 + 8 * v)) / 2
b(v) = v - a(v) * (a(v) + 1) / 2
Devuelve los índices en la matriz triangular superior.
*/

#endif