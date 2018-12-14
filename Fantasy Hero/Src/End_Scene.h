#ifndef GBA_SPRITE_ENGINE_END_SCENE_H
#define GBA_SPRITE_ENGINE_END_SCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>

class EndScene : public Scene {
private:

public:
    explicit EndScene(std::shared_ptr<GBAEngine> engine);
    EndScene(EndScene &other) = delete;

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 i) override;
};


#endif