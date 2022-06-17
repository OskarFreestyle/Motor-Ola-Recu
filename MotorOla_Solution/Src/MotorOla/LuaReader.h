#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif
#include <string>
class Entidad;

extern "C"
{
    struct lua_State;
}

class MOTOR_API LuaReader {
public:
    ~LuaReader() {};
    inline static LuaReader* GetInstance() { return _singleton; }
    static bool Init();
    void readFile(std::string file);
    void readFileMenus(std::string file, const char* get);

    void PrintTable(lua_State* L);

    void readFileTest(std::string file);
    Entidad* readPrefab(std::string file);
protected:
    static LuaReader* _singleton;
    LuaReader() {};


};
