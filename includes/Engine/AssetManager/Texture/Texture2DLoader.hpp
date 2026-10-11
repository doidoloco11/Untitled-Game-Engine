#pragma once

#include <Engine/AssetManager/AssetLoader.hpp>
#include <Engine/AssetManager/Asset.hpp>

#include <filesystem>
#include <memory>

class Texture2DLoader : public AssetLoader{
    std::shared_ptr<Asset> Load(const std::filesystem::path& filePath) override;
};