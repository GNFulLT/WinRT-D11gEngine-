#ifndef FOLDERS_H
#define FOLDERS_H

#include "string/string_utils.h"

constexpr inline static std::string_view RESOURCES_FOLDER_FULL_PATH = "resources/";
constexpr inline static std::string_view DATA_FOLDER_FULL_PATH = "data/";
constexpr inline static std::string_view LOCALES_FOLDER = "locales/";
constexpr inline static std::string_view LOCALES_FOLDER_FULL_PATH = join_v<RESOURCES_FOLDER_FULL_PATH, LOCALES_FOLDER>;
constexpr inline static std::string_view CONFIG_FOLDER = "config/";
constexpr inline static std::string_view CONFIG_FOLDER_FULL_PATH = join_v<DATA_FOLDER_FULL_PATH, CONFIG_FOLDER>;

#endif // FOLDERS_H