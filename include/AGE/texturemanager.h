#ifndef TextureMANAGER_H
#define TextureMANAGER_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>

namespace age{
class TextureManager
{
private:
    std::map<std::string, sf::Texture*> Textures;

    /**
   * Search an Texture in memory
   * @param TextureLink File name of the Texture
   * @param ImgTemp Output pointer to the sf::Texture if found
   * @return Boolean, true if the sf::Texture is found, false otherwise
   */
    bool SearchTexture(const std::string &TextureLink, sf::Texture *&ImgTemp);

    /**
   * Create a sf::Texture from his file name
   * @param TextureLink File name of the Texture
   * @return Pointer to the sf::Texture created, or NULL if failed
   */
    sf::Texture *CreateTexture(const std::string &TextureLink);

public:
    /**
   * TextureManager constructor
   */
    TextureManager();

    /**
   * TextureManager destructor
   */
    ~TextureManager();

    /**
   * Get a sf::Texture from a file name
   * @param TextureLink File name of the Texture
   * @return Pointer to the sf::Texture
   */
    sf::Texture *GetTexture(const std::string &TextureLink);

    /**
   * Delete a sf::Texture if already loaded
   * @param Img The sf::Texture to destroy
   * @return None
   */
    void DeleteTexture(sf::Texture &Img);

    /**
   * Delete a sf::Texture if already loaded
   * @param TextureLink File name of the Texture to destroy
   * @return None
   */
    void DeleteTexture(const std::string& TextureLink);
};
}//age

#endif // TextureMANAGER_H
