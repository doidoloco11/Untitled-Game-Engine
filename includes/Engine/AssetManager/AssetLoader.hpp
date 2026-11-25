#pragma once

#include <Engine/AssetManager/Asset.hpp>

#include <memory>
#include <filesystem>

namespace uge{
    class AssetLoader{
    public:
        virtual ~AssetLoader() = default;
        virtual std::shared_ptr<Asset> Load(const std::filesystem::path& filePath) = 0;
    };
}