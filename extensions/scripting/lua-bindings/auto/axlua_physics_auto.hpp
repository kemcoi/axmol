#include "base/Config.h"
#if AX_USE_PHYSICS
#ifndef __ax_physics_h__
#define __ax_physics_h__
#include "tolua++.h"
int register_all_ax_physics(lua_State* tolua_S);
#endif // __ax_physics_h__
#endif //#if AX_USE_PHYSICS
