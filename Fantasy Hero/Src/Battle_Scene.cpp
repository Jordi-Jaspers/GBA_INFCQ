#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/background/text_stream.h>

#include "Battle_Scene.h"

std::vector<Sprite *> BattleScene::sprites() {
    return {
    };
}

std::vector<Background *> BattleScene::backgrounds() {
    return {
    };
}

void BattleScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager());

    SpriteBuilder<Sprite> builder;
    SpriteBuilder<AffineSprite> affineBuilder;
}

void BattleScene::tick(u16 i) {

}
