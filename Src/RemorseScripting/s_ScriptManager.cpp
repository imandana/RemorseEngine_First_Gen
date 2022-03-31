#include <iostream>  // cout
#include <stdio.h>  // fopen, fclose
#include <string.h> // strcmp
#include <assert.h>

#include "add_on/scriptarray/scriptarray.h"
#include "add_on/scriptstdstring/scriptstdstring.h"
#include "add_on/scriptbuilder/scriptbuilder.h"
#include "add_on/weakref/weakref.h"


#include "add_on/scriptarray/scriptarray.cpp"
#include "add_on/scriptstdstring/scriptstdstring_utils.cpp"
#include "add_on/scriptstdstring/scriptstdstring.cpp"
#include "add_on/scriptbuilder/scriptbuilder.cpp"
#include "add_on/weakref/weakref.cpp"

using namespace std;

/* CScriptMgr::CScriptMgr() */
ScriptManager::ScriptManager()
{
	engine           = 0;
	hasCompileErrors = false;
}

ScriptManager::~ScriptManager()
{
/* 	for( unsigned int n = 0; n < controllers.size(); n++ )
		delete controllers[n]; */

	for( unsigned int n = 0; n < contexts.size(); n++ )
		contexts[n]->Release();

	if( engine )
		engine->ShutDownAndRelease();
}

int ScriptManager::Initialization()
{
	int r;
	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	// Set the message callback to print the human readable messages that the engine gives in case of errors
	r = engine->SetMessageCallback(asMETHOD(ScriptManager,MessageCallback), this, asCALL_THISCALL); assert( r >= 0 );
	// Register the string type
	RegisterStdString(engine);

	// Register the generic handle type, called 'ref' in the script
	RegisterScriptHandle(engine);

	// Register the weak ref template type
	RegisterScriptWeakRef(engine);

    RegisterVector2f();
    RegisterTransformable(); 
    RegisterAudio();
    RegisterKeyboardInput();
    
	// The game engine will determine the class that represents the controller
	// by checking if the class implements the IController interface. No methods
	// are registered for this interface, as the script shouldn't be required to
	// implement the methods. This will allow the game engine to avoid calling
	// methods that doesn't do anything, thus improving performance.
	r = engine->RegisterInterface("IRemorseBehaviour"); assert( r >= 0 );



	// Register the game manager as a singleton. The script will access it through the global property
	r = engine->RegisterObjectType("GameManager", 0, asOBJ_REF | asOBJ_NOHANDLE); assert( r >= 0 );

	// Register the game manager's methods
	r = engine->RegisterGlobalProperty("GameManager game", globalGame.gameMgr); assert( r >= 0 );

	// The script can determine what the user wants to do through the actionStates
	/* r = engine->RegisterObjectMethod("GameManager", "bool get_actionState(int idx)", asMETHOD(GameManager, GetActionState), asCALL_THISCALL); assert( r >= 0 ); */

	// The script can call this method to end the game
	//r = engine->RegisterObjectMethod("GameManager", "void EndGame(bool win)", asMETHOD(GameManager, EndGame), asCALL_THISCALL); assert( r >= 0 );
	
	// Register a method that will allow the script to find an object by its name.
	// This returns the object as const handle, as the script should only be 
	// allow to directly modify its owner object.
	// Observe the @+ that tells AngelScript to automatically increase the refcount
	r = engine->RegisterObjectMethod("GameManager", "GameObject @+ GetGameObjectByName(const string &in name)", asMETHOD(GameManager, GetGameObjectByName), asCALL_THISCALL); assert( r >= 0 );
    
    r = engine->RegisterObjectMethod("GameManager", "const GameObject @+ GetComponentByName(const string &in name)", asMETHOD(GameManager, GetComponentByName), asCALL_THISCALL); assert( r >= 0 );


	return 0;
}

void ScriptManager::RegisterGameObject()
{
	// Register the game object. The scripts cannot create these directly, so there is no factory function.
	r = engine->RegisterObjectType("GameObject", 0, asOBJ_REF); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("GameObject", asBEHAVE_ADDREF, "void f()", asMETHOD(GameObject, AddRef), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("GameObject", asBEHAVE_RELEASE, "void f()", asMETHOD(GameObject, Release), asCALL_THISCALL); assert( r >= 0 );

    r = engine->RegisterObjectMethod("GameObject", "Vector2f GetPosition()", asMETHOD(GameObject, GetPosition), asCALL_THISCALL); assert( r >= 0 );
    
    r = engine->RegisterObjectProperty("GameObject", "Transformable @ transform", offsetof(GameObject, transform)); assert( r >= 0 );
}

void ScriptManager::RegisterComponent()
{
	r = engine->RegisterObjectType("Component", 0, asOBJ_REF); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Component", asBEHAVE_ADDREF, "void f()", asMETHOD(Component, AddRef), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Component", asBEHAVE_RELEASE, "void f()", asMETHOD(Component, Release), asCALL_THISCALL); assert( r >= 0 );

}

