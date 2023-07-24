#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <fstream>
#include <sstream>

const std::string cfg_path = "Resource/Textures.cfg";

class TextureManager {
private:
	std::unordered_map<std::string, std::pair<sf::Texture*, unsigned int> > m_resources;
	std::unordered_map<std::string, std::string> m_paths;

public:
	TextureManager(const std::string& l_pathsFile) {
		LoadPaths(l_pathsFile);
	}

	~TextureManager() {
		PurgeResource();
	}

	bool RequireTexture(const std::string& l_id) {
		auto res = Find(l_id);
		if (res) {
			res->first++;
			return true;
		}

		auto path = m_paths.find(l_id);
		if (path == m_paths.end()) {
			return false;
		}

		sf::Texture* texture = Load(path->second);
		if (!texture) {
			return false;
		}

		m_resources.emplace(l_id, std::make_pair(texture, 1));
	}

	bool ReleaseTexture(const std::string& l_id) {
		auto res = Find(l_id);
		if (!res) {
			return false;
		}

		res->second--;
		if (!res->second)
			Unload(l_id);
	}

	void PurgeResource() {

	}

	std::pair<sf::Texture*, unsigned int>* Find(const std::string& l_id) {
		auto itr = m_resources.find(l_id);
		return (itr != m_resources.end() ? &itr->second : nullptr);
	}

	sf::Texture* Load(const std::string& l_path) {
		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile(l_path)) {
			delete texture;
			texture = nullptr;
			std::cerr << "Fail to load Texture: " << l_path << '\n';
		}

		return texture;
	}

	bool Unload(const std::string& l_id) {
		auto itr = m_resources.find(l_id);
		if (itr == m_resources.end())
			return false;

		delete itr->second.first;
		m_resources.erase(itr);
		return true;
	}

	void LoadPaths(const std::string& l_pathFile) {
		std::ifstream paths;
		paths.open(l_pathFile);

		if (paths.is_open()) {
			std::string line;
			while (std::getline(paths, line)) {
				std::stringstream keystream(line);
				std::string pathName;
				std::string path;
				keystream >> pathName >> path;
				m_paths.emplace(pathName, path);
			}

			paths.close();
			return;
		}

		std::cerr << "Fail to load path file: " << l_pathFile << '\n';
	}

	std::string GetPath(const std::string& l_id) {
		auto path = m_paths.find(l_id);
		return (path != m_paths.end() ? path->second : "");
	}

	sf::Texture* GetTexture(const std::string& l_id) {
		auto res = Find(l_id);
		return (res ? res->first : nullptr);
	}
};