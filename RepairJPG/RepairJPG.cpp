#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>


void printHex(const std::vector<unsigned char>& buffer) {
    for (size_t i = 0; i < buffer.size(); ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(buffer[i]) << " ";

        if ((i + 1) % 16 == 0)
            std::cout << "\n";
    }
    std::cout << "The binary code of the file is:\n" << std::endl;
    std::cout << std::dec << std::endl; // revenir en base 10
}

bool readWord(std::ifstream& in, unsigned short& out) {
    unsigned char a, b;
    if (!in.read(reinterpret_cast<char*>(&a), 1)) return false;
    if (!in.read(reinterpret_cast<char*>(&b), 1)) return false;
    out = (a << 8) | b;
    return true;
}

int main() {
    const std::string inputFile = "IMG_20180601_211229.jpg";
    const std::string outputFile = "Fixed_IMG_20180601_211229.jpg";

    // Offset où insérer FF D9
    // Exemple : 500000 (à adapter)
    std::streamoff insertPos = 500008;  // Exemple

    unsigned short marker;
    unsigned short length;

    std::ifstream in(inputFile, std::ios::binary);
    if (!in) {
        std::cerr << "Erreur : impossible d’ouvrir " << inputFile << std::endl;
        return 1;
    }

    // Lire SOI = FF D8
    readWord(in, marker);
    if (marker != 0xFFD8) {
        std::cerr << "Pas un JPEG valide." << std::endl;
        return 1;
    }

    std::cout << "SOI trouvé." << std::endl;

    std::streamoff lastGoodPos = 2; // après FFD8

    while (true) {
        if (!readWord(in, marker)) break;

        if ((marker >> 8) != 0xFF) {
            // plus un marqueur valide
            break;
        }

        // Si c'est SOS (FF DA), lecture différente
        if (marker == 0xFFDA) {
            std::cout << "SOS trouvé, lecture jusqu’à corruption..." << std::endl;
            lastGoodPos = in.tellg();
            break;
        }

        // Lire la longueur du segment
        if (!readWord(in, length)) break;

        if (length < 2 || length > 65535) {
            // Longueur invalide = corruption → on s’arrête
            std::cout << "Longueur invalide détectée." << std::endl;
            break;
        }

        // Position suivante après ce segment
        in.seekg(length - 2, std::ios::cur);

        if (!in.good()) break;

        lastGoodPos = in.tellg();
    }

    std::cout << "Dernière position valide : " << lastGoodPos << std::endl;

    // Aller à la fin pour vérifier la taille du fichier
    in.seekg(0, std::ios::end);
    std::streamoff fileSize = in.tellg();

    if (insertPos > fileSize) {
        std::cerr << "Erreur : insertPos dépasse la taille du fichier." << std::endl;
        return 1;
    }

    // Revenir au début
    in.seekg(0, std::ios::beg);

    // Lire seulement les insertPos premiers octets
    std::vector<unsigned char> buffer(insertPos);

    in.read(reinterpret_cast<char*>(buffer.data()), insertPos);
    in.close();

    // Ajouter le marqueur FF D9
    buffer.push_back(0xFF);
    buffer.push_back(0xD9);

    // Écrire dans le nouveau fichier
    std::ofstream out(outputFile, std::ios::binary);
    if (!out) {
        std::cerr << "Erreur : impossible d’ouvrir " << outputFile << std::endl;
        return 1;
    }

    out.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    out.close();

    std::cout << "Fichier tronqué + FF D9 ajouté : " << outputFile << std::endl;

    return 0;
}