#include "AGE/texturemanager.h"
#include <stdlib.h>
#include <dirent.h>

using namespace age;

/* -------------------------- */
/* TextureManager::TextureManager */
/* -------------------------- */
TextureManager::TextureManager()
{

}

/* --------------------------- */
/* TextureManager::~TextureManager */
/* --------------------------- */
TextureManager::~TextureManager()
{
  //On détruit toutes les Textures restantes
  std::map<std::string, sf::Texture*>::iterator it;
  for(it = Textures.begin(); it != Textures.end(); it++)
  {
    delete it->second;
  }
}

/* ------------------------- */
/* TextureManager::SearchTexture */
/* ------------------------- */
bool TextureManager::SearchTexture(const std::string &TextureLink, sf::Texture *&ImgTemp)
{
  std::map<std::string, sf::Texture*>::iterator it;

  it = Textures.find(TextureLink);

  if(it == Textures.end())
  {
    return false;
  }
  else
  {
    ImgTemp = it->second;
  }

  return true;
}

/* ------------------------- */
/* TextureManager::CreateTexture */
/* ------------------------- */
sf::Texture *TextureManager::CreateTexture(const std::string &TextureLink)
{
  sf::Texture *ImgTemp = new sf::Texture();
  if(ImgTemp->loadFromFile(TextureLink))
  {
    Textures.insert(std::make_pair(TextureLink, ImgTemp));
    return ImgTemp;
  }
  else
  {
    delete ImgTemp;
  }

  return NULL;
}

/* ---------------------- */
/* TextureManager::GetTexture */
/* ---------------------- */
sf::Texture *TextureManager::GetTexture(const std::string &TextureLink)
{
  sf::Texture *ImgTemp;

  //Si l'Texture demandée n'existe pas déjà, on la créée
  if(!SearchTexture(TextureLink, ImgTemp))
  {
    ImgTemp = CreateTexture(TextureLink);
  }

  return ImgTemp;
}

/* ------------------------- */
/* TextureManager::DeleteTexture */
/* ------------------------- */
void TextureManager::DeleteTexture(sf::Texture &Img)
{
  std::map<std::string, sf::Texture*>::iterator it;

  //On cherche l'Texture demandée
  for(it = Textures.begin(); it != Textures.end(); it++)
  {
    //Si on la trouve, on la détruit et on la retire
    if(it->second == &Img)
    {
      delete &Img;
      Textures.erase(it);
      break;
    }
  }
}

/* ------------------------- */
/* TextureManager::DeleteTexture */
/* ------------------------- */
void TextureManager::DeleteTexture(const std::string& TextureLink)
{
  std::map<std::string, sf::Texture*>::iterator it;

  it = Textures.find(TextureLink);

  if(it != Textures.end())
  {
    if(it->second != NULL)
    {
      delete (it->second);
      Textures.erase(it);
    }
  }
}
