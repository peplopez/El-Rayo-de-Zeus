/**
@file PlayerSettings.h

Contiene la declaración de la clase que guarda los ajustes de configuración
iniciales de cada jugador (modelo, color, etc).

@author FRS
@date Septiembre, 2013
*/
#ifndef __Logic_PlayerSettings_H
#define __Logic_PlayerSettings_H


#include <map/Entity.h> // HACK FRS Tener que importar todo el Entity.h tan solo por el TAttrKeywords es un poco... 
#include <net/buffer.h>
#include <net/serializable.h>

#include <set>
#include <vector>

#define KEYWORD_AVATAR  "AVATAR"
#define KEYWORD_COLOR   "COLOR"
#define KEYWORD_CONTROL "CONTROL"
#define KEYWORD_LOCAL   "LOCAL"
#define KEYWORD_NICK    "NICK"
#define KEYWORD_SCENE   "SCENE"

namespace Logic
{
	// TODO FRS Esto estaría mejor si fuera configurable desde datos
	namespace Player
	{
		namespace Avatar {
			enum TAvatar : unsigned char {
				SPARTAN		= 0,
				ATALANTA	= 1,
				_COUNT		= 2
			};
		}
		namespace Color {
			enum TColor : unsigned char {
				RED		= 0,
				GREEN	= 1,
				BLUE	= 2,
				YELLOW	= 3,
				_COUNT	= 4
			};		
		}		
		namespace Stage {
			enum TStage: unsigned char {
				SPARTAN	= 0,
				HADES	= 1,
				_COUNT	= 2
			};
		}

		namespace Climatology {
			enum TClimatology: unsigned char {
				STORM1	= 0,
				STORM2	= 1,
				DESERT	= 2,
				SUNSET	= 3,
				CLEAR	= 4,
				NIGHT	= 5,
				_COUNT	= 6
			};
		}
		
		const std::string COLOR_STRINGS[] = {
			"Red",
			"Green",
			"Blue",
			"Yellow"
		};
		const std::string AVATAR_STRINGS[] = {
			"Spartan",
			"Atalanta"
		};	
		const std::string STAGE_SCENES[] = {
			"Spartan",
			"Hades"
		};

		const std::string CLIMATOLOGY_STRINGS[] = {
			"Thunderstorm1",
			"Thunderstorm2",
			"Desert",
			"Sunset",
			"Clear",
			"Night"
		};
	}  // namespace Player

	
	typedef Player::Color::TColor	TPlayerColor;
	typedef std::set<TPlayerColor> TColors;
	typedef Player::Avatar::TAvatar	TPlayerAvatar;
	typedef Player::Stage::TStage	TPlayerStage;
	typedef Player::Stage::TStage	TPlayerClimatology;

	
	class CPlayerSettings : Net::Serializable
	{

	public:

		static CPlayerSettings createRandSettings(bool isNPC = true);

		CPlayerSettings(const std::string& nick, 
			bool isLocalPlayer = false,
			bool isNPC = true,
			TPlayerColor color = Player::Color::RED, 
			TPlayerAvatar avatar = Player::Avatar::SPARTAN,
			TPlayerStage stage = Player::Stage::SPARTAN);

			  
		//--------- SERIALIZABLE ------------------
		/**
		 * Serializar objeto en buffer
		*/
		virtual void serialize(Net::CBuffer& buffer);

		/**
		 * Leer objeto desde buffer
		 */
		virtual void deserialize(Net::CBuffer& buffer);


		// ---------------- GET's ---------------------

		TPlayerAvatar		getAvatar()			const	{	return _avatar; }		
		TPlayerColor		getColor()			const	{	return _color;	}		
		const std::string&	getNick()			const	{	return _nick;	}		
		TPlayerStage		getStage()			const	{	return _stage; }	
		bool				isNPC()				const	{	return _isNPC; }		
		bool				isLocalPlayer()		const	{	return _isLocalPlayer; }

		const std::string&  getMapName()		const	{	return _mapName; }
		const Map::TAttrKeywords& getMapProperties();
		static void resetUsedColors() { _colorsNotUsed = _initColorsNotUsed(); }

		
		// ----------------- SET's ---------------------
		static void setLowQMode(bool lowQ) { CPlayerSettings::_isLowQMode = lowQ; }
		
		
	private:

		static TColors	_colorsNotUsed;
		static TColors	_initColorsNotUsed();
		static std::string	_nextRandNick;
		static bool _isLowQMode;
		
		TPlayerAvatar		_avatar;		
		TPlayerColor		_color;			
		bool				_isLocalPlayer;
		bool				_isNPC;		
		std::string			_nick;
		TPlayerStage		_stage;	

		std::string			_mapName;
		Map::TAttrKeywords  _mapProperties;

		// TODO FRS _material ? (wounds, armor...)

	}; // cllas CSettings



	typedef std::vector<CPlayerSettings> TMultiSettings;

} // namespace Logic

#endif //__Logic_Player_Settings_H