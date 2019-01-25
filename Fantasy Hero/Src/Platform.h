#ifndef GBA_SPRITE_ENGINE_PROJECT_PLATFORM_H
#define GBA_SPRITE_ENGINE_PROJECT_PLATFORM_H

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/gba/tonc_math.h>

class Platform
{
  private:
    std::unique_ptr<Sprite> sprite;

  public:
    Platform(std::unique_ptr<Sprite> sprite) : sprite(std::move(sprite)) {}

    bool isOffScreen() { return sprite->isOffScreen(); }
    Sprite *getSprite() { return sprite.get(); }
};

#endif //GBA_SPRITE_ENGINE_PROJECT_PLATFORM_H
