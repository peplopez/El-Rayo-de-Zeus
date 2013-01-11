/**
@file Message.cpp

Contiene definición de funciones de serialización y deserialización de mensajes.

@see Logic::TMessage

@author David Llansó
@date Diciembre, 2010
*/

#include "Message.h"
#include "Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

namespace Logic 
{
	void Message::Serialize(const TMessage &message, Net::CBuffer &data)
	{
		
		data.write((void*) (&message._type), (size_t) sizeof(message._type));	// Serializamos el tipo
		
		// TODO TIPOS PRIMITIVOS: campos _int, _float y _bool.
		data.write((void*) (&message._int), (size_t) sizeof(message._int)); 
		data.write((void*) (&message._float), (size_t) sizeof(message._float));
		data.write((void*) (&message._bool), (size_t) sizeof(message._bool));

		// STRING: Para deserializar luego el string necesitaremos saber su longitud
		unsigned int size = message._string.size();  
		data.write((void*)(&size),(size_t) sizeof(size));			// 1º SIZE
		data.write((void*)(message._string.c_str()),(size_t) size); // 2º CHAR[]

		// Serializamos el campo con una posicion
		data.write((void*)(&message._vector3.x),(size_t) sizeof(message._vector3.x));
		data.write((void*)(&message._vector3.y),(size_t) sizeof(message._vector3.y));
		data.write((void*)(&message._vector3.z),(size_t) sizeof(message._vector3.z));

		/* TODO TRANSFORM serializar el campo _transform.
			Quizá solo habría que hacerlo si _type == SET_TRANSFORM? */
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				data.write((void*)(&message._transform[i][j]),(size_t) sizeof(message._transform[i][j]));
		/* ENTIDAD: Serializamos el campo con una entidad. 
					Lo que hacemos es  mandar el ID para su recuperación */
		Logic::TEntityID id;
			if(message._entity)
				id = message._entity->getEntityID();
			else
				id = EntityID::UNASSIGNED;
		data.write((void*)(&id),(size_t) sizeof(id));

	} // Serialize
	
	//---------------------------------------------------------

	void Message::Deserialize(Net::CBuffer &data, TMessage& message)
	{
		// Deserializamos el tipo
		data.read(&message._type,(size_t) sizeof(message._type));
		
		// TODO PRIMITIVOS: deserializar los campos _int, _float y _bool.
		data.read(&message._int,(size_t) sizeof(message._int));
		data.read(&message._float,(size_t) sizeof(message._float));
		data.read(&message._bool,(size_t) sizeof(message._bool));

		// STRING		
		unsigned int size;
			data.read(&size,(size_t) sizeof(size)); // Leemos longitud
		
		char* aux = new char[size];		// Reservamos bloque car[] de tamaño size
			data.read(aux,(size_t) size);
		message._string.assign(aux,size); // Init string cargando size caracteres de aux
			delete[] aux;

		// Deserializamos el campo con una posición
		data.read(&message._vector3.x,(size_t) sizeof(message._vector3.x));
		data.read(&message._vector3.y,(size_t) sizeof(message._vector3.y));
		data.read(&message._vector3.z,(size_t) sizeof(message._vector3.z));
		
		// TODO deserializar el campo _transform.
		// TODO reserva dinámica?
		// Deserializamos el campo con una transformada
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				data.read(&message._transform[i][j],(size_t) sizeof(message._transform[i][j]));
		// TODO ENTIDAD: Deserializar el campo con una entidad. Con el id sacarla del mapa		 
		//HACK 	// Si en algún momento hay más de un mapa habrá que modificar esto
		Logic::TEntityID id;
			data.read(&id, (size_t) sizeof(id));
			if(id == EntityID::UNASSIGNED)
				message._entity = 0;
			else
				message._entity = CServer::getSingletonPtr()->getMap()->getEntityByID(id);
	} // Deserialize

} // namespace Logic
