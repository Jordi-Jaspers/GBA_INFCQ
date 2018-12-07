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
#include "Battle_Scene_Shared.h"
#include "Battle_Scene_Objects.h"

SpriteBuilder<Sprite> builder;

int Xco_Hero;
int Yco_Hero;

std::vector<Sprite *> BattleScene::sprites() {
    return {
    Hero.get()
    //Enemy.get(),
    //Star.get()
    };
}

std::vector<Background *> BattleScene::backgrounds() {
    return {
        bg2.get()
    };
}

void BattleScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(sharedPal, sizeof(sharedPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Battle_Scene_BackgroundPal, sizeof(Battle_Scene_BackgroundPal)));

    SpriteBuilder<Sprite> builder;

    bg2 = std::unique_ptr<Background>(new Background(1, Battle_Scene_BackgroundTiles, sizeof(Battle_Scene_BackgroundTiles), Battle_Scene_BackgroundMap, sizeof(Battle_Scene_BackgroundMap)));
    bg2.get()->useMapScreenBlock(24);

    Hero = builder
        .withData(Hero_Walk_LeftTiles, sizeof(Hero_Walk_LeftTiles))
        .withSize(SIZE_16_32)
        .withAnimated(4, 5)
        .withLocation(0, 0)
        .buildPtr();
    Hero -> stopAnimating();    

    // Enemy = builder
    //                    .withData(Press_Start_Object2Tiles, sizeof(Press_Start_Object2Tiles))
    //                    .withSize(SIZE_64_64)
    //                    .withLocation(0, 0)
    //                    .buildPtr();

    // Star = builder
    //                   .withData(Knight_DanceTiles, sizeof(Knight_DanceTiles))
    //                   .withSize(SIZE_16_32)
    //                   .withLocation(0, 0)
    //                   .buildPtr();
}


void BattleScene::tick(u16 keys) {
    Hero -> setVelocity(0,0);
    Hero -> stopAnimating();

    if (keys & KEY_START)
    {
        if (engine -> isDisabled()){
            engine.get()->enableText();
        }
        else{
            engine.get()->disableText();
        }
    }
    else if (keys & KEY_LEFT)
    {
        Hero -> flipHorizontally(false);
        Hero -> setVelocity(-2, 0);
        Hero -> animate();
    }
    else if (keys & KEY_RIGHT)
    {   
        Hero -> flipHorizontally(true);
        Hero -> setVelocity(2, 0);
        Hero -> animate();
    }
    else if (keys & KEY_UP)
    {
    }
    else if (keys & KEY_DOWN)
    {
    }
}
