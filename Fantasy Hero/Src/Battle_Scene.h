#ifndef GBA_SPRITE_ENGINE_BATTLE_SCENE_H
#define GBA_SPRITE_ENGINE_BATTLE_SCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>

class BattleScene : public Scene {
private:
    
public:
    BattleScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 i) override;
};


#endif //GBA_SPRITE_ENGINE_FLYING_STUFF_SCENE_H
