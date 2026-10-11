#pragma once

#include <Engine/AssetManager/Asset.hpp>
#include <Engine/AssetManager/AssetLoader.hpp>

#include <filesystem>
#include <type_traits>
#include <unordered_map>
#include <memory>

class AssetManager{
private:
    std::unordered_map<std::filesystem::path, std::unique_ptr<AssetLoader>> loaders;
    std::unordered_map<std::filesystem::path, std::weak_ptr<Asset>> assets;
public:
    template <class L>
    void AddLoader(const std::filesystem::path& extension){
        static_assert(std::is_base_of<AssetLoader, L>(), "L não é derivada de AssetLoader");
        loaders[extension] = std::make_unique<L>();
    }
    std::shared_ptr<Asset> LoadAsset(const std::filesystem::path& filePath);
};