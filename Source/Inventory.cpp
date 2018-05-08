#include "Inventory.h"

Item::Item(std::string name, std::string description, std::string icon, game_id id, bool consumes) : name(name), description(description), id(id), consumes(consumes) {
    if(!image.loadFromFile(icon)) {
        image.create(64, 64);
    }
}

game_id Item::getId() const { return id; }

sf::Image& Item::getImage() { return image; }


std::string Item::getName() {
    return name;
}

std::string Item::getDescription() {
    return description;
}


bool Item::consumesOnUse() const { return consumes; }


Inventory::Inventory(int size) : maxSize(size) {

}

Inventory::~Inventory() {
    for(auto& i : items) {
        delete i.second;
    }
}

bool Inventory::addItem(game_id id) {
    for(auto& i : items) {
        if(i.second->getId() == id) {
            i.first++;
            return true;
        }
    }
    if(items.size() < maxSize) {
        items.push_back(std::make_pair<int, Item*>(1, allocNewItem(id)));
        return true;
    }
    return false;
}

void Inventory::deleteItem(game_id id) {
    auto it = std::find_if(items.begin(), items.end(), [id](std::pair<int, Item*> i){ return i.second->getId() == id; });
    if(it != items.end()) {
        items.erase(it);
    }
}

Item* Inventory::getItem(game_id id) {
    for(auto& i : items) {
        if(i.second->getId() == id) {
            return i.second;
        }
    }
    return nullptr;
}

std::vector<std::pair<int, Item*>>& Inventory::getItems() {
    return items;
}

void Inventory::useItem(game_id id, AbstractItemInteractionProvider* itp) {
    for(auto& i : items) {
        if(i.second->getId() == id) {
            i.second->use(itp);
            if(i.second->consumesOnUse()) {
                i.first--;
                if(i.first == 0) {
                    deleteItem(id);
                }
            }
            return;
        }
    }
}

Item* Inventory::allocNewItem(game_id id) {
    switch(id) {
        case 10: return new TeleportationOrb();
        default: return new DefaultItem();
    }
}

DefaultItem::DefaultItem() : Item("default", "This is a sample item", "Assets/redorb.png", 0, true) {}

void DefaultItem::use(AbstractItemInteractionProvider* itp) {
    std::cout << "Using default item" << std::endl;
}


TeleportationOrb::TeleportationOrb() : Item("Orbe de téléportation", "Cet orbe vous teleporte au point de coordonnees (0;0). ", "Assets/redorb.png", 1, true) {}

void TeleportationOrb::use(AbstractItemInteractionProvider* itp) {

}