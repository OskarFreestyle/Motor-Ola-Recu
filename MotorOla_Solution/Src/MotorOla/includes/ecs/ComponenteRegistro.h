#pragma once
#include "ComponenteFactoria.h"
#if _DEBUG
#include "checkML.h"
#endif

/// <summary>
/// Registro de los nuevos componentes, ejemplo:
/// ComponenteRegistro::ComponenteRegistro<Transform>("transform");
/// </summary>
namespace ComponenteRegistro {
	template<typename T> class ComponenteRegistro {
	public:
		ComponenteRegistro(std::string compName) {
			ComponenteFactoria::GetInstance()->registerGenerator(compName, []()
				{
					return static_cast<Componente*>(new T());
				}
			);
		}
	};
}