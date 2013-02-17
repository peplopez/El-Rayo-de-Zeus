#include "Rtti.h"
#include "Logic/Entity/Components/AngularMovement.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Components/Camera.h"
#include "Logic/Entity/Components/Death.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/Life.h"
#include "Logic/Entity/Components/LifeController.h"
#include "Logic/Entity/Components/NetConnector.h"


template <> const char* GetTypeName<int>() { return "int"; }
template <> const char* GetTypeName<Logic::CAngularMovement>() { return "CAngularMovement"; }
template <> const char* GetTypeName<Logic::CAnimatedGraphics>() { return "CAnimatedGraphics"; }
template <> const char* GetTypeName<Logic::CAvatarController>() { return "CAvatarController"; }
template <> const char* GetTypeName<Logic::CCamera>() { return "CCamera"; }
template <> const char* GetTypeName<Logic::CCollider>() { return "CCollider"; }
template <> const char* GetTypeName<Logic::CDeath>() { return "CDeath"; }
template <> const char* GetTypeName<Logic::CGraphics>() { return "CGraphics"; }
template <> const char* GetTypeName<Logic::CLife>() { return "CLife"; }
template <> const char* GetTypeName<Logic::CLifeController>() { return "CLifeController"; }
template <> const char* GetTypeName<Logic::CNetConnector>() { return "CNetConnector"; }