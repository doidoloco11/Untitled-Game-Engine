#include <Engine/AssetManager/AssetManager.hpp>
#include <Engine/AssetManager/Asset.hpp>

#include <memory>
#include <stdexcept>

std::shared_ptr<Asset> AssetManager::LoadAsset(const std::filesystem::path &filePath){
    auto ait = assets.find(filePath);
    if (ait != assets.end()){
        if (std::shared_ptr<Asset> Asset = ait->second.lock()){
            return Asset;
        }
    }
    auto lit = loaders.find(filePath.extension());
    if (lit == loaders.end()) throw std::runtime_error("não tem loader para a extensão: " + filePath.extension().string());

    std::shared_ptr<Asset> asset = lit->second->Load(filePath);
    assets[filePath] = asset;
    return asset;
}