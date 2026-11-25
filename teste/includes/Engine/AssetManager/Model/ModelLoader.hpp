#pragma once

#include <Engine/AssetManager/AssetLoader.hpp>
#include <Engine/AssetManager/Asset.hpp>

#include <memory>

namespace uge{
    class ModelLoader : public AssetLoader{
    public:
        std::shared_ptr<Asset> Load(const std::filesystem::path& filePath) override;
    };
}