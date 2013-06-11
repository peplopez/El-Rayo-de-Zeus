#ifndef __Logic_MessageAudio_H
#define __Logic_MessageAudio_H

#include "Net/buffer.h"
#include "Message.h"
#include "BaseSubsystems/Math.h"
// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaración de la clase
	class CMessageAudio : public CMessage
	{
		DEC_MFACTORY(CMessageAudio);
	public:


		CMessageAudio() : CMessage(GetAltTypeIdOf(CMessageAudio)), _path(""), _id(""), _position(Vector3::ZERO), _notIfPlay(false), _isPlayer(false){}	
		~CMessageAudio() {}

		std::string getPath() const { return _path; }
		void setPath(const std::string& value) {_path=value; }

		std::string getId() const { return _id; }
		void setId(const std::string& value) {_id=value; }

		Vector3 CMessageAudio::getPosition(){return _position;}		
		void CMessageAudio::setPosition(Vector3 position){_position=position;}

		bool getNotIfPlay() const { return _notIfPlay;}
		void setNotIfPlay(const bool notIfPlay){_notIfPlay=notIfPlay;}

		bool getIsPlayer() const {return _isPlayer;}
		void setIsPlayer(const bool isPlayer){_isPlayer=isPlayer;}

		void serialize(Net::CBuffer &data);
		void deserialize(Net::CBuffer &data);	

	private:
		std::string _path;
		std::string _id;
		Vector3 _position;
		bool _notIfPlay;
		bool _isPlayer;

	}; //class CmessageBool

	REG_MFACTORY(CMessageAudio);

} // namespace Logic

#endif // __Logic_Message_H