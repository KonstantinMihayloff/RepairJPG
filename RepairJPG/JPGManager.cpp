#include "JPGManager.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>

#include "Utils.hpp"

JPGManager::JPGManager()
    : isImageLoaded(false)
{
}

JPGManager::~JPGManager()
{
}

ErrorCode JPGManager::loadImage(const std::string& filePath)
{
	in = std::ifstream(filePath, std::ios::binary);
    if (!in) {
        std::cerr << "Erreur : impossible d’ouvrir " << filePath << std::endl;
        return ErrorCode::FAILED_TO_LOAD;
	}
    else {
		imagePath = filePath;
    }

    isImageLoaded = true;

	return ErrorCode::SUCCESS_TO_LOAD;

}

bool JPGManager::saveImage(const std::string& filePath)
{
	return false;
}

void JPGManager::repairImage()
{
}

void JPGManager::displayImageInfo() const
{
}

void JPGManager::insertEOIMarker()
{
}

void JPGManager::analyzeMarkers()
{
    MarkerAnalyseResult result = analyzeBeginningMarkers();

    if (result == MarkerAnalyseResult::MARKER_IS_CORRUPTED) {
        std::cerr << "Le marqueur de début est corrompu." << std::endl;
        return;
	}


	result = analyzeEndMarkers();

    if (result == MarkerAnalyseResult::MARKER_IS_CORRUPTED) {
        std::cerr << "Le marqueur de fin n'est pas trouvé." << std::endl;
        return;
    }
}

MarkerAnalyseResult JPGManager::analyzeBeginningMarkers()
{
    unsigned short length;

    readWord(in, marker);
    if (marker != 0xFFD8) {
        std::cerr << "Pas un JPEG valide." << std::endl;
        return MarkerAnalyseResult::MARKER_IS_CORRUPTED;
    }

	std::streampos position = in.tellg();

    std::cout << "Marqueur de début trouvé à la position " << position << std::endl;

    return MarkerAnalyseResult::MARKER_IS_OK;
} 

MarkerAnalyseResult JPGManager::analyzeEndMarkers()
{
    while (marker != 0xFFD9) {
        readWord(in, marker);
    }

    if (marker != 0xFFD9) {
        std::cerr << "Marqueur de fin non trouvé." << std::endl;
        return MarkerAnalyseResult::MARKER_IS_CORRUPTED;
    }

    std::streampos position = in.tellg();

    std::cout << "Marqueur de fin trouvé à la position " << position << std::endl;

    return MarkerAnalyseResult::MARKER_IS_OK;
    
}
