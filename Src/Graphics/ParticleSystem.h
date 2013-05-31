//---------------------------------------------------------------------------
// ParticleSystem.h
//---------------------------------------------------------------------------

/**
@file ParticleSystem.h

Contiene la declaración de la clase que maneja el ParticleSystem.

@see Graphics::CParticleSystem

@author FRS
@date Mayo, 2013
*/

#ifndef __Graphics_ParticleSystem_H
#define __Graphics_ParticleSystem_H

#include <IHHFXPublic.h>

#include "SceneElement.h"


namespace Graphics 
{
	/**
	Clase de ParticleSysteme.
	
	@ingroup graphicsGroup

	@author FRS
	@date Mayo, 2013
	*/
	class CParticleSystem : public CSceneElement, public IHHFX::IFXListener
	{
	public:

		/**
		Constructor de la clase.		
		*/		
		// TODO FRS parametrizar o hacer enum de hfx
		CParticleSystem(
			const std::string& hfx, const std::string& parentName, const Vector3& relativePos = Vector3::ZERO) :
			_hfx(hfx), _parentName(parentName), _relativePos(relativePos) {}
			

		/*****************
			GET's & SET'S
		*******************/

		bool isVisible() const;
		void start() const;
		void stop() const;


	private:
		
		std::string _hfx;
		std::string _parentName;
		Vector3 _relativePos;	// Pos relativa respecto al padre (particulas son hijas de otra entidad gráfica)
		

		//-------- SceneElement implementation --------------
		bool				load()		override; 
		void				unload()	override;


		/*********************
			HELL HEAVENS FX		
		**********************/
		IHHFXOgre*			_movObj;	// Sistema de particulas (movable object)		
	
		//--------- IHHFX::IFXListener implementation -------------------
		void				OnFXStarted(IHHFX *obj) override;
		void				OnFXStopped(IHHFX *obj) override;


	//	static bool					_IntersectScene(void *arg, const Ogre::Vector3 &start, const Ogre::Vector3 &direction, float length, SContactReport &contactReport);

	}; // class CParticleSystem

} // namespace Graphics

#endif // __Graphics_ParticleSystem_H
