#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <vector>
#include "remote_data.h"

class Utils {
public:
    static std::string cleanCommandName(const std::string& commandName);
    static std::string removeUnknownPrefix(const std::string& input);
    static std::string removeCsvSuffix(const std::string& input);
    static std::string toUpperCase(const std::string& input);
    static bool isUpperString(const std::string& str);
    static float getTextCenterOffset(const std::string text, int16_t width, float sizeText);
    static std::vector<std::string> addRemotesPrefixes(const std::vector<std::string>& names);
    static std::vector<std::string> convertCharVectorToStringVector(const std::vector<char>& charVector);

    // Fonction template pour extraire les noms
    template <typename T>
    static std::vector<std::string> extractFieldNames(const std::vector<T>& items, const std::string& fieldName);
};

// Déclaration des spécifications pour chaque entité
template <>
std::vector<std::string> Utils::extractFieldNames<Product>(const std::vector<Product>& items, const std::string& fieldName);

template <>
std::vector<std::string> Utils::extractFieldNames<Remote>(const std::vector<Remote>& items, const std::string& fieldName);

template <>
std::vector<std::string> Utils::extractFieldNames<RemoteCommand>(const std::vector<RemoteCommand>& items, const std::string& fieldName);


#endif // UTILS_H
