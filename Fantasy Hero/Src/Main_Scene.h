#ifndef GBA_SPRITE_ENGINE_MAIN_SCENE_H
#define GBA_SPRITE_ENGINE_MAIN_SCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>
#include "Platform.h"

class MainScene : public Scene
{
  private:
    std::unique_ptr<Background> bgLevel;
    std::unique_ptr<Background> bgMoving;
    std::unique_ptr<Sprite> Hero;
    std::unique_ptr<Sprite> Enemy;
    std::unique_ptr<Sprite> PlatformSprite;
    
    std::vector<std::unique_ptr<Platform>> platforms;

    std::unique_ptr<SpriteBuilder<Sprite>> spriteBuilder;
    std::unique_ptr<Platform> createPlatform(int xCo, int yCo);
    
    void removePlatforms();

    float scrollX, scrollY, scrollLevel;

  public:
    explicit MainScene(std::shared_ptr<GBAEngine> engine);
    MainScene(MainScene &other) = delete;

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void enemyController();
    void checkEnvironment1();
    void checkEnvironment2(u16 i);
    void checkEnvironment3(u16 i);
    void load() override;
    void tick(u16 i) override;
};

#endif //GBA_SPRITE_ENGINE_FLYING_STUFF_SCENE_H
