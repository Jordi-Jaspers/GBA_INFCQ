#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>

//include objecten voor deze scene
#include "Battle_Scene.h"
#include "Battle_Scene_Audio.h"
#include "Battle_Scene_Background.h"
#include "Battle_Scene_Objects.h"


std::vector<Sprite *> BattleScene::sprites() {
    return {
    Hero.get(),
    Enemy.get(),
    Star.get()
    };
}

std::vector<Background *> BattleScene::backgrounds() {
    return {
        bg2.get()
    };
}

void BattleScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Battle_Scene_BackgroundPal, sizeof(Battle_Scene_BackgroundPal)));

    SpriteBuilder<Sprite> builder;

    bg2 = std::unique_ptr<Background>(new Background(1, Battle_Scene_BackgroundTiles, sizeof(Battle_Scene_BackgroundTiles), Battle_Scene_BackgroundMap, sizeof(Battle_Scene_BackgroundMap)));
    bg2.get()->useMapScreenBlock(16);
}

void BattleScene::tick(u16 i) {

}