static void FakeAddRef(void* ptr)
{
}
static void FakeReleaseRef(void* ptr)
{
}
void ScriptManager::RegisterTheComponents()
{
    engine->RegisterObjectType("FrameAnimation", 0, asOBJ_REF);
    engine->RegisterObjectBehaviour("FrameAnimation", asBEHAVE_ADDREF, "void f()", asFUNCTION(FakeAddRef), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("FrameAnimation", asBEHAVE_RELEASE, "void f()", asFUNCTION(FakeReleaseRef), asCALL_CDECL_OBJLAST);

    engine->RegisterObjectType("ShaderScript", 0, asOBJ_REF);
    engine->RegisterObjectBehaviour("ShaderScript", asBEHAVE_ADDREF, "void f()", asFUNCTION(FakeAddRef), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("ShaderScript", asBEHAVE_RELEASE, "void f()", asFUNCTION(FakeReleaseRef), asCALL_CDECL_OBJLAST);

    engine->RegisterObjectType("Shadow2D", 0, asOBJ_REF);
    engine->RegisterObjectBehaviour("Shadow2D", asBEHAVE_ADDREF, "void f()", asFUNCTION(FakeAddRef), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("Shadow2D", asBEHAVE_RELEASE, "void f()", asFUNCTION(FakeReleaseRef), asCALL_CDECL_OBJLAST);

    engine->RegisterObjectType("BoxCollision", 0, asOBJ_REF);
    engine->RegisterObjectBehaviour("BoxCollision", asBEHAVE_ADDREF, "void f()", asFUNCTION(FakeAddRef), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("BoxCollision", asBEHAVE_RELEASE, "void f()", asFUNCTION(FakeReleaseRef), asCALL_CDECL_OBJLAST);

    engine->RegisterObjectType("SubEntities", 0, asOBJ_REF);
    engine->RegisterObjectBehaviour("SubEntities", asBEHAVE_ADDREF, "void f()", asFUNCTION(FakeAddRef), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("SubEntities", asBEHAVE_RELEASE, "void f()", asFUNCTION(FakeReleaseRef), asCALL_CDECL_OBJLAST);

    engine->RegisterObjectType("EntityStates", 0, asOBJ_REF);
    engine->RegisterObjectBehaviour("EntityStates", asBEHAVE_ADDREF, "void f()", asFUNCTION(FakeAddRef), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("EntityStates", asBEHAVE_RELEASE, "void f()", asFUNCTION(FakeReleaseRef), asCALL_CDECL_OBJLAST);

    engine->RegisterObjectType("SolidCollide", 0, asOBJ_REF);
    engine->RegisterObjectBehaviour("SolidCollide", asBEHAVE_ADDREF, "void f()", asFUNCTION(FakeAddRef), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("SolidCollide", asBEHAVE_RELEASE, "void f()", asFUNCTION(FakeReleaseRef), asCALL_CDECL_OBJLAST);
    
}

void ScriptManager::ConstructVector2f(Vector2f* ptr)
{
    new(ptr) Vector2f();
}

void ScriptManager::ConstructVector2fCopy(const Vector2f& vector, Vector2f* ptr)
{
    new(ptr) Vector2f(vector);
}

void ScriptManager::ConstructVector2fInit(float x, float y, Vector2f* ptr)
{
    new(ptr) Vector2f(x, y);
}


void ScriptManager::RegisterVector2f()
{
    engine->RegisterObjectType("Vector2f", sizeof(Vector2f), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<Vector2f>() | asOBJ_APP_CLASS_ALLINTS);
    engine->RegisterObjectBehaviour("Vector2f", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructVector2f), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("Vector2f", asBEHAVE_CONSTRUCT, "void f(const Vector2f&in)", asFUNCTION(ConstructVector2fCopy), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("Vector2f", asBEHAVE_CONSTRUCT, "void f(int, int)", asFUNCTION(ConstructVector2fInit), asCALL_CDECL_OBJLAST);
    /* Vector2<T>& left, const Vector2<T>& right); */
    engine->RegisterObjectMethod("Vector2f", "Vector2f& opAssign(const Vector2f&in)", asFUNCTION(ConstructVector2fCopy), asCALL_CDECL_OBJLAST);
/*     engine->RegisterObjectMethod("Vector2f", "Vector2f& opAddAssign(const Vector2f&in, const Vector2f&in2)", asMETHOD(Vector2f, operator +=), asCALL_THISCALL);
    engine->RegisterObjectMethod("Vector2f", "Vector2f& opSubAssign(const Vector2f&in, const Vector2f&in2)", asMETHOD(Vector2f, operator -=), asCALL_THISCALL);
    engine->RegisterObjectMethod("Vector2f", "Vector2f& opMulAssign(const Vector2f&in, float)", asMETHODPR(Vector2f, operator *=, (float), Vector2f&), asCALL_THISCALL);
    engine->RegisterObjectMethod("Vector2f", "Vector2f& opDivAssign(const Vector2f&in, float)", asMETHODPR(Vector2f, operator /=, (float), Vector2f&), asCALL_THISCALL);
    engine->RegisterObjectMethod("Vector2f", "Vector2f& opMulAssign(const Vector2f&in, const Vector2f&in2)", asMETHODPR(Vector2f, operator *=, (const Vector2f&), Vector2f&), asCALL_THISCALL);
    engine->RegisterObjectMethod("Vector2f", "Vector2f& opDivAssign(const Vector2f&in, const Vector2f&in2)", asMETHODPR(Vector2f, operator /=, (const Vector2f&), Vector2f&), asCALL_THISCALL);
     */
    engine->RegisterObjectProperty("Vector2f", "float x", offsetof(Vector2f, x));
    engine->RegisterObjectProperty("Vector2f", "float y", offsetof(Vector2f, y));
    
}

void ScriptManager::RegisterIntRect()
{
/*         engine->RegisterObjectType("IntRect", sizeof(IntRect), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<IntRect>() | asOBJ_APP_CLASS_ALLINTS);
    engine->RegisterObjectBehaviour("IntRect", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructIntRect), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("IntRect", asBEHAVE_CONSTRUCT, "void f(const IntRect&in)", asFUNCTION(ConstructIntRectCopy), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("IntRect", asBEHAVE_CONSTRUCT, "void f(int, int)", asFUNCTION(ConstructIntRectInit), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("IntRect", asBEHAVE_CONSTRUCT, "void f(int[]&)", asFUNCTION(ConstructIntRectArrayInit), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("IntRect", "int[]& get_data() const", asFUNCTION(IntRectData), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod("IntRect", "IntRect& opAssign(const IntRect&in)", asMETHOD(IntRect, operator =), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect& opAddAssign(const IntRect&in)", asMETHOD(IntRect, operator +=), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect& opSubAssign(const IntRect&in)", asMETHOD(IntRect, operator -=), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect& opMulAssign(int)", asMETHODPR(IntRect, operator *=, (int), IntRect&), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect& opDivAssign(int)", asMETHODPR(IntRect, operator /=, (int), IntRect&), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect& opMulAssign(const IntRect&in)", asMETHODPR(IntRect, operator *=, (const IntRect&), IntRect&), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect& opDivAssign(const IntRect&in)", asMETHODPR(IntRect, operator /=, (const IntRect&), IntRect&), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "bool opEquals(const IntRect&in) const", asMETHOD(IntRect, operator ==), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect opNeg() const", asMETHODPR(IntRect, operator -, () const, IntRect), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect opAdd(const IntRect&in) const", asMETHOD(IntRect, operator +), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect opSub(const IntRect&in) const", asMETHODPR(IntRect, operator -, (const IntRect&) const, IntRect), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect opMul(int) const", asMETHODPR(IntRect, operator *, (int) const, IntRect), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect opDiv(int) const", asMETHODPR(IntRect, operator /, (int) const, IntRect), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect opMul(const IntRect&in) const", asMETHODPR(IntRect, operator *, (const IntRect&) const, IntRect), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "IntRect opDiv(const IntRect&in) const", asMETHODPR(IntRect, operator /, (const IntRect&) const, IntRect), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "String ToString() const", asMETHOD(IntRect, ToString), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "uint ToHash() const", asMETHOD(IntRect, ToHash), asCALL_THISCALL);
    engine->RegisterObjectMethod("IntRect", "float Length() const", asMETHOD(IntRect, Length), asCALL_THISCALL);
    engine->RegisterObjectProperty("IntRect", "int x", offsetof(IntRect, x));
    engine->RegisterObjectProperty("IntRect", "int y", offsetof(IntRect, y));
    engine->RegisterGlobalFunction("IntRect VectorMin(const IntRect&in, const IntRect&in)", asFUNCTIONPR(VectorMin, (const IntRect&, const IntRect&), IntRect), asCALL_CDECL);
    engine->RegisterGlobalFunction("IntRect VectorMax(const IntRect&in, const IntRect&in)", asFUNCTIONPR(VectorMax, (const IntRect&, const IntRect&), IntRect), asCALL_CDECL);

    engine->SetDefaultNamespace("IntRect");
    engine->RegisterGlobalProperty("const IntRect ZERO", (void*)&IntRect::ZERO);
    engine->RegisterGlobalProperty("const IntRect LEFT", (void*)&IntRect::LEFT);
    engine->RegisterGlobalProperty("const IntRect RIGHT", (void*)&IntRect::RIGHT);
    engine->RegisterGlobalProperty("const IntRect UP", (void*)&IntRect::UP);
    engine->RegisterGlobalProperty("const IntRect DOWN", (void*)&IntRect::DOWN);
    engine->RegisterGlobalProperty("const IntRect ONE", (void*)&IntRect::ONE);
    engine->SetDefaultNamespace(""); */
    
}

/* void ScriptManager::ConstructTransformable(Transformable* ptr)
{
    new(ptr) Transformable();
} */

void ScriptManager::RegisterTransformable()
{
    engine->RegisterObjectType("Transformable", 0, asOBJ_REF);
    engine->RegisterObjectBehaviour("Transformable", asBEHAVE_ADDREF, "void f()", asFUNCTION(FakeAddRef), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("Transformable", asBEHAVE_RELEASE, "void f()", asFUNCTION(FakeReleaseRef), asCALL_CDECL_OBJLAST);
   
    /* engine->RegisterObjectBehaviour("Transformable", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructTransformable), asCALL_CDECL_OBJLAST); */

    engine->RegisterObjectMethod("Transformable", "void SetPosition(float dx, float dy)", asMETHODPR(Transformable, setPosition, (float, float), void), asCALL_THISCALL); assert( r >= 0 );   
    engine->RegisterObjectMethod("Transformable", "void SetPosition(const Vector2f& in)", asMETHODPR(Transformable, setPosition, (const Vector2f&), void), asCALL_THISCALL); assert( r >= 0 );
    
    engine->RegisterObjectMethod("Transformable", "void SetScale(float dx, float dy)", asMETHODPR(Transformable, setScale, (float, float), void), asCALL_THISCALL); assert( r >= 0 );   
    engine->RegisterObjectMethod("Transformable", "void SetScale(const Vector2f& in)", asMETHODPR(Transformable, setScale, (const Vector2f&), void), asCALL_THISCALL); assert( r >= 0 );
    
    engine->RegisterObjectMethod("Transformable", "void Scale(float dx, float dy)", asMETHODPR(Transformable, scale, (float, float), void), asCALL_THISCALL); assert( r >= 0 );   
    engine->RegisterObjectMethod("Transformable", "void Scale(const Vector2f& in)", asMETHODPR(Transformable, scale, (const Vector2f&), void), asCALL_THISCALL); assert( r >= 0 );
    
    engine->RegisterObjectMethod("Transformable", "void SetRotation(float angle)", asMETHOD(Transformable, setRotation), asCALL_THISCALL); assert( r >= 0 );
    engine->RegisterObjectMethod("Transformable", "void Rotate(float angle)", asMETHOD(Transformable, rotate), asCALL_THISCALL); assert( r >= 0 );
    
    engine->RegisterObjectMethod("Transformable", "void Move(float dx, float dy)", asMETHODPR(Transformable, move, (float, float), void), asCALL_THISCALL); assert( r >= 0 );   
    engine->RegisterObjectMethod("Transformable", "void Move(const Vector2f& in)", asMETHODPR(Transformable, move, (const Vector2f&), void), asCALL_THISCALL); assert( r >= 0 );
    
    
    /*     engine->RegisterObjectMethod("Transformable", "Transformable& opAddAssign(const Transformable&in, const Transformable&in2)", asMETHOD(Transformable, operator +=), asCALL_THISCALL);
    engine->RegisterObjectMethod("Transformable", "Transformable& opSubAssign(const Transformable&in, const Transformable&in2)", asMETHOD(Transformable, operator -=), asCALL_THISCALL);
    engine->RegisterObjectMethod("Transformable", "Transformable& opMulAssign(const Transformable&in, float)", asMETHODPR(Transformable, operator *=, (float), Transformable&), asCALL_THISCALL);
    engine->RegisterObjectMethod("Transformable", "Transformable& opDivAssign(const Transformable&in, float)", asMETHODPR(Transformable, operator /=, (float), Transformable&), asCALL_THISCALL);
    engine->RegisterObjectMethod("Transformable", "Transformable& opMulAssign(const Transformable&in, const Transformable&in2)", asMETHODPR(Transformable, operator *=, (const Transformable&), Transformable&), asCALL_THISCALL);
    engine->RegisterObjectMethod("Transformable", "Transformable& opDivAssign(const Transformable&in, const Transformable&in2)", asMETHODPR(Transformable, operator /=, (const Transformable&), Transformable&), asCALL_THISCALL);
     */
    
}

void ScriptManager::ConstructMusic(Music* ptr)
{
    new(ptr) Music();
}

void ScriptManager::RegisterAudio()
{
    engine->RegisterObjectType("Music", sizeof(Music), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<Music>() | asOBJ_APP_CLASS_ALLINTS);
    engine->RegisterObjectBehaviour("Music", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructMusic), asCALL_CDECL_OBJLAST);
    
    engine->RegisterObjectMethod("Music", "void OpenFromFile(const string &in name)", asMETHOD(Music, openFromFile), asCALL_THISCALL); assert( r >= 0 );   
    
    engine->RegisterObjectMethod("Music", "void SetLoop(bool loop)", asMETHOD(Music, setLoop), asCALL_THISCALL); assert( r >= 0 ); 
    engine->RegisterObjectMethod("Music", "void Play()", asMETHOD(Music, play), asCALL_THISCALL); assert( r >= 0 );   
}
void ScriptManager::RegisterSFMLEvents()
{
    
}
void ScriptManager::RegisterKeyboardInput()
{
    engine->SetDefaultNamespace("Keyboard");
    engine->RegisterEnum("Key");
    engine->RegisterEnumValue("Key", "Unknown", Keyboard::Unknown);
    engine->RegisterEnumValue("Key", "A", Keyboard::A);
    engine->RegisterEnumValue("Key", "B", Keyboard::B);
    engine->RegisterEnumValue("Key", "C", Keyboard::C);
    engine->RegisterEnumValue("Key", "D", Keyboard::D);
    engine->RegisterEnumValue("Key", "E", Keyboard::E);
    engine->RegisterEnumValue("Key", "F", Keyboard::F);
    engine->RegisterEnumValue("Key", "G", Keyboard::G);
    engine->RegisterEnumValue("Key", "H", Keyboard::H);
    engine->RegisterEnumValue("Key", "I", Keyboard::I);
    engine->RegisterEnumValue("Key", "J", Keyboard::J);
    engine->RegisterEnumValue("Key", "K", Keyboard::K);
    engine->RegisterEnumValue("Key", "L", Keyboard::L);
    engine->RegisterEnumValue("Key", "M", Keyboard::M);
    engine->RegisterEnumValue("Key", "N", Keyboard::N);
    engine->RegisterEnumValue("Key", "O", Keyboard::O);
    engine->RegisterEnumValue("Key", "P", Keyboard::P);
    engine->RegisterEnumValue("Key", "Q", Keyboard::Q);
    engine->RegisterEnumValue("Key", "R", Keyboard::R);
    engine->RegisterEnumValue("Key", "S", Keyboard::S);
    engine->RegisterEnumValue("Key", "T", Keyboard::T);
    engine->RegisterEnumValue("Key", "U", Keyboard::U);
    engine->RegisterEnumValue("Key", "V", Keyboard::V);
    engine->RegisterEnumValue("Key", "W", Keyboard::W);
    engine->RegisterEnumValue("Key", "X", Keyboard::X);
    engine->RegisterEnumValue("Key", "Y", Keyboard::Y);
    engine->RegisterEnumValue("Key", "Z", Keyboard::Z);

    engine->RegisterEnumValue("Key", "Num0", Keyboard::Num0);
    engine->RegisterEnumValue("Key", "Num1", Keyboard::Num1);
    engine->RegisterEnumValue("Key", "Num2", Keyboard::Num2);
    engine->RegisterEnumValue("Key", "Num3", Keyboard::Num3);
    engine->RegisterEnumValue("Key", "Num4", Keyboard::Num4);
    engine->RegisterEnumValue("Key", "Num5", Keyboard::Num5);
    engine->RegisterEnumValue("Key", "Num6", Keyboard::Num6);
    engine->RegisterEnumValue("Key", "Num7", Keyboard::Num7);
    engine->RegisterEnumValue("Key", "Num8", Keyboard::Num8);
    engine->RegisterEnumValue("Key", "Num9", Keyboard::Num9);
    engine->RegisterEnumValue("Key", "Escape", Keyboard::Escape);
    engine->RegisterEnumValue("Key", "LControl", Keyboard::LControl);
    engine->RegisterEnumValue("Key", "LShift", Keyboard::LShift);
    engine->RegisterEnumValue("Key", "LAlt", Keyboard::LAlt);
    engine->RegisterEnumValue("Key", "LSystem", Keyboard::LSystem);
    engine->RegisterEnumValue("Key", "RControl", Keyboard::RControl);
    engine->RegisterEnumValue("Key", "RShift", Keyboard::RShift);
    engine->RegisterEnumValue("Key", "RAlt", Keyboard::RAlt);
    engine->RegisterEnumValue("Key", "RSystem", Keyboard::RSystem);
    engine->RegisterEnumValue("Key", "Menu", Keyboard::Menu);
    
    engine->RegisterEnumValue("Key", "LBracket", Keyboard::LBracket);
    engine->RegisterEnumValue("Key", "RBracket", Keyboard::RBracket);
    engine->RegisterEnumValue("Key", "Semicolon", Keyboard::Semicolon);
    engine->RegisterEnumValue("Key", "Comma", Keyboard::Comma);
    engine->RegisterEnumValue("Key", "Period", Keyboard::Period);
    engine->RegisterEnumValue("Key", "Quote", Keyboard::Quote);
    engine->RegisterEnumValue("Key", "Slash", Keyboard::Slash);
    engine->RegisterEnumValue("Key", "Backslash", Keyboard::Backslash);
    engine->RegisterEnumValue("Key", "Tab", Keyboard::Tab);
    engine->RegisterEnumValue("Key", "PageUp", Keyboard::PageUp);
    engine->RegisterEnumValue("Key", "PageDown", Keyboard::PageDown);
    engine->RegisterEnumValue("Key", "End", Keyboard::End);
    engine->RegisterEnumValue("Key", "Home", Keyboard::Home);

    
    engine->RegisterEnumValue("Key", "Insert", Keyboard::Insert);
    engine->RegisterEnumValue("Key", "Delete", Keyboard::Delete);
    engine->RegisterEnumValue("Key", "Add", Keyboard::Add);
    engine->RegisterEnumValue("Key", "Subtract", Keyboard::Subtract);
    engine->RegisterEnumValue("Key", "Multiply", Keyboard::Multiply);
    engine->RegisterEnumValue("Key", "Divide", Keyboard::Divide);
    engine->RegisterEnumValue("Key", "Left", Keyboard::Left);
    engine->RegisterEnumValue("Key", "Right", Keyboard::Right);
    engine->RegisterEnumValue("Key", "Up", Keyboard::Up);
    engine->RegisterEnumValue("Key", "Down", Keyboard::Down);
    engine->RegisterEnumValue("Key", "Numpad0", Keyboard::Numpad0);
    engine->RegisterEnumValue("Key", "Numpad1", Keyboard::Numpad1);
    engine->RegisterEnumValue("Key", "Numpad2", Keyboard::Numpad2);

    
    engine->RegisterEnumValue("Key", "Numpad3", Keyboard::Numpad3);
    engine->RegisterEnumValue("Key", "Numpad4", Keyboard::Numpad4);
    engine->RegisterEnumValue("Key", "Numpad5", Keyboard::Numpad5);
    engine->RegisterEnumValue("Key", "Numpad6", Keyboard::Numpad6);
    engine->RegisterEnumValue("Key", "Numpad7", Keyboard::Numpad7);
    engine->RegisterEnumValue("Key", "Numpad8", Keyboard::Numpad8);
    engine->RegisterEnumValue("Key", "Numpad9", Keyboard::Numpad9);
    engine->RegisterEnumValue("Key", "F1", Keyboard::Right);
    engine->RegisterEnumValue("Key", "F2", Keyboard::Up);
    engine->RegisterEnumValue("Key", "F3", Keyboard::Down);
    engine->RegisterEnumValue("Key", "F4", Keyboard::Numpad0);
    engine->RegisterEnumValue("Key", "F5", Keyboard::Numpad1);
    engine->RegisterEnumValue("Key", "F6", Keyboard::Numpad2);
    engine->RegisterEnumValue("Key", "F7", Keyboard::Down);
    engine->RegisterEnumValue("Key", "F8", Keyboard::Numpad0);
    engine->RegisterEnumValue("Key", "F9", Keyboard::Numpad1);
    engine->RegisterEnumValue("Key", "F10", Keyboard::Numpad2);
    engine->RegisterEnumValue("Key", "F11", Keyboard::Down);
    engine->RegisterEnumValue("Key", "F12", Keyboard::Numpad0);
    engine->RegisterEnumValue("Key", "F13", Keyboard::Numpad1);
    engine->RegisterEnumValue("Key", "F14", Keyboard::Numpad2);
    engine->RegisterEnumValue("Key", "F15", Keyboard::Down);
    engine->RegisterEnumValue("Key", "Pause", Keyboard::Numpad0);
    engine->RegisterEnumValue("Key", "KeyCount", Keyboard::Numpad1);

    
    engine->RegisterGlobalFunction("bool IsKeyPressed(Key key)", asFUNCTION(Keyboard::isKeyPressed), asCALL_CDECL);
    engine->RegisterGlobalFunction("void SetVirtualKeyboardVisible(bool value)", asFUNCTION(Keyboard::setVirtualKeyboardVisible), asCALL_CDECL);

    engine->SetDefaultNamespace("");
}

void ScriptManager::MessageCallback(const asSMessageInfo &msg)
{
    std::cout << "MSG CALLBACK" << std::endl;
	const char *type = "ERR ";
	if( msg.type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg.type == asMSGTYPE_INFORMATION ) 
		type = "INFO";
std::cout << "MSG CALLBACK" << std::endl;
	cout << msg.section << " (" << msg.row << ", " << msg.col << ") : " << type << " : " << msg.message << endl;

	if( msg.type == asMSGTYPE_ERROR )
		hasCompileErrors = true;
}

/* ScriptManager::SController *ScriptManager::GetControllerScript(const string &script) */
Script* ScriptManager::GetControllerScript(const string &script, GameObject* gameObj)
{
	int r;

    std::cout << "_____ GetCrontroller " << script << std::endl;
	// Find the cached controller
	for( unsigned int n = 0; n < controllers.size(); n++ )
	{
		if( controllers[n]->module == script )
			return controllers[n];
	}

    std::cout << "_____ GetCrontroller 2 " << script << std::endl;
	// No controller, check if the script has already been loaded
	asIScriptModule *mod = engine->GetModule(script.c_str(), asGM_ONLY_IF_EXISTS);
	if( mod )
	{
		// We've already attempted loading the script before, but there is no controller
		return 0;
	}

    std::cout << "_____ GetCrontroller 3" << script << std::endl;
	// Compile the script into the module
	CScriptBuilder builder;
	r = builder.StartNewModule(engine, script.c_str());
	if( r < 0 )
		return 0;

    std::cout << "_____ GetCrontroller 4" << script << std::endl;
	// If the script file doesn't exist, then there is no script controller for this type
	FILE *f;
	if( (f = fopen((script).c_str(), "r")) == 0 )
		return 0;
	fclose(f);

    std::cout << "=======Ada " << script << std::endl;
	// Let the builder load the script, and do the necessary pre-processing (include files, etc)
	r = builder.AddSectionFromFile((script).c_str());
	if( r < 0 )
		return 0;

        std::cout << "=======Ada " << script << std::endl;
	r = builder.BuildModule();
	if( r < 0 )
		return 0;

	// Cache the functions and methods that will be used
	/* SController *ctrl = new SController; */
    std::cout << "=======Ada " << script << std::endl;
    Script* ctrl = new Script();
    
    ctrl->name = script;
	controllers.push_back( ctrl );
    
	ctrl->module = script;

	// Find the class that implements the IController interface
	mod = engine->GetModule(script.c_str(), asGM_ONLY_IF_EXISTS);
	asITypeInfo *type = 0;
	int tc = mod->GetObjectTypeCount();
	for( int n = 0; n < tc; n++ )
	{
		bool found = false;
		type = mod->GetObjectTypeByIndex(n);
		int ic = type->GetInterfaceCount();
		for( int i = 0; i < ic; i++ )
		{
			if( strcmp(type->GetInterface(i)->GetName(), "IRemorseBehaviour") == 0 )
			{
				found = true;
				break;
			}
		}

		if( found == true )
		{
			ctrl->type = type;
			break;
		}
	}

	if( ctrl->type == 0 )
	{
		cout << "Couldn't find the controller class for the type '" << script << "'" << endl;
		controllers.pop_back();
		delete ctrl;
		return 0;
	}

	// Find the factory function
	// The game engine will pass in the owning GameObject to the controller for storage
	string s = string(type->GetName()) + "@ " + string(type->GetName()) + "(GameObject @)";
	ctrl->factoryFunc = type->GetFactoryByDecl(s.c_str());
	if( ctrl->factoryFunc == 0 )
	{
		cout << "Couldn't find the appropriate factory for the type '" << script << "'" << endl;
		controllers.pop_back();
		delete ctrl;
		return 0;
	}
	
	// Find the optional event handlers
	ctrl->startMethod     = type->GetMethodByDecl("void Start()");
	ctrl->updateMethod   = type->GetMethodByDecl("void Update()");

	// Add the cache as user data to the type for quick access
	type->SetUserData(ctrl);

	return ctrl;
}

void ScriptManager::CreateController(Script* ctrl, GameObject* gameObj)
{
	int r;
	asIScriptObject *obj = 0;

    std::cout << "Create Controller 1 \n";
	if( ctrl == 0 ) return;
		
	// Create the object using the factory function
	asIScriptContext *ctx = PrepareContextFromPool(ctrl->factoryFunc);

	// Pass the object pointer to the script function. With this call the 
	// context will automatically increase the reference count for the object.
	ctx->SetArgObject(0, gameObj);

    std::cout << "Create Controller 2 \n";
	// Make the call and take care of any errors that may happen
	r = ExecuteCall(ctx);
	if( r == asEXECUTION_FINISHED )
	{
		// Get the newly created object
		obj = *((asIScriptObject**)ctx->GetAddressOfReturnValue());

		// Since a reference will be kept to this object 
		// it is necessary to increase the ref count
		obj->AddRef();
	}
    std::cout << "Create Controller 3 \n";
	// Return the context to the pool so it can be reused
	ReturnContextToPool(ctx);
	
    std::cout << "Create Controller 4 \n";
    /* Ini Sementara pake index 0 karena test 1 script component dulu */
    ctrl->controller.push_back( obj );
    std::cout << "Create Controller 5 \n";
}

void ScriptManager::CallStart(asIScriptObject *object)
{
	// Find the cached onThink method id 
	Script *ctrl = reinterpret_cast<Script*>(object->GetObjectType()->GetUserData());

	// Call the method using the shared context
	if( ctrl->startMethod != 0 )
	{
		asIScriptContext *ctx = PrepareContextFromPool(ctrl->startMethod);
		ctx->SetObject(object);
		ExecuteCall(ctx);
		ReturnContextToPool(ctx);
	}
}

void ScriptManager::CallUpdate(asIScriptObject *object)
{
	// Find the cached onMessage method id
	Script* ctrl = reinterpret_cast<Script*>(object->GetObjectType()->GetUserData());

	// Call the method using the shared context
	if( ctrl->updateMethod != 0 )
	{
		asIScriptContext *ctx = PrepareContextFromPool(ctrl->updateMethod);
		ctx->SetObject(object);
		ExecuteCall(ctx);
		ReturnContextToPool(ctx);
	}
}

int ScriptManager::ExecuteCall(asIScriptContext *ctx)
{
	int r = ctx->Execute();
	if( r != asEXECUTION_FINISHED )
	{
		if( r == asEXECUTION_EXCEPTION )
		{
			cout << "Exception: " << ctx->GetExceptionString() << endl;
			cout << "Function: " << ctx->GetExceptionFunction()->GetDeclaration() << endl;
			cout << "Line: " << ctx->GetExceptionLineNumber() << endl;

			// It is possible to print more information about the location of the 
			// exception, for example the call stack, values of variables, etc if 
			// that is of interest.
		}
	}

	return r;
}

asIScriptContext *ScriptManager::PrepareContextFromPool(asIScriptFunction *func)
{
	asIScriptContext *ctx = 0;
	if( contexts.size() )
	{
		ctx = *contexts.rbegin();
		contexts.pop_back();
	}
	else
		ctx = engine->CreateContext();

	int r = ctx->Prepare(func); assert( r >= 0 );

	return ctx;
}

void ScriptManager::ReturnContextToPool(asIScriptContext *ctx)
{
	contexts.push_back(ctx);

	// Unprepare the context to free any objects that might be held
	// as we don't know when the context will be used again.
	ctx->Unprepare();
}
