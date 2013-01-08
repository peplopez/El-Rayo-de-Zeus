/**
@file Conversions.h

Funciones auxiliares para convertir entre los tipos de PhysX y los usados en la lógica. 

@author Antonio Sánchez Ruiz-Granados
@date Octubre, 2010
*/

#ifndef	__PHYSICS_CONVERSIONS_H_
#define	__PHYSICS_CONVERSIONS_H_

#include "BaseSubsystems/Math.h"

#include <NxPhysics.h>

// Las clases y funciones nuevas relacionadas con la física pertenecen a este paquete
namespace Physics {

	/**
	Transforma un vector lógico al equivalente en PhysX.
	*/
	inline NxVec3 Vector3ToNxVec3(const Vector3 &v) {
		return NxVec3(v.x, v.y, v.z);
	}

	/**
	Transforma un vector de PshysX al equivalente lógico.
	*/
	inline Vector3 NxVec3ToVector3(const NxVec3 &v) {
		return Vector3(v.x, v.y, v.z);
	}

	/**
	Transforma una matriz 3x3 lógica al equivalente en PhysX.
	*/
	inline NxMat33 Matrix3ToNxMat33(const Matrix3 &m) {
		NxVec3 row[3] = { NxVec3(m[0][0], m[0][1], m[0][2]),
						  NxVec3(m[1][0], m[1][1], m[1][2]),
						  NxVec3(m[2][0], m[2][1], m[2][2]) };
		
		return NxMat33(row[0], row[1], row[2]);
	}

	/**
	Transforma una matriz 3x3 de PhysX al equivalente lógico.
	*/
	inline Matrix3 NxMat33ToMatrix3(const NxMat33 &m) {
		return Matrix3(m(0,0), m(0,1), m(0,2),
					   m(1,0), m(1,1), m(1,2),
					   m(2,0), m(2,1), m(2,2));
	}

	/**
	Transforma una matriz 4x4 lógica a una matriz 3x4 de PhysX.
	*/
	inline NxMat34 Matrix4ToNxMat34(const Matrix4 &m) {
		Matrix3 rot;
		m.extract3x3Matrix(rot);
		Vector3 tras = m.getTrans();

		return NxMat34(Matrix3ToNxMat33(rot), Vector3ToNxVec3(tras));
	}

	/**
	Transforma una matriz 3x4 de PhysX a una matriz 4x4 lógica.
	*/
	inline Matrix4 NxMat34ToMatrix4(const NxMat34 &m) {
		return Matrix4(m.M(0,0), m.M(0,1), m.M(0,2), m.t.x, 
					   m.M(1,0), m.M(1,1), m.M(1,2), m.t.y,
					   m.M(2,0), m.M(2,1), m.M(2,2), m.t.z,
					   0, 0, 0, 1);
	}

	/**
	Transforma un rayo lógico en uno de PhysX.
	*/
	inline NxRay RayToNxRay(const Ray &r) {
		return NxRay(Vector3ToNxVec3(r.getOrigin()), Vector3ToNxVec3(r.getDirection()));
	}

}; // namespace Physics

#endif