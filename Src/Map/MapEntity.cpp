/**
@file Entity.cpp

Contiene la implementación de la clase que representa una entidad leida 
del fichero del mapa.

@author David Llansó García
@date Agosto, 2010
*/
#include "MapEntity.h"

#include <cassert>

namespace Map {
	
	typedef std::pair<std::string, std::string> TSSPar;

	void CEntity::setAttribute(const std::string &attr, const std::string &value)
	{
		TSSPar elem(attr,value);
		if(_attributes.count(attr))
			_attributes.erase(attr);
		_attributes.insert(elem);

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
		
		TAttrList::const_iterator it,end;

		it=archetype.getAttributes().begin();
		end=archetype.getAttributes().end();
		
		for (; it != end; it++)
		{
			if(!this->hasAttribute(it->first) && it->first.compare("type") != 0)
				this->setAttribute(it->first, it->second);
		}

		//Por ultimo se modifica el tipo de la entidad al que se indique en el arquetipo
		this->setType(archetype.getType());

	} // mergeWithArchetype

} // namespace Map
