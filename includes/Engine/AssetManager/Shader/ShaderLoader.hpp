#pragma once

#include <Engine/AssetManager/AssetLoader.hpp>
#include <Engine/AssetManager/Asset.hpp>

#include <filesystem>
#include <memory>

class ShaderLoader : public AssetLoader {
public:
    std::shared_ptr<Asset> Load(const std::filesystem::path& filePath) override;
};