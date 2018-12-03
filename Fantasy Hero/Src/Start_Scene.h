#ifndef GBA_SPRITE_ENGINE_START_SCENE_H
#define GBA_SPRITE_ENGINE_START_SCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>

/**
 * @brief 
 * Een klasse voor het maken van een beginscherm
 */
class StartScene : public Scene {
private:
    std::unique_ptr<Background> bg;
    std::unique_ptr<Sprite> startButton1;
    std::unique_ptr<Sprite> startButton2;
    int scrollX, scrollY;

public:
    StartScene(std::shared_ptr<GBAEngine> engine) : Scene(engine), scrollX(8), scrollY(96)  {}

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 keys) override;
};

#endif