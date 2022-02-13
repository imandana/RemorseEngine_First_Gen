#ifndef __REMORSE_WINDOW_FRAMEWORK_H__
#define __REMORSE_WINDOW_FRAMEWORK_H__

//////////////////////////* HEADER *//////////////////////////////////
/* Window Framework */
#include "h_ControllBase.hpp"
#include "h_ControllManager.hpp"
#include "h_MenuBarControll.hpp"
#include "h_WindowControll.hpp"
#include "h_MemoryPool.hpp"
#include "h_Canvas.hpp"

/* Game Framework */
/* Used by This Derived for GameObject etc */
#include "h_GameFramework.hpp"

/* Derived Class */
#include "h_ObjectWindowCanvas.hpp"
#include "h_ObjectWindowView.hpp"


//////////////////////////* IMPLEMENTATION *//////////////////////////////////
/* Window Framework */
#include "s_ControllBase.cpp"
#include "s_ControllManager.cpp"
#include "s_MenuBarControll.cpp"
#include "s_WindowControll.cpp"
#include "s_MemoryPool.cpp"
#include "s_Canvas.cpp"

/* Game Framework */
/* Used by This Derived for GameObject etc */
#include "s_GameFramework.cpp"

/* Derived Class */
#include "s_ObjectWindowCanvas.cpp"
#include "s_ObjectWindowView.cpp"

#endif /* __REMORSE_WINDOW_FRAMEWORK_H__ */