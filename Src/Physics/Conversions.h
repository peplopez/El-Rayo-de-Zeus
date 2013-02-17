/**
@file Conversions.h

Funciones auxiliares para convertir entre los tipos de PhysX y los tipos de la lógica. 

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef	__PHYSICS_CONVERSIONS_H_
#define	__PHYSICS_CONVERSIONS_H_

#include "BaseSubsystems/Math.h"

#include <foundation/PxVec3.h> 
#include <foundation/PxTransform.h>
#include <foundation/PxMat44.h>
#include <characterkinematic/PxExtended.h>


// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/**
	Transforma un vector lógico al equivalente en PhysX.
	*/
	inline physx::PxVec3 Vector3ToPxVec3(const Vector3 &v) {
		return physx::PxVec3(v.x, v.y, v.z);
	}

	/**
	Transforma un vector de PshysX al equivalente lógico.
	*/
	inline Vector3 PxVec3ToVector3(const physx::PxVec3 &v) {
		return Vector3(v.x, v.y, v.z);
	}


	/**
	Transforma un vector lógico al equivalente vector extendido de PhysX.
	*/
	inline physx::PxExtendedVec3 Vector3ToPxExtendedVec3(const Vector3 &v) {
		return physx::PxExtendedVec3(v.x, v.y, v.z);
	}

	/**
	Transforma un vector extendido de PshysX al equivalente lógico.
	*/
	inline Vector3 PxExtendedVec3ToVector3(const physx::PxExtendedVec3 &v) {
		return Vector3((float) v.x, (float) v.y, (float) v.z);
	}

	/**
	Transforma un Transform de PhysX a una matriz 4x4 lógica equivalente.
	*/
	inline Matrix4 PxTransformToMatrix4(const physx::PxTransform &t) {
		physx::PxMat44 m(t);

		return Matrix4(m(0,0), m(0,1), m(0,2), m(0,3), 
					   m(1,0), m(1,1), m(1,2), m(1,3),
					   m(2,0), m(2,1), m(2,2), m(2,3),
					   0, 0, 0, 1);
	}

	/**
	Transforma una matriz 4x4 lógica en un Transform de PhysX equivalente.
	*/
	inline physx::PxTransform Matrix4ToPxTransform(const Matrix4 &m) {
		physx::PxMat44 pm(physx::PxVec4(m[0][0], m[1][0], m[2][0], m[3][0]),
						  physx::PxVec4(m[0][1], m[1][1], m[2][1], m[3][1]),
						  physx::PxVec4(m[0][2], m[1][2], m[2][2], m[3][2]),
						  physx::PxVec4(m[0][3], m[1][3], m[2][3], m[3][3]));

		return physx::PxTransform(pm);
	}

}; // namespace Physics

#endif // __PHYSICS_CONVERSIONS_H_