#ifndef TEXMAN_HPP
#define TEXMAN_HPP

#include <map>
#include <string>
#include <string_view>
#include <filesystem>

#include <SFML/Graphics.hpp>

namespace fs = std::filesystem;

namespace mines
{
    class TextureManager
    {
    public:
        TextureManager();

        void Initialize(fs::path asset_dir);
        bool Open(std::string_view name, fs::path filename);

        const sf::Texture& Fetch(const std::string &name) const;
        sf::Texture& Fetch(const std::string &name);

        const sf::Texture* FetchPtr(const std::string &name) const;
        sf::Texture* FetchPtr(const std::string &name);

        const sf::Texture& Placeholder() const;
        sf::Texture& Placeholder();
        sf::Texture* PlaceholderPtr();

    private:
        static bool initialized;
        static std::map<std::string, sf::Texture> textures;

        static bool IsImageFile(const fs::path &filename);
    };
}

#endif /* TEXMAN_HPP */