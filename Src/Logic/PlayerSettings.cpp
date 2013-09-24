/**
@file PlayerSettings.cpp

Contiene la implementación de la clase que guarda los ajustes de configuración
iniciales de cada jugador (modelo, color, etc).

@author FRS
@date Septiembre, 2013
*/

#include "PlayerSettings.h"
#include <assert.h>

namespace Logic
{
	
	std::string CPlayerSettings::_nextRandNick = "PlayerRandom0";
	TColors CPlayerSettings::_colorsNotUsed = _initColorsNotUsed();
	bool CPlayerSettings::_isLowQMode = false;


	TColors CPlayerSettings::_initColorsNotUsed()
	{
		TColors colors;
		for(int i = 0; i < Player::Color::_COUNT; ++i)
			colors.insert( static_cast<TPlayerColor>(i) );
		return colors;
	}

	//---------------------------------------------------------

	CPlayerSettings::CPlayerSettings(
		const std::string& nick, bool isLocalPlayer, bool isNPC,
		TPlayerColor color,	TPlayerAvatar avatar, TPlayerStage stage) :
			_nick(nick), _isLocalPlayer(isLocalPlayer), _isNPC(isNPC),
			_color(color), _avatar(avatar), _stage(stage), 
			_mapName("map")
	{ 
		assert( !(_isLocalPlayer & _isNPC) && "Configuracion incompatible!");
		assert( _colorsNotUsed.count( color ) && "El color está en uso" ); 
		_colorsNotUsed.erase(color);
		_mapName.append( Player::COLOR_STRINGS[_color] );
	}
			
	//---------------------------------------------------------

	// FRS Nunca crearemos random settings para jugadores remotos 
	// (nos enviarán sus settings serializados).
	// Por tanto, en el contexto de esta función asumimos isLocalPlayer = !isNPC
	CPlayerSettings CPlayerSettings::createRandSettings(bool isNPC)
	{
		++_nextRandNick[ _nextRandNick.length() - 1 ];
	
		return CPlayerSettings(
			_nextRandNick,
			!isNPC,
			 isNPC,
			*_colorsNotUsed.cbegin(), 
			static_cast<TPlayerAvatar>	( rand() % Player::Avatar::_COUNT ), 
			static_cast<TPlayerStage>	( rand() % Player::Stage::_COUNT  )
		);
	} // createRandSettings


	//---------------------------------------------------------

	const Map::TAttrKeywords& CPlayerSettings::getMapProperties()
	{
		if( _mapProperties.empty() ) {
			_mapProperties[KEYWORD_AVATAR]	= "Avatar" + Player::AVATAR_STRINGS[_avatar];
			_mapProperties[KEYWORD_COLOR]	=  Player::COLOR_STRINGS[_color];
			_mapProperties[KEYWORD_CONTROL]	= _isNPC? "NPC" : "Player";
			_mapProperties[KEYWORD_LOCAL]	= _isLocalPlayer? "true" : "false";
			_mapProperties[KEYWORD_NICK]	= _nick;
			if (_isLowQMode)
				_mapProperties[KEYWORD_SCENE]	= "Scene" + Player::STAGE_SCENES[_stage] + "LowQ";	
			else
				_mapProperties[KEYWORD_SCENE]	= "Scene" + Player::STAGE_SCENES[_stage];
		}
		return _mapProperties;

	} // getMapProperties

	//---------------------------------------------------------


	/******************
		SERIALIZABLE
	*******************/
	/**
	* Serializar objeto en buffer
	*/
	void CPlayerSettings::serialize(Net::CBuffer& buffer){}

	//---------------------------------------------------------

	/**
	* Leer objeto desde buffer
	*/
	void CPlayerSettings::deserialize(Net::CBuffer& buffer){}

} // namespace Logic
