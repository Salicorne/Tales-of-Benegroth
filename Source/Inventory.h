#pragma once
#include "Memory.h"

class AbstractItemInteractionProvider {
    //public:
        //virtual void foo() = 0;
};

class Item {
    protected:
        std::string name;
        std::string description;
        sf::Image image;
        game_id id;
        bool consumes;

    public:
        Item(std::string name, std::string description, std::string icon, game_id id, bool consumes);
        game_id getId() const;
        sf::Image& getImage();
        std::string getName();
        std::string getDescription();
        bool consumesOnUse() const;
        virtual void use(AbstractItemInteractionProvider*) = 0;
};

class Inventory {
    protected:
        std::vector<std::pair<int, Item*>> items;
        int maxSize;
        
    public:
        Inventory(int size);
        ~Inventory();
        bool addItem(game_id id);
        void deleteItem(game_id id);
        Item* getItem(game_id item);
        std::vector<std::pair<int, Item*>>& getItems();
        void useItem(game_id id, AbstractItemInteractionProvider* itp);
        Item* allocNewItem(game_id id);
};



class DefaultItem : public Item {
    public:
        DefaultItem();
        void use(AbstractItemInteractionProvider* itp);
};

class TeleportationOrb : public Item {
    public: 
        TeleportationOrb();
        void use(AbstractItemInteractionProvider* itp);
};
