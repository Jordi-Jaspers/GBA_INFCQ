#ifndef GBA_SPRITE_ENGINE_BATTLE_SCENE_H
#define GBA_SPRITE_ENGINE_BATTLE_SCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>

class BattleScene : public Scene {
private:
    std::unique_ptr<Background> bg;
    std::unique_ptr<Sprite> Hero;
    std::unique_ptr<Sprite> Enemy;
    std::unique_ptr<Sprite> Star;
    int scrollX, scrollY;

public:
    BattleScene(std::shared_ptr<GBAEngine> engine) : Scene(engine), scrollX(0), scrollY(0) {}

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 i) override;
};


#endif //GBA_SPRITE_ENGINE_FLYING_STUFF_SCENE_H
