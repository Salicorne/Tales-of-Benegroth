#include "Scene.h"

bool Scene::loadScene(std::string filePath, MemoryManager& mgr, GameManager& gmgr) {
    std::ifstream file(filePath);
    if(!file) { std::cerr << "Error : could not open scene file " << filePath << std::endl; return false; }
    mgr.dropSprites();

    int op = -1, i = 0;
    std::string line;
    while(std::getline(file, line)) {
        i++;
        std::stringstream stream(line);
        if(stream >> op) {
            switch(op) {
                case Game::FileOp::Texture:
                    {
                        std::string name;
                        int id;
                        if(!(stream >> name)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> id)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        mgr.addTexture(name, id);
                    }
                    break;

                case Game::FileOp::Sprite:
                    {
                        int texture, posx, posy, id;
                        float offset;
                        if(!(stream >> texture)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posx)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posy)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> id)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> offset)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        mgr.addSprite(static_cast<game_id>(texture), vec2f(posx, posy), static_cast<game_id>(id != 0 ? id : mgr.getFreeSpriteId()), offset);
                    }
                    break;

                case Game::FileOp::RepeatedSprite:
                    {
                        int texture, w, h, posx, posy, id;
                        float offset;
                        if(!(stream >> texture)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> w)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> h)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posx)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posy)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> id)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> offset)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        mgr.addRepeatedSprite(static_cast<game_id>(texture), w, h, vec2f(posx, posy), static_cast<game_id>(id != 0 ? id : mgr.getFreeSpriteId()), offset);
                    }
                    break;

                case Game::FileOp::AnimatedSprite:
                    {
                        int texture, left, top, w, h, millis, posx, posy, id;
                        float offset;
                        if(!(stream >> texture)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> left)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> top)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> w)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> h)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> millis)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posx)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posy)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> id)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> offset)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        mgr.addAnimatedSprite(static_cast<game_id>(texture), sf::IntRect(left, top, w, h), sf::milliseconds(millis), vec2f(posx, posy), static_cast<game_id>(id != 0 ? id : mgr.getFreeSpriteId()), offset);
                    }
                    break;

                case Game::FileOp::TreeSprite:
                    {
                        int trunk, leaves, min, max, posx, posy, id;
                        float offset;
                        if(!(stream >> trunk)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> leaves)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> min)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> max)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posx)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posy)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> id)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> offset)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        AssetCreator::createTreeSprite(mgr, static_cast<game_id>(trunk), static_cast<game_id>(leaves), min, max, vec2f(posx, posy), static_cast<game_id>(id != 0 ? id : mgr.getFreeSpriteId()), offset);
                    }
                    break;

                case Game::FileOp::SpriteSet:
                    {
                        int texture, left, top, w, h, posx, posy, millis, id, tmp_id, tmp_r, tmp_g, tmp_b;
                        float offset;
                        std::vector<std::pair<game_id, sf::Color>> vec;
                        if(!(stream >> texture)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> left)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> top)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> w)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> h)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posx)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posy)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> millis)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> id)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> offset)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        while(stream >> tmp_id >> tmp_r >> tmp_g >> tmp_b) {
                            vec.push_back(std::make_pair<game_id, sf::Color>(static_cast<game_id>(tmp_id), sf::Color(tmp_r, tmp_g, tmp_b)));
                        }
                        AssetCreator::createSpriteSet(mgr, texture, vec, sf::IntRect(left, top, w, h), vec2f(posx, posy), sf::milliseconds(millis), static_cast<game_id>(id != 0 ? id : mgr.getFreeSpriteId()), offset);
                    }
                    break;

                case Game::FileOp::SetLayer:
                    {
                        int id, layer;
                        if(!(stream >> id)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> layer)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        mgr.getSprite(static_cast<game_id>(id)).setLayer(layer);                       
                    }
                    break;

                case Game::FileOp::SetBlur:
                    {
                        int id, blur;
                        if(!(stream >> id)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> blur)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        mgr.getSprite(static_cast<game_id>(id)).blurs(static_cast<bool>(blur));                     
                    }
                    break;

                case Game::FileOp::SetPlayer:
                    {
                        int sprite, posx, posy, id;
                        float speed;
                        if(!(stream >> sprite)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posx)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posy)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> speed)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> id)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        gmgr.setPlayer(gmgr.addActor(static_cast<game_id>(sprite), vec2f(posx, posy), speed, static_cast<game_id>(id != 0 ? id : mgr.getFreeSpriteId())));                      
                    }
                    break;

                case Game::FileOp::AddNPC:
                    {
                        int sprite, posx, posy, id;
                        float speed, locx, locy, loct;
                        if(!(stream >> sprite)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posx)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> posy)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> speed)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        if(!(stream >> id)) { std::cerr << "Malformed save file " << filePath << " at line " << i << std::endl; return false; }
                        NPC* npc = gmgr.addNPC(static_cast<game_id>(sprite), vec2f(posx, posy), speed, static_cast<game_id>(id != 0 ? id : mgr.getFreeSpriteId()));
                        while(stream >> locx >> locy >> loct) {
                            npc->addLocation(vec2f(locx, locy), sf::seconds(loct));
                        }
                    }
                    break;



                default:
                    std::cerr << "Error while parsing scene file " << filePath << " : unknown operation code #" << op << " at line " << i << ", Omitting the line." << std::endl;
                    return false;
            }

        }
    }

    return true;
}