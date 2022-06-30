# Motor-Ola
Motor de juego de "Proyectos 3"

Subgrupo Óscar Pedregal y Óscar Fernández

# ÍNDICE 
1. Bibliotecas
2. Diseño del motor
3. Preparación del entorno

# 1. BIBLIOTECAS  
## SDL
Simple DirectMedia Layer es una biblioteca de desarrollo multiplataforma diseñada para brindar acceso de bajo nivel a hardware de audio, teclado, mouse, joystick y gráficos a través de OpenGL y Direct3D. MotorOla la utiliza para procesar el Input.

Página oficial: https://www.libsdl.org/

<p align="center">
  <img src="https://github.com/OskarFreestyle/Motor-Ola-Recu/blob/main/ImagenesMarkdown/LogoSDL.png">
</p>

## OGRE 3D Y OGREOVERLAY
Object-Oriented Graphics Rendering Engine, biblioteca escrita principalmente en C++ y orientada a *escenas*.  
Facilita el desarrollo de aplicación con gráficos 3D acelerados por hardware,  
evita la dificultad de utilización de librerías de capas inferiores para gráficos como OpenGL o Direct3D,  
además, ofrece una interfaz basada en *objetos del mundo* y otras clases de alto nivel.  

OgreOverlay es un UI facilitada por Ogre para renderizar objetos encima de la escena.

Página oficial: https://www.ogre3d.org/ 

<p align="center">
  <img src="https://github.com/OskarFreestyle/Motor-Ola-Recu/blob/main/ImagenesMarkdown/LogoOgre3D.png" width="200" height="150">
</p>

## PHYSX
Motor propietario de capa de software intermedia (middleware)  
y kit de desarrollo para llevar a cabo cálculos físicos complejos.  
Está desarrollado por NVIDIA y de hecho está integrado en sus chips gráficos más recientes. 

Página oficial: https://developer.nvidia.com/physx-sdk

<p align="center">
  <img src="https://github.com/OskarFreestyle/Motor-Ola-Recu/blob/main/ImagenesMarkdown/LogoPhysx.png">
</p>

## FMOD
Motor propietario de efectos de sonido especializado para el desarrollo de videojuegos,  
reproduce y mezcla sonidos en muchos sistemas operativos.  

Página oficial: https://www.fmod.com/

<p align="center">
  <img src="https://github.com/OskarFreestyle/Motor-Ola-Recu/blob/main/ImagenesMarkdown/LogoFMOD.png">
</p>

## LUA
Lenguaje multiparadigma, imperativo, estructurado y bastante ligero.  
Diseñado como un lenguaje interpretado con una semántica extendible, pensado para ser incorporado.  
Escrito en ANSI C y suficientemente compacto como para usarse en diferentes plataformas.

Página oficial: https://www.lua.org/

<p align="center">
  <img src="https://github.com/OskarFreestyle/Motor-Ola-Recu/blob/main/ImagenesMarkdown/LogoLua.png">
</p>

# 2. DISEÑO DEL MOTOR

## Inicio del motor
El motor al iniciarse creará los managers, después registrará los componentes en la factoría abstracta y por último cargará el juego que se encuentre en la DLL.

## Buble principal

<p align="center">
  <img src="https://github.com/OskarFreestyle/Motor-Ola-Recu/blob/main/ImagenesMarkdown/BuclePrincipal.png">
</p>

## Managers
Los managers utilizan el patrón de diseño *Singleton*, que permite restringir la creación de objetos pertenecientes a una clase o el valor de un tipo a un único objeto. Son los siguientes:

* **AudioManager:** Utiliza FMOD para gestionar todos los audios del juego. Dispone de 24 canales en los que cargar distintos audios.
* **ComponenteFactoria:** Utiliza el patrón *Abstract Factory* para registrar los componentes.
* **InputManager:** Se encarga de recoger el Input de ratón y teclado en cada frame.
* **LoadResources:** Almacena una serie de *maps* que relaciona las rutas de cada recurso con su nombre para poder acceder a ellas de una manera más cómoda y eficiente.
* **LuaReader:** Sirve para leer las escenas.lua y los prefabs.prefab porque nuestro motor ahora sí es dirigido a datos.
* **OgreManager:** Pone en marcha la ventana del juego y se encarga del renderizado cada frame.
* **OverlayManager:** Se encarga de los elementos del UI que se renderizan por encima de los demás elementos de la escena.
* **PhysxManager:** Realiza la simulación de las físicas.
* **SceneManager:** Almacena las entidades de la escena y se encarga de cargar/borrar dichas entidades.

## ECS (Entity-Component-System)
Es un patrón arquitectónico de software utilizado principalmente en el desarrollo de videojuegos para la representación de objetos del mundo del juego. Un ECS comprende entidades compuestas de componentes de datos, con sistemas que operan en componentes de entidades.

Los componentes del motor son los siguientes: AudioSource, Button, Camera, Collider, Image, Light, Mesh, Rigidbody, Text y Transform.

Para la creación de nuevos componentes se utiliza *Abstract Factory.* Es un patrón de diseño creacional que nos permite producir familias de objetos relacionados sin especificar sus clases concretas.

