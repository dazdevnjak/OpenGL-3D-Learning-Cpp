#pragma once
#include "GameObject.h"

class Utilities {
public:
    static GameObject* find_game_object(
        const std::vector<std::unique_ptr<GameObject>>& objects,
        const std::string& target_name)
    {
        auto it = std::find_if(objects.begin(), objects.end(),
            [&](const std::unique_ptr<GameObject>& obj) {
                return obj->name == target_name;
            });
        if (it != objects.end()) {
            return it->get();
        }
        return nullptr;
    }
};
