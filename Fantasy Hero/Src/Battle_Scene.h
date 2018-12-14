#ifndef GBA_SPRITE_ENGINE_BATTLE_SCENE_H
#define GBA_SPRITE_ENGINE_BATTLE_SCENE_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>

class BattleScene : public Scene
{
  private:
    std::unique_ptr<Background> bg;
    std::unique_ptr<Sprite> Hero;
    std::unique_ptr<Sprite> Star;
    std::unique_ptr<Sprite> Platform;
    int scrollX, scrollY;

  public:
    explicit BattleScene(std::shared_ptr<GBAEngine> engine);
    BattleScene(BattleScene &other) = delete;

    void checkMainStageObstacles();
    void checkSubStageObstacles();

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 i) override;
};

#endif
