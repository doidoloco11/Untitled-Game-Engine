#pragma once

#include <Engine/AssetManager/Asset.hpp>

#include <memory>
#include <filesystem>

class AssetLoader{
public:
    virtual std::shared_ptr<Asset> Load(const std::filesystem::path& filePath) = 0;
};