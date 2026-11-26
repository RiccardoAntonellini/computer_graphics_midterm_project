#pragma once
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

/**
 * @namespace FileUtils
 * @brief Utility per la lettura di file di testo e shader glsl.
 */
namespace FileUtils {

    /**
     * @brief Legge l'intero contenuto di un file di testo in una std::string.
     * @param path Percorso del file da aprire.
     * @return std::string Contenuto completo del file.
     * @throws std::runtime_error Se il file non esiste o non può essere aperto.
     */
    inline std::string readText(const std::string& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("File non trovato: " + path);
        std::ostringstream out;
        out << file.rdbuf();
        return out.str();
    }

} // namespace FileUtils
