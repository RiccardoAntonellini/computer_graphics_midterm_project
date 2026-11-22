#pragma once
#include "MeshData.h"
#include "ObjLoader.h"
#include <stdexcept>
#include <string>

/**
 * @namespace AssetsUtils
 * @brief Utility per il caricamento delle risorse geometriche da disco.
 */
namespace AssetsUtils {

    /**
     * @brief Carica una mesh 2D/3D in formato OBJ dalla cartella "assets/".
     * @param file Nome del file (o percorso relativo interno a "assets/").
     * @return MeshData Struttura contenente i vertici e gli indici caricati dal file OBJ.
     * @throws std::runtime_error Se il file non esiste, non è leggibile o il parsing OBJ fallisce.
     * @note Il percorso è risolto relativamente alla Working Directory dell'eseguibile.
     */
    inline MeshData loadMesh(const std::string& file)
    {
        MeshData mesh;
        std::string error;
        if (!ObjLoader::load("assets/" + file, mesh, error))
            throw std::runtime_error(error);
        return mesh;
    }

} // namespace AssetsUtils
