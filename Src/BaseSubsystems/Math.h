/**
@file Math.h

Este fichero contiene la definición de ciertos tipos de datos
utilizados por la aplicación y relacionados con la base
matemática; en particular, define distintos tipos de matriz,
vector, etc.
<p>
En la práctica, los tipos son simplemente tipos sinónimos
a los de Ogre, para evitar pérdidas de tiempo en conversiones
superfluas entre la aplicación y Ogre (sí habrá que hacerlas
entre la aplicación y el motor de física, por ejemplo).
<p>
Se ofrecen también una serie de funciones auxiliares.
 
@author David Llansó
@date Julio, 2010
*/

#ifndef __BaseSubsystems_Math_H
#define __BaseSubsystems_Math_H

// Includes de Ogre donde se definen los tipos
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreQuaternion.h>
#include <OgreMatrix3.h>
#include <OgreMatrix4.h>
#include <OgreRay.h>

/**
Definicion de matriz de 4x4. La definición del tipo de datos
es la misma que la utilizada por el motor gráfico, por lo tanto
es dependiente del motor usado.
 */
typedef Ogre::Matrix4 Matrix4;

/**
Definición de matriz de 3x3 de rotación. La definición del tipo 
de datos es la misma que la utilizada por el motor gráfico, por 
lo tanto es dependiente del motor usado.
 */
typedef Ogre::Matrix3 Matrix3;

/**
Vector (o punto) 2d utilizado. La definición del tipo de datos
es la misma que la utilizada por el motor gráfico, por lo tanto
es dependiente del motor usado.
 */
typedef Ogre::Vector2 Vector2;

/**
Vector (o punto) 3d utilizado. La definición del tipo de datos
es la misma que la utilizada por el motor gráfico, por lo tanto
es dependiente del motor usado.
 */
typedef	Ogre::Vector3 Vector3;

/**
Vector (o punto) 4d utilizado. La definición del tipo de datos
es la misma que la utilizada por el motor gráfico, por lo tanto
es dependiente del motor usado.
 */
typedef	Ogre::Vector4 Vector4;

/**
Quaternion, usado para cálculos de rotaciones tridimensionales. 
La definición del tipo de datos es la misma que la utilizada por 
el motor gráfico, por lo tanto es dependiente del motor usado.
 */
typedef Ogre::Quaternion Quaternion;

/**
Rayo. La definición del tipo de datos es la misma que la utilizada 
por el motor gráfico, por lo tanto es dependiente del motor usado.
 */
typedef Ogre::Ray Ray;

/**
Namespace en el que ofrecemos alguna definición de constante
matamática y métodos para convertir grados en radianes, etc.
*/
namespace Math
{
	/**
	Definición de la constante PI.
	*/
	static const float PI = float( 4.0 * atan( 1.0 ) );

	/**
	Constante para pasar de grados a radianes.
	*/
	static const float _deg2Rad = PI / 180.0f;

	/**
	Constante para pasar de radianes a grados.
	*/
	static const float _rad2Deg = 180.0f / PI;

	/**
	Transforma grados en radianes.

	@param degree Ángulo en grados.
	@return Ángulo en radianes.
	*/
	static float fromDegreesToRadians(float degrees) {return degrees*_deg2Rad;}

	/**
	Transforma radianes en grados.

	@param radian Ángulo en radianes.
	@return Ángulo en grados.
	*/
	static float fromRadiansToDegrees(float radians) {return radians*_rad2Deg;}
	
	/**
	Crea un vector unitario de dirección a partir de un angulo de
	orientación en radianes.

	@param orientation Orientación en radianes.
	@return Vector unitario en el plano XZ.
	*/
	static Vector3 getDirection(float orientation) 
	{
		return Vector3(-sin(orientation), 0, -cos(orientation));

	} // getDirection
	
	/**
	Aplica un viraje a una matriz de transformación.

	@param turn Giro en radianes que se quiere aplicar.
	@param transform Matriz de transformación a modificar.
	*/
	static void yaw(float turn, Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newYaw = yaw + Ogre::Radian(turn);
		rotation.FromEulerAnglesYXZ(newYaw, pitch, roll);
		transform = rotation;

	} // yaw
	
	/**
	Extrae el estado del viraje de una matriz de transformación.

	@param transform Matriz de transformación.
	@return Viraje de la entidad.
	*/
	static float getYaw(const Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		return yaw.valueRadians();

	} // getYaw
	
	/**
	Establece un viraje a una matriz de transformación.

	@param turn Giro en radianes que se quiere etablecer.
	@param transform Matriz de transformación a modificar.
	*/
	static void setYaw(float turn, Matrix4& transform) 
	{
		// Reiniciamos la matriz de rotación
		transform = Matrix3::IDENTITY;
		// Sobre esta rotamos.
		Math::yaw(turn,transform);

	} // setYaw
	
	/**
	Crea un vector unitario de dirección en el plano XZ a partir 
	de una matriz de transformación.

	@param transform Matriz de transformación.
	@return Vector unitario en el plano XZ.
	*/
	static Vector3 getDirection(const Matrix4& transform) 
	{
		return getDirection(getYaw(transform));

	} // getDirection

} // namespace Math

#endif // __BaseSubsystems_Math_H
