/**
@file Math.cpp
*/

#include "Math.h"

/**
Namespace en el que ofrecemos alguna definición de constante
matamática y métodos para convertir grados en radianes, etc.
*/
namespace Math
{
	
	void yaw(float turn, Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newYaw = yaw + Ogre::Radian(turn);
		rotation.FromEulerAnglesYXZ(newYaw, pitch, roll);
		transform = rotation;

	} // yaw
	

	void roll(float turn, Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newRoll = roll + Ogre::Radian(turn);
		rotation.FromEulerAnglesYXZ(yaw, pitch, newRoll);
		transform = rotation;

	} // roll

	void pitch(float turn, Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newPitch = pitch + Ogre::Radian(turn);
		rotation.FromEulerAnglesYXZ(yaw, newPitch, roll);
		transform = rotation;

	} // pitch

	void pitchYaw(float turnPitch, float turnYaw, Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newPitch = pitch + Ogre::Radian(turnPitch);
		Ogre::Radian newYaw = yaw + Ogre::Radian(turnYaw);
		
		rotation.FromEulerAnglesYXZ(newYaw, newPitch, roll);
		transform = rotation;

	} // pitchyaw

	
	/**
	Extrae el estado del viraje de una matriz de transformación.

	@param transform Matriz de transformación.
	@return Viraje de la entidad.
	*/
	float getYaw(const Matrix4& transform) 
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
	void setYaw(float turn, Matrix4& transform) 
	{
		// Reiniciamos la matriz de rotación
		transform = Matrix3::IDENTITY;
		// Sobre esta rotamos.
		Math::yaw(turn,transform);

	} // setYaw
	
	/**
	Establece un viraje a una matriz de transformación.

	@param turn Giro en radianes que se quiere etablecer.
	@param transform Matriz de transformación a modificar.
	*/
	void setPitch(float turn, Matrix4& transform) 
	{
		// Reiniciamos la matriz de rotación
		transform = Matrix3::IDENTITY;
		// Sobre esta rotamos.
		Math::pitch(turn,transform);

	} // setPitch

	/**
	Establece un viraje a una matriz de transformación.

	@param turn Giro en radianes que se quiere etablecer.
	@param transform Matriz de transformación a modificar.
	*/
	void setRoll(float turn, Matrix4& transform) 
	{
		// Reiniciamos la matriz de rotación
		transform = Matrix3::IDENTITY;
		// Sobre esta rotamos.
		Math::roll(turn,transform);

	} // setYaw

	void setPitchYaw(float pitch,float yaw, Matrix4& transform)
	{
		// Reiniciamos la matriz de rotación
		transform = Matrix3::IDENTITY;
		// Sobre esta rotamos.
		Math::pitchYaw(pitch,yaw,transform);

	}

	Vector3 fromCartesianToCylindrical(const Vector3 coordenadas) 
	{//el Vector3 de retorno contiene const float grados, const float radio, const float y
		Vector3 retorno=Vector3::ZERO;
		
		retorno.x= sqrtf( powf(coordenadas.x,2) +  powf(coordenadas.z, 2) );//grados
		retorno.y= coordenadas.y;//altura
		retorno.z= atan(coordenadas.z/coordenadas.x); //radio
		
		return retorno;
	} // de cartesianas a cilindricas

	Vector3 fromCylindricalToCartesian(const float grados, const float radio, const float y) 
	{		
		Vector3 retorno=Vector3::ZERO;
		retorno.x = radio * cos(fromDegreesToRadians(grados));
		retorno.y = y;
		retorno.z = radio * sin(fromDegreesToRadians(grados));
		return retorno;
	} // de cilindricas a cartesianas

	
} // namespace Math
