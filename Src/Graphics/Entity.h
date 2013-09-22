//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaración de la clase que representa una entidad gráfica.

@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_Entity_H
#define __Graphics_Entity_H

#pragma warning(disable: 4482)

#include <Graphics\SceneElement.h>

#include <OgreEntity.h>
#include <OgreSubEntity.h>

#include <stack>


namespace Graphics 
{

	enum TAttachPoint {
			ARM_L,
			ARM_R,
			FACE,
			HAND_L,
			HAND_R,
			HEAD			
	};

	/**
	Clase que representa una entidad gráfica. Contiene una referencia a
	una entidad de Ogre y al Nodo que la contiene.
	Oculta los detalles escabrosos del motor gráfico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las entidades de Ogre. Para cargar la entidad en Ogre y que 
	sea reenderizada, ésta debe añadirse a una escenada de Ogre mediante el 
	método attachToScene(). Solo la clase CScene puede invocar los métodos 
	para añadir o eliminar entidades de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Esto deriva de que en Ogre la carga
	de entidades se hace por las escenas, por lo que en nuestra arquitectura
	no permitimos entidades cargadas que no pertenezcan a ninguna escena.
	Al añadir una entidad a una escena se fuerza su carga en Ogre invocando a 
	load.
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	entidad gráfica (posición, orientación, escala...) se deberán hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la entidad debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de éstas, que permitan
	modelos con distintas características. Por ejemplo, ésta clase
	no permite especificar la animación utilizada. Si queremos que podamos
	guardar un modelo gráfico, y poder cambiar la animación actual,
	se utilizará la clase derivada correspondiente.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CEntity : public CSceneElement
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CEntity(const std::string &name, const std::string &mesh, bool isStatic = false);
		~CEntity();
		


		/************
			ATTACH
		**************/
		/**
			Unir el model mesh al hueso toBone
		*/
		void attach(TAttachPoint attachPoint, const std::string &mesh) {
			attach( _BONE_DICTIONARY[attachPoint], mesh);
			_boneObjectsNameTable[ _BONE_DICTIONARY[attachPoint] ].push_back(mesh); 
		}		
		void detach(TAttachPoint detachPoint) {
			detach( _BONE_DICTIONARY[detachPoint] );
			_boneObjectsNameTable[ _BONE_DICTIONARY[detachPoint] ].pop_back();
		}
			

		

		/******************
			GET's & SET's
		********************/
			
		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.

		 @return Cierto si la entidad es visible (está activa 
		 para ser reenderizada).
		*/	
		inline const std::string getName() const { return _name; }	
		inline const std::string getMesh() const { return _mesh; }
	
		inline bool isVisible() const { 
			assert(_loaded && "La entidad no ha sido cargada en la escena");
			return _entity->isVisible();
		} 
		inline unsigned int getNumSubEntities() const { 	
			assert(_loaded && "La entidad no ha sido cargada en la escena"); 
			return _entity->getNumSubEntities(); 
		}
			

	protected:
	
		/**
		Entidad de Ogre.
		*/ 
		Ogre::Entity *_entity;

		
		/**
		Carga la entidad gráfica correspondiente al nombre _mesh. No hace 
		comprobaciónes de si la entidad está ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		virtual bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaciónes de si la entidad está o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		virtual void unload();		
		

	private:

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Nombre del modelo de la entidad.
		*/
		std::string _mesh;




	/******************
		ATTACHMENTS
	*****************/

	private:

		 // DICCIONARIO TAttachPoint -> BoneName
		typedef std::map<TAttachPoint, std::string> TBoneDictionary;

			static TBoneDictionary _BONE_DICTIONARY;

				static TBoneDictionary _initBoneDictionary() {
					TBoneDictionary dictionary;
//*
						dictionary[TAttachPoint::ARM_L] =	"LeftHelper";		// SPARTAN
						dictionary[TAttachPoint::ARM_R] =	"RightHelper";
						dictionary[TAttachPoint::FACE] =	"paracascos";
						dictionary[TAttachPoint::HAND_L] =	"LeftHelperWeapon";
						dictionary[TAttachPoint::HAND_R] =	"RightHelperWeapon";
						dictionary[TAttachPoint::HEAD] =	"paracascos";
/*/
						dictionary[TAttachPoint::ARM_L] =	"Bip01 L Forearm";	// MARINE
						dictionary[TAttachPoint::ARM_R] =	"Bip01 R Forearm";
						dictionary[TAttachPoint::FACE] =	"Bip01 Head";
						dictionary[TAttachPoint::HAND_L] =	"Bip01 L Hand";
						dictionary[TAttachPoint::HAND_R] =	"Bip01 R Hand";
						dictionary[TAttachPoint::HEAD] =	"Bip01 Head";
/**/
					// TODO añadir on demand...
					return dictionary;
				}
				
		typedef std::stack<Ogre::Entity*> TAttachedMeshes;
		typedef std::map<std::string, TAttachedMeshes> TBoneObjectsTable;
			TBoneObjectsTable _boneObjectsTable;

		typedef std::deque<std::string> TAttachedMeshNames;
		typedef std::map<std::string, TAttachedMeshNames> TBoneObjectNamesTable;
			TBoneObjectNamesTable _boneObjectsNameTable;		

		void attach(const std::string &toBone, const std::string &mesh, bool permanently = false);
		void detach(const std::string &fromBone);	
		void _reloadAttachments();




	/****************
		MATERIALS
	***************/

	public:
		
		void setMaterial(const std::string &materialName, unsigned int subIndex = 0);
		inline void resetLastMaterial( unsigned int subIndex = 0) {
			if(_matListLast) setMaterial( _matListLast[ subIndex], subIndex ); 
		}
		void		setColor(const std::string& color	, unsigned int subEntityIndex = 0);
		void		setColor(const Vector3& color		, unsigned int subEntityIndex = 0);

	private:

		// FRS Hemos considerado que normalmente vamos tener materiales permanentes, cambios de
		// material permanentes y, quizá, algún cambio temporal. Si en un futuro necesitáramos
		// varios cambios consecutivos, habría que considerar guardar un _matListInit o que los
		// arrays se unificaran en una pila (imitando los attach).
		std::string* _matListLast;
		std::string* _matListCurrent;
	
		inline void _reloadMaterial( unsigned int subIndex = 0 ) const {
			assert(_entity && "La entidad grafica no ha sido cargada");
			if(_matListCurrent && _matListCurrent[ subIndex ].length() )
				_entity->getSubEntity(subIndex)->setMaterialName( _matListCurrent[ subIndex ] );
		}
		inline void _reloadMaterials() const {
			if(_matListCurrent) {
				const int N = _entity->getNumSubEntities();
				for(int i=0; i < N; ++i) _reloadMaterial(i);
			}
		} 
		

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
