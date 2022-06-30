# Motor-Ola
Motor de juego de "Proyectos 3"

Subgrupo Óscar Pedregal y Óscar Fernández

# ÍNDICE 
1. Bibliotecas
2. Diseño del motor
3. Preparación del entorno

# 1. BIBLIOTECAS  
## OGRE 3D
Object-Oriented Graphics Rendering Engine, biblioteca escrita principalmente en C++ y orientada a *escenas*.  
Facilita el desarrollo de aplicación con gráficos 3D acelerados por hardware,  
evita la dificultad de utilización de librerías de capas inferiores para gráficos como OpenGL o Direct3D,  
además, ofrece una interfaz basada en *objetos del mundo* y otras clases de alto nivel.  

## PHYSX
Motor propietario de capa de software intermedia (middleware)  
y kit de desarrollo para llevar a cabo cálculos físicos complejos.  
Está desarrollado por NVIDIA y de hecho está integrado en sus chips gráficos más recientes.  

## FMOD
Motor propietario de efectos de sonido especializado para el desarrollo de videojuegos,  
reproduce y mezcla sonidos en muchos sistemas operativos.  

## OGREOVERLAY
UI facilitada por Ogre para renderizar objetos encima de la escena.

## LUA
Lenguaje multiparadigma, imperativo, estructurado y bastante ligero.  
Diseñado como un lenguaje interpretado con una semántica extendible, pensado para ser incorporado.  
Escrito en ANSI C y suficientemente compacto como para usarse en diferentes plataformas.

# 2. DISEÑO DEL MOTOR

## Buble principal

Foto aqui

## Managers
Los managers utilizan el patrón de diseño *Singleton*, que permite restringir la creación de objetos pertenecientes a una clase o el valor de un tipo a un único objeto. Hay un manager para el Audio, Componentes, Input, Recursos, Lua, Ogre, Overlay, Physx y Scenes.

## ECS (Entity-Component-System)
Es un patrón arquitectónico de software utilizado principalmente en el desarrollo de videojuegos para la representación de objetos del mundo del juego. Un ECS comprende entidades compuestas de componentes de datos, con sistemas que operan en componentes de entidades.

Los componentes del motor son los siguientes: AudioSource, Button, Camera, Collider, Image, Light, Mesh, Rigidbody, Text y Transform.

Para la creación de nuevos componentes se utiliza *Abstract Factory.* Es un patrón de diseño creacional que nos permite producir familias de objetos relacionados sin especificar sus clases concretas.

