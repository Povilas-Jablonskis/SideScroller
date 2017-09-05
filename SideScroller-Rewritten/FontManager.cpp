#include "FontManager.h"

namespace Engine
{
	FontManager::~FontManager()
	{
		for (std::vector<std::pair<std::string, std::shared_ptr<Font>>>::iterator it = faces.begin(); it != faces.end(); ++it)
		{
			auto characterList = it->second->getCharacterList();
			for (std::map<GLchar, Character>::iterator it2 = characterList->begin(); it2 != characterList->end(); ++it2)
			{
				glDeleteTextures(1, &it2->second.TextureID);
			}
		}

		faces.clear();

		if (FT_Done_FreeType(library))
		{
			#if _DEBUG
				std::cout << "ERROR::FREETYPE: Could not free FreeType Library" << std::endl;
			#endif
		}
	}

	FontManager::FontManager()
	{
		if (FT_Init_FreeType(&library))
		{
			#if _DEBUG
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			#endif
		}
	}

	void FontManager::loadFont(const std::string& _path, const std::string& _name)
	{
		for (auto face : faces)
		{
			if (face.first == _name)
				return;
		}

		FT_Face face;
		if (FT_New_Face(library, _path.c_str(), 0, &face))
		{
			#if _DEBUG
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			#endif
		}
		auto tempFont = std::make_shared<Font>(face);
		faces.push_back(std::pair<std::string, std::shared_ptr<Font>>(_name, std::move(tempFont)));
	}

	std::shared_ptr<Font> FontManager::getFont(const std::string& name)
	{
		for (auto face : faces)
		{
			if (face.first == name)
				return face.second;
		}

		return nullptr;
	}
}