#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>

//include objecten voor deze scene
#include "Battle_Scene.h"
#include "Sound_Effect.h"
#include "Battle_Scene_Audio.h"
#include "Battle_Scene_Background.h"
#include "Battle_Scene_Shared.h"
#include "Battle_Scene_Objects.h"

bool isJumped;
bool gravityOn;

std::vector<Sprite *> BattleScene::sprites() {
    return {
    Hero.get()
    //Enemy.get(),
    //Star.get()
    };
}

std::vector<Background *> BattleScene::backgrounds() {
    return {
        bg.get()
    };
}

void setIsJumped(bool jumped){
    isJumped = jumped;
}

bool getIsJumped(){
    return isJumped;
}

void setGravityOn(bool gravity){
    gravityOn = gravity;
}

bool getGravityOn(){
    return gravityOn;
}

void BattleScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(sharedPal, sizeof(sharedPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Battle_Scene_BackgroundPal, sizeof(Battle_Scene_BackgroundPal)));

    SpriteBuilder<Sprite> builder;

    engine->enqueueMusic(Battle_Scene_Audio, Battle_Scene_Audio_bytes, 88200);

    bg = std::unique_ptr<Background>(new Background(1, Battle_Scene_BackgroundTiles, sizeof(Battle_Scene_BackgroundTiles), Battle_Scene_BackgroundMap, sizeof(Battle_Scene_BackgroundMap)));
    bg.get() -> useMapScreenBlock(16);

    Hero = builder
        .withData(HeroTiles, sizeof(HeroTiles))
        .withSize(SIZE_32_32)
        .withAnimated(4, 5)
        .withLocation(0, 108)
        .buildPtr();
    Hero -> stopAnimating();

    bg->scroll(5, 95); 

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

    Hero->stopAnimating();

    if(Hero -> getY() == 108 && getIsJumped()){
        setIsJumped(false);
        setGravityOn(false);
    }
    else if(Hero -> getY() == 80 && getIsJumped()){
        setGravityOn(true);
    }

    if (getIsJumped() && !getGravityOn()){
        Hero->animateToFrame(5);
        Hero->setVelocity(0, -2);
    }
    else if(getGravityOn()){
        Hero->animateToFrame(1);
        Hero->setVelocity(0, 2);
    }
    else if(!gravityOn && !keys){
        Hero->animateToFrame(1);
        Hero->setVelocity(0, 0);
    }

    if (keys & KEY_START)
    {
        if (engine -> isDisabled())
            engine.get()->enableText();
        else
            engine.get()->disableText();
    }

    else if (keys & KEY_LEFT)
    {
        Hero->flipHorizontally(false);
        Hero->animate();

        if (Hero->getY() < 108)
            Hero->setVelocity(-2, 2);
        else
            Hero->setVelocity(-2, 0);
    }

    else if (keys & KEY_RIGHT)
    {   
        Hero -> flipHorizontally(true);
        Hero -> animate();

        if (Hero->getY() < 108)
            Hero->setVelocity(2, 2);
        else
            Hero->setVelocity(2, 0);
    }

    else if (keys & KEY_UP)
    {
        if (!getIsJumped()){
            setIsJumped(true);
            engine -> enqueueSound(Jump_Audio, Jump_Audio_bytes, 88200);
            Hero->animateToFrame(5);
            Hero->setVelocity(0, -2);
        }
    }

    else if (keys & KEY_A)
    {
        engine->enqueueSound(Slash_Audio, Slash_Audio_bytes, 88200);
        Hero->animateToFrame(7);
    }
}
