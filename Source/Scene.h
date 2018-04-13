#pragma once
#include "Game.h"

class Scene {
    public:
        static bool loadScene(std::string filePath, MemoryManager& mgr, GameManager& gmgr);
};