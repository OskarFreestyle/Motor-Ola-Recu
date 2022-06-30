:: Generar MotorOla.lib, MotorOla.dll, MotorOla_d.lib y MotorOla_d.dll para poder compilar la dll del juego
echo BUILDING MOTOROLA.LIB
msbuild MotorOla_Solution.sln -p:Configuration=Debug
msbuild MotorOla_Solution.sln -p:Configuration=Release
echo BUILDING DONE