/**
@file Entity.cpp

Contiene la implementaci�n de la clase que representa una entidad leida 
del fichero del mapa.

@author David Llans� Garc�a
@date Agosto, 2010
*/
#include "Entity.h"

#include <cassert>

namespace Map {
	
	typedef std::pair<std::string, std::string> TSSPar;

	void CEntity::setAttribute(const std::string &attr, const std::string &value)
	{	
		_attributes[attr] = value;
	} // setAttribute

	//--------------------------------------------------------

	bool CEntity::hasAttribute(const std::string &attr) const
	{
		// Casos especiales
		if(!attr.compare("name"))
			return true;
		if(!attr.compare("type"))
			return true;
		if(_attributes.count(attr))
			return true;
		return false;

	} // hasAttribute

	//--------------------------------------------------------

	const std::string &CEntity::getStringAttribute(const std::string &attr) const
	{
		// Casos especiales
		if(!attr.compare("name"))
			return _name;
		if(!attr.compare("type"))
			return _type;
		return (*_attributes.find(attr)).second;

	} // getStringAttribute

	//--------------------------------------------------------

	int CEntity::getIntAttribute(const std::string &attr) const
	{
		return atoi((*_attributes.find(attr)).second.c_str());

	} // getIntAttribute

	//--------------------------------------------------------

	float CEntity::getFloatAttribute(const std::string &attr) const
	{
		return (float)atof((*_attributes.find(attr)).second.c_str());

	} // getFloatAttribute

	//--------------------------------------------------------

	double CEntity::getDoubleAttribute(const std::string &attr) const
	{
		return atof((*_attributes.find(attr)).second.c_str());

	} // getDoubleAttribute

	//--------------------------------------------------------

	bool CEntity::getBoolAttribute(const std::string &attr) const
	{
		if(!(*_attributes.find(attr)).second.compare("true"))
			return true;
		else if(!(*_attributes.find(attr)).second.compare("false"))
			return false;
		throw new std::exception("Leido archivo booleano que no es ni true ni false.");

	} // getBoolAttribute

	//--------------------------------------------------------

	const Vector3 CEntity::getVector3Attribute(const std::string &attr) const
	{
		// Recuperamos la cadena  "x y z"
		std::string position = (*_attributes.find(attr)).second;
		int space1 = position.find(' ');
		float x = (float)atof(position.substr(0,space1).c_str());
		int space2 = position.find(' ',space1+1);
		float y = (float)atof(position.substr(space1+1,space2-(space1+1)).c_str());
		float z = (float)atof(position.substr(space2+1,position.size()-(space2+1)).c_str());

		return Vector3(x,y,z);

	} // getVector3Attribute

	
	//--------------------------------------------------------
	const Float4 CEntity::getFloat4Attribute(const std::string &attr) const
	{
		// Recuperamos la cadena  "x y z w"
		std::string position = (*_attributes.find(attr)).second;
		int space1 = position.find(' ');
		float x = (float)atof(position.substr(0,space1).c_str());
		int space2 = position.find(' ',space1+1);
		float y = (float)atof(position.substr(space1+1,space2-(space1+1)).c_str());
		int space3 = position.find(' ',space2+1);
		float z = (float)atof(position.substr(space2+1,space3-(space2+1)).c_str());
		float w = (float)atof(position.substr(space3+1,position.size()-(space3+1)).c_str());
		return Float4(x,y,z,w);
	} // getVector4Attribute

	//--------------------------------------------------------

	void CEntity::mergeWithArchetype(const CEntity& archetype)
	{		
		// UNDONE FRS Es m�s sencillo utilizar insert directamente, el cual 
		// inserta tan s�lo los pares cuya clave no est� registrada ya en _attributes.
		// Tampoco hay que temer por "name" o "type", ya que el parser no las almacena 
		// en _attributes, si no en variables independientes (_name y _type).

		//TAttrList::const_iterator it	= archetype.getAttributes().cbegin();
		//TAttrList::const_iterator end	= archetype.getAttributes().cend();		
		//	for (; it != end; ++it)	
		//		if( !this->hasAttribute(it->first) )			// UNDONE FRS it->first != "type": el 
		//			this->setAttribute(it->first, it->second);	//parser no guarda "name" ni "type" en Attributes.

		_attributes.insert(archetype.getAttributes().cbegin(), archetype.getAttributes().cend() );

		//Por ultimo se modifica el tipo de la entidad al que se indique en el arquetipo
		this->setType(archetype.getType());

	} // mergeWithArchetype

	//--------------------------------------------------------

	void CEntity::replaceAttrKeywords(
			const TAttrKeywords& keywords, const std::string& attr)
	{				
		if( keywords.empty() )
			return;
		
		// ATTRIBUTO ESPEC�FICO
		if( attr.length() ) { // Reemplazo del valor de un atributo espec�fico
		
			if( attr == "name") {
				_replaceKeywords(_name, keywords);
			}else if( attr == "type") {
				_replaceKeywords(_type, keywords);
		
			} else { // Resto de atributos		

				// B�SQUEDA Y REEMPLAZO
				TAttrList::iterator       attrIt	= _attributes.find(attr);			
				TAttrList::const_iterator attrEnd	= _attributes.cend();
				if(attrIt != attrEnd) // Atributo encontrado					
					_replaceKeywords(attrIt->second, keywords);
			}
		
		// TODOS LOS ATRIBUTOS
		} else { // Reemplazo de todos los atributos

			_replaceKeywords(_name, keywords);
			_replaceKeywords(_type, keywords);

			TAttrList::iterator       attrIt	= _attributes.begin();			
			TAttrList::const_iterator attrEnd	= _attributes.cend();
			for (; attrIt != attrEnd; ++attrIt) 
				_replaceKeywords(attrIt->second, keywords);
		}
				
	} // replaceAttrKeywords

	//--------------------------------------------------------

	void CEntity::_replaceKeywords(std::string& text, const TAttrKeywords& keywords)
	{
		TAttrKeywords::const_iterator keyIt 	= keywords.cbegin();
		TAttrKeywords::const_iterator keyEnd	= keywords.cend();
		for(; keyIt != keyEnd; ++keyIt) {

			const std::string& keyword  = keyIt->first;
			const std::string& value	= keyIt->second;

			size_t start_pos = text.find( keyword );
				if(start_pos != std::string::npos)       
					text.replace(start_pos, keyword.length(), value);
		}

	} // replaceKeywords

} // namespace Map