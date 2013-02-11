/**
@file Message.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::TMessage

@author ESC
@date Enero, 2013
*/


#include "Message.h"
#include "Logic/Entity/Entity.h"


namespace Logic 
{
	IMP_MFACTORY(CMessage);
	//---------------------------------------------------------
		
	CMessage::~CMessage() {/*std::cout << this << " no se necesita más" << std::endl;*/}

	//---------------------------------------------------------
			
	void CMessage::grab() const {++_refCount;}

	//---------------------------------------------------------

	void CMessage::release() const
	{
		assert(_refCount > 0);
		--_refCount;

		if(_refCount == 0) {delete this;}
		
	} // release	

	//---------------------------------------------------------

	void CMessage::serialize(Net::CBuffer &data) const
	{
		data.write((void*) (&_classId),	sizeof(_classId));	
		data.write((void*) (&_type),	sizeof(_type));	
		data.write((void*) (&_action),	sizeof(_action));
		data.write((void*) (&_refCount),		sizeof(_refCount)); 
		
	} // Serialize
	
	//---------------------------------------------------------

	void CMessage::deserialize(Net::CBuffer &data)
	{
		//El _classId ya habrá sido deserializado para uso de la Factoría de mensajes.
		data.read(&_type, sizeof(_type));
		data.read(&_action, sizeof(_action));
		data.read(&_refCount,   sizeof(_refCount));

	} // Deserialize

} // namespace Logic
