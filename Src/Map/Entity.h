/**
@file Entity.h

Contiene la declaración de la clase que representa una entidad leida 
del fichero del mapa.

@see Map::CEntity

@author David Llansó García
@date Agosto, 2010
*/

#ifndef __Map_Entity_H
#define __Map_Entity_H

#include <BaseSubsystems/Math.h>
#include <string>
#include <map>

namespace Map 
{
	typedef std::map<std::string, std::string> TAttrKeywords;

	/**
	Clase que representa una entidad leida del fichero del mapa. Es
	simplemente una colección de pares <atributo,valor> más un string
	con el tipo de la entidad. 
	<p>
	La clase contiene métodos accesores que devuelven un atributo 
	convertido al tipo de datos requerido. El demandante es responsable 
	de solicitar el atributo por el método accesor adecuado, si el
	atributo no existe o no se puede convertir en el tipo de datos deseado
	salta una excepción. Acepta extraer strings, enteros, flotantes, 
	flotantes de doble precisión, booleanos o posiciones (éstas deben
	seguir el formato "(x,y,z)").

	@ingroup mapParserGroup
	@ingroup mapGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CEntity 
	{

	public:

		CEntity(): _name(""), _type("") {}
		/**
		Constructor de la clase.
		*/
		CEntity(const std::string &name) : _name(name), _type("") {}

		/**
		Destructor.
		*/
		~CEntity() {}

		/**
		Añade un atributo a la lista de atributos de la entidad.

		@param attr Nombre del atributo a añadir.
		@param value Valor del atributo.
		*/
		void setAttribute(const std::string &attr, const std::string &value);

		/**
		Dice si se ha añadido un atributo.

		@param attr Nombre del atributo.
		@return true si contiene el atributo.
		*/
		bool hasAttribute(const std::string &attr) const;

		/**
		Recupera un atributo previamente añadido. Si no existe devuelve "".

		@param attr Nombre del atributo a recuperar.
		@return Valor del atributo.
		*/
		const std::string &getStringAttribute(const std::string &attr) const;

		/**
		Recupera un atributo previamente añadido de tipo entero. Si no existe 
		salta una excepción.

		@param attr Nombre del atributo a recuperar.
		@return Valor del atributo.
		*/
		int getIntAttribute(const std::string &attr) const;

		/**
		Recupera un atributo previamente añadido de tipo flotante. Si no existe 
		salta una excepción.

		@param attr Nombre del atributo a recuperar.
		@return Valor del atributo.
		*/
		float getFloatAttribute(const std::string &attr) const;

		/**
		Recupera un atributo previamente añadido de tipo double. Si no existe 
		salta una excepción.

		@param attr Nombre del atributo a recuperar.
		@return Valor del atributo.
		*/
		double getDoubleAttribute(const std::string &attr) const;

		/**
		Recupera un atributo previamente añadido de tipo booleano. Si no existe 
		salta una excepción.

		@param attr Nombre del atributo a recuperar.
		@return Valor del atributo.
		*/
		bool getBoolAttribute(const std::string &attr) const;

		/**
		Recupera un atributo previamente añadido de tipo posición. Si no existe 
		salta una excepción. Las posiciones deben tener el formato "(x,y,z)".

		@param attr Nombre del atributo a recuperar.
		@param x Valor del eje X.
		@param y Valor del eje Y.
		@param z Valor del eje Z.
		*/
		const Vector3 getVector3Attribute(const std::string &attr) const;

		
		/**
		Recupera un atributo previamente añadido de tipo posición. Si no existe 
		salta una excepción. Las posiciones deben tener el formato "(x,y,z,w)".

		@param attr Nombre del atributo a recuperar.
		@param x Valor del float x.
		@param y Valor del float y.
		@param z Valor del float z.
		@param w Valor del float w.
		*/

		const Float4 getFloat4Attribute(const std::string &attr) const;

		/**
		Devuelve un string con el tipo de la entidad.

		@return Tipo de la entidad.
		*/
		const std::string &getType() const {return _type;}

		/**
		Establece el tipo de la entidad.

		@param type Tipo de la entidad.
		*/
		void setType(const std::string &type) {_type = type;}

		/**
		Devuelve un string con el nombre de la entidad.

		@return Nombre de la entidad.
		*/
		const std::string &getName() const {return _name;}

		/**
		Establece el nombre de la entidad.

		@param name Nombre de la entidad.
		*/
		void setName(const std::string &name) {_name = name;}

		/**
		Add - ESC 
		Fusiona los atributos de la entidad con los del 
		arquetipo pasado como parametro. Los atributos de la entidad no 
		se sobreescriben.La única expceción a esto es el tipo de la entidad, 
		que si es sobreescrito por el del archetype.

		@param  archetype a fusionar
		*/

		void mergeWithArchetype(const CEntity &archetype);
		void replaceAttrKeywords(
			const TAttrKeywords& properties, const std::string& attribute = "");



		/**
		Tipo lista de CEntity donde guardaremos los pendientes de borrar.
		*/
		typedef std::map<std::string, std::string> TAttrList;

		/**
		Add - ESC
		Devuelve un mapa con la lista de pares (atributo,valor) de la entidad
		*/

		const TAttrList& getAttributes() const {return _attributes;}


		
		
	

	protected:

		/**
		Atributos de la entidad.
		*/
		TAttrList _attributes;

		/**
		Tipo de la entidad.
		*/
		std::string _type;

		/**
		Nombre de la entidad.
		*/
		std::string _name;


	private:
		void _replaceKeywords(std::string& text, const TAttrKeywords& keywords);


	}; // CEntity

	

} // namespace Map

#endif // __Map_Entity_H
