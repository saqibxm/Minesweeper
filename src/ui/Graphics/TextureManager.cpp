#include "Graphics/TextureManager.hpp"
#include "../../../inc/ui/PlaceholderTexture.h"

using namespace mines;

std::map<std::string, sf::Texture> TextureManager::textures;
bool TextureManager::initialized = false;

TextureManager::TextureManager()
{
    Initialize(fs::current_path() / "assets"); // hardcoded assets directory

    if(!textures.empty()) initialized = true;
}

void TextureManager::Initialize(fs::path asset_dir)
{
    fs::recursive_directory_iterator dit(asset_dir, fs::directory_options::skip_permission_denied);
    for(; dit != decltype(dit){}; ++dit)
    {
        if(!dit->is_regular_file() || !IsImageFile(dit->path())) continue;
        // only load png images for now, subject to change

        Open(dit->path().stem().string(), dit->path()); // store each file by its name without extension
    }

    sf::Image img({QMARK_WIDTH, QMARK_HEIGHT}, qmark);
    sf::Texture placeholder(img);
    textures.emplace("__placeholder__", std::move(placeholder));
}

bool TextureManager::Open(std::string_view name, fs::path filename)
{
    sf::Texture texture;
    if(!texture.loadFromFile(filename)) return false;

    if(textures.find(filename.string()) != textures.end()) return true;

    textures.emplace(name, texture);

    return true;
}

const sf::Texture& TextureManager::Fetch(const std::string &name) const
{
    decltype(textures)::const_iterator it = textures.find(name);
    if(it == textures.end())
        return Placeholder(); // update to return the placeholder texture
        // throw std::runtime_error("Couldn't get texture " + name);
    return it->second;
}

sf::Texture& TextureManager::Fetch(const std::string &name)
{
    return const_cast<sf::Texture&>(static_cast<const TextureManager*>(this)->Fetch(name));
}

const sf::Texture* TextureManager::FetchPtr(const std::string &name) const
{
    if(auto it = textures.find(name); it != textures.end())
        return &(it->second);
    return nullptr;
}

sf::Texture* TextureManager::FetchPtr(const std::string &name)
{
    return const_cast<sf::Texture*>(static_cast<const TextureManager*>(this)->FetchPtr(name));
}

const sf::Texture& TextureManager::Placeholder() const
{
    return Fetch("__placeholder__");
}

sf::Texture& TextureManager::Placeholder()
{
    return const_cast<sf::Texture&>(static_cast<const TextureManager*>(this)->Placeholder());
}

sf::Texture* TextureManager::PlaceholderPtr()
{
    return &Placeholder();
}

bool TextureManager::IsImageFile(const fs::path &filename)
{
    static constexpr const char* imgs[] = { ".png", ".jpg", ".jpeg", ".bmp", ".svg" };
    auto extension = filename.extension();

    for(auto img : imgs)
    {
        if(extension == img)
            return true;
    }
    return false;
}