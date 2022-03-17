#include "LoadResources.h"

LoadResources::LoadResources()
{
}

LoadResources::~LoadResources()
{
}

void LoadResources::init()
{
	search(ASSETS);
}

string LoadResources::aud(string name)
{
	auto s= audio.find(name);
	return s->second;
}

string LoadResources::tex(string tex)
{
	auto t=textures.find(tex);
	return t->second;
}

void LoadResources::search(path p)
{
	//Recorre los directorios cargando los archivos en los diferentes map
	for (const auto& it : directory_iterator(p)) {
		size_t end = it.path().string().find_last_of(".");
		
		if (end <8) {
			search(it.path());
		}
		else {
			load(it.path(), end, p.string().length());
		}
	}
	if (textures.size() == 1) {
		std::cout << "Imagen cargada" << std::endl;
	}
}

void LoadResources::load(path p, size_t end, size_t pathLenght)
{
	//Cargan los assets cada uno en el map que le corresponde 
	//A�adir si hay otro tipo de assets
	string extension = p.string().substr(end);
	string name = p.string().substr(pathLenght + 1);
	if (extension == ".mp3" || extension == ".ogg" || extension == ".wav") {
		audio.insert(pair<string, string>(name, p.string()));
	}
	else if (extension == ".jpg" || extension == ".png" || extension == ".bmp") {
		textures.insert(pair<string, string>(name, p.string()));
	}
}
