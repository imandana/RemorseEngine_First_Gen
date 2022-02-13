#ifndef __REMORSE_GAME_FRAMEWORK_H__
#define __REMORSE_GAME_FRAMEWORK_H__

//////////////////////////* HEADER *//////////////////////////////////

/* Game Framework */
/* Used by This Derived for GameObject etc */
#include "h_MemoryPool.hpp"
#include "h_GameFramework.hpp"
#include "h_GameManager.hpp"

#include "../RemorseScripting/h_ScriptManager.hpp"

//////////////////////////* IMPLEMENTATION *//////////////////////////////////

/* Game Framework */
/* Used by This Derived for GameObject etc */
#include "s_MemoryPool.cpp"
#include "../RemorseScripting/s_ScriptManager.cpp"

#include "s_GameFramework.cpp"
#include "s_GameManager.cpp"

#endif /* __REMORSE_GAME_FRAMEWORK_H__ */

