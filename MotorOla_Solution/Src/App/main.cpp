#include <iostream>
#include <MotorOla.h>

#if _DEBUG
    #include "checkML.h"
int main(){
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#include <windows.h>
int WINAPI
WinMain(HINSTANCE zhInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow) {
#endif
    std::cout << "---------- APLICACION INICIADA ----------\n";

    bool hayError = false;
    
    MotorOla* m = nullptr;
    
    // Inicia el Motor
    try {
        std::cout << "---------- NEW MOTOR ----------\n";
        m = new MotorOla();
    }
    catch (const char* error) {
        std::cout << "Error creando el motor\n";
        std::cout << "Error: " << error << "\n";
        hayError = true;
    }
    
    // Intenta iniciar los sistemas principales
    try {
        std::cout << "---------- MOTOR INIT SISTEMS ----------\n";
        m->initMotorOla();
    }
    catch (const char* error) {
        std::cout << "Error iniciando el motor\n";
        std::cout << "Error: " << error << "\n";
        hayError = true;
        system("pause");
    }

    // Pone en marcha el bucle principal del Motor
    try {
        std::cout << "MOTOR START MAIN LOOP\n";
        m->initLoop();
    }
    catch (const char* error) {
        std::cout << "Error durante el bucle principal del motor\n";
        std::cout << "Error: " << error << "\n";
        hayError = true;
        system("pause");
    }
    
    // Libera la memoria del Motor y los sistemas principales
    if (m != nullptr) {
        delete m;
        m = nullptr;
    }

    return hayError;
}