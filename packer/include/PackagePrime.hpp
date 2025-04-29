#pragma once
#include <filesystem>

bool packagePrime(const std::filesystem::path& packageDefPath, const std::filesystem::path& repPath,
                  const std::filesystem::path& outPath);