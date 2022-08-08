#ifndef __SCRIPT_MANAGER_HPP__
#define __SCRIPT_MANAGER_HPP__

#include <string>
#include <vector>

#include <angelscript.h>
#include <add_on/scripthandle/scripthandle.h>

#include <add_on/scripthandle/scripthandle.cpp>

/* class CGameObj; */

/* CScriptMgr */
class ScriptManager
{
public:
	ScriptManager();
	~ScriptManager();

	int Initialization();

	void CreateController(Script* ctrl, GameObject* gameObj);
    Script* GetControllerScript(const std::string &type, GameObject* gameObj);
    
    void CallStart(asIScriptObject *object);
    void CallUpdate(asIScriptObject *object);
  
	bool hasCompileErrors;

protected:
	void MessageCallback(const asSMessageInfo &msg);
	asIScriptContext *PrepareContextFromPool(asIScriptFunction *func);
	void ReturnContextToPool(asIScriptContext *ctx);
	int ExecuteCall(asIScriptContext *ctx);

    void RegisterGameObject();
    void RegisterComponent();
    
    void RegisterTheComponents();
    
    void RegisterGameManager();
    
    void RegisterVector2f();
    void RegisterIntRect();
    void RegisterTransformable();
    
    void RegisterAudio();
    void RegisterSFMLEvents();
    void RegisterKeyboardInput();
    
static void ScriptManager::ConstructMusic(Music* ptr);
    
static void ConstructVector2f(Vector2f* ptr);
static void ConstructVector2fCopy(const Vector2f& vector, Vector2f* ptr);
static void ConstructVector2fInit(float x, float y, Vector2f* ptr);
static void ConstructTransformable(Transformable* ptr);

	asIScriptEngine  *engine;

	// Our pool of script contexts. This is used to avoid allocating
	// the context objects all the time. The context objects are quite
	// heavy weight and should be shared between function calls.
	std::vector<asIScriptContext *> contexts;

	// This is the cache of function ids etc that we use to avoid having
	// to search for the function ids everytime we need to call a function.
	// The search is quite time consuming and should only be done once.
	/* std::vector<SController *> controllers;  */
    std::vector<Script*> controllers;
};

/* extern ScriptManager *scriptMgr; */

#endif /* __SCRIPT_MANAGER_HPP__ */


