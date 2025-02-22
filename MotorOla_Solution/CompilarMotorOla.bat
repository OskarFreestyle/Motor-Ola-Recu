:: Este .bat hay que ejecutarlo desde el Developer Command Prompt de Visual Studio 
:: se puede encontrar en la pestaña tools de visual en Command Line

:: BUILD OGRE
:: Entra en la carpeta de dependencias
cd Dependencies/Ogre

:: Crea el directorio Ogre/Build y entramos
mkdir Build
cd Build

:: CMake hace la Build de Ogre
..\..\CMake\bin\cmake.exe -DOGRE_BUILD_COMPONENT_OVERLAY=TRUE -DOGRE_BUILD_RENDERSYSTEM_D3D9=FALSE -DOGRE_BUILD_RENDERSYSTEM_D3D11=FALSE -DOGRE_BUILD_RENDERSYSTEM_GL3PLUS=FALSE -DOGRE_BUILD_RENDERSYSTEM_GL=TRUE -DOGRE_BUILD_RENDERSYSTEM_GLES2=FALSE -DOGRE_BUILD_PLUGIN_ASSIMP=FALSE -DOGRE_BUILD_PLUGIN_BSP=FALSE -DOGRE_BUILD_PLUGIN_OCTREE=FALSE -DOGRE_BUILD_PLUGIN_DOT_SCENE=FALSE -DOGRE_BUILD_PLUGIN_PCZ=FALSE -DOGRE_BUILD_COMPONENT_TERRAIN=FALSE -DOGRE_BUILD_COMPONENT_VOLUME=FALSE -DOGRE_BUILD_COMPONENT_BITES=FALSE -DOGRE_BUILD_COMPONENT_PYTHON=FALSE -DOGRE_BUILD_COMPONENT_JAVA=FALSE -DOGRE_BUILD_COMPONENT_CSHARP=FALSE -DOGRE_INSTALL_CMAKE=FALSE -DOGRE_INSTALL_SAMPLES=FALSE -DOGRE_INSTALL_DOCS=FALSE -DOGRE_INSTALL_PDB=FALSE -DOGRE_INSTALL_VSPROPS=TRUE -DOGRE_BUILD_TOOLS=FALSE ../Src
..\..\CMake\bin\cmake.exe --build . --config release 
..\..\CMake\bin\cmake.exe --build . --config debug

:: Se copian las dlls de Ogre a Exes
cd bin/debug
copy /Y Codec_STBI_d.dll "../../../../../Exes/Codec_STBI_d.dll"
copy /Y OgreMain_d.dll "../../../../../Exes/OgreMain_d.dll"
copy /Y OgreMeshLodGenerator_d.dll "../../../../../Exes/OgreMeshLodGenerator_d.dll"
copy /Y OgreOverlay_d.dll "../../../../../Exes/OgreOverlay_d.dll"
copy /Y OgrePaging_d.dll "../../../../../Exes/OgrePaging_d.dll"
copy /Y OgreProperty_d.dll "../../../../../Exes/OgreProperty_d.dll"
copy /Y OgreRTShaderSystem_d.dll "../../../../../Exes/OgreRTShaderSystem_d.dll"
copy /Y Plugin_ParticleFX_d.dll "../../../../../Exes/Plugin_ParticleFX_d.dll"
copy /Y RenderSystem_GL_d.dll "../../../../../Exes/RenderSystem_GL_d.dll"
REM copy /Y SDL2.dll "../../../../../Exes/SDL2.dll"
REM copy /Y zlib.dll "../../../../../Exes/zlib.dll"

:: Ojo que hay alguna dll más en release
cd ../release
copy /Y Codec_STBI.dll "../../../../../Exes/Codec_STBI.dll"
copy /Y OgreMain.dll "../../../../../Exes/OgreMain.dll"
copy /Y OgreMeshLodGenerator.dll "../../../../../Exes/OgreMeshLodGenerator.dll"
copy /Y OgreOverlay.dll "../../../../../Exes/OgreOverlay.dll"
copy /Y OgrePaging.dll "../../../../../Exes/OgrePaging.dll"
copy /Y OgreProperty.dll "../../../../../Exes/OgreProperty.dll"
copy /Y OgreRTShaderSystem.dll "../../../../../Exes/OgreRTShaderSystem.dll"
copy /Y Plugin_ParticleFX.dll "../../../../../Exes/Plugin_ParticleFX.dll"
copy /Y RenderSystem_GL.dll "../../../../../Exes/RenderSystem_GL.dll"

:: Mensaje de verificación: OGRE
echo: && Echo ::::: BUILD [ OGRE ] COMPLETADA :::::

:: Estas están repetidas pero por si acaso
copy /Y SDL2.dll "../../../../../Exes/SDL2.dll"
copy /Y zlib.dll "../../../../../Exes/zlib.dll"

:: Copia SDL2d.dll en Bin y en Exes
cd ..\..\SDL2-build 
..\..\..\CMake\bin\cmake.exe --build . --config release 
..\..\..\CMake\bin\cmake.exe --build . --config debug

cd Debug
copy /Y SDL2d.dll "../../../../../Exes/SDL2d.dll"

:: Mensaje de verificación: SDL
echo: && Echo ::::: BUILD [ SDL ] COMPLETADA :::::

:: BUILD FMOD
:: Copia fmod.dll y fmodL.dll en Exes
cd ..\..\..\..\FMOD\libreria\x64
copy /Y fmod.dll "../../../../Exes/fmod.dll"
copy /Y fmodL.dll "../../../../Exes/fmodL.dll"

:: Vuelta a la carpeta principal del proyecto (En la que se encuentra MotorOla.bat)
cd ..\..\..\..\

:: Mensaje de verificación: FMOD
echo: && Echo ::::: BUILD [ FMOD ] COMPLETADA :::::

:: BUILD PHYSX
:: Build de PhysX-4.1
cd Dependencies\PhysX-4.1\Src\physx\
call buildPhysX.bat

:: Vuelta a la carpeta principal del proyecto (En la que se encuentra MotorOla.bat)
cd ..\..\..\..\

:: Mensaje de verificación: PhysX
echo: && Echo ::::: BUILD [ PhysX ] COMPLETADA :::::

:: BUILD LUA
:: Se va al directorio de Lua que esta en Dependencies
cd Dependencies\Lua
:: Uso msbuild para que se haga la build de Lua y especifico que se haga en debug y release
msbuild -p:Configuration=Debug
msbuild -p:Configuration=Release

:: Me meto en la carpeta de Debug que se ha generado al hacer la build y copio Lua_d.dll en Bin y Exes
cd x64/Debug
copy /Y Lua_d.dll "../../../../Exes/Lua_d.dll"
:: Me muevo a la carpeta Release y copio Lua.dll en Bin y Exes
cd ../Release
copy /Y Lua.dll "../../../../Exes/Lua.dll"

:: Vuelta a la carpeta principal del proyecto (En la que se encuentra MotorOla.bat)
cd ..\..\..\..\

:: Mensaje de verificacion
echo: && Echo ::::: BUILD [ LUA ] COMPLETADA :::::

:: FINALIZADO!!!

:: Mensaje de verificación
echo: && Echo ::::: BUILD [ MotorOla ] COMPLETADA :::::

:: Pausa para ver que ha ocurrido
:: echo: && Echo ::::: PULSA CUALQUIER BOTON PARA CONTINUAR :::::
:: PAUSE >nul
