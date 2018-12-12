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
#include "Environment.h"

SpriteBuilder<Sprite> builder;

std::vector<Sprite *> BattleScene::sprites() {
    return {
    Platform1.get(),
    // Platform2.get(),
    Hero.get(),
    Star.get()
    };
}

std::vector<Background *> BattleScene::backgrounds() {
    return {
        bg.get()
    };
}

void BattleScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(sharedPal, sizeof(sharedPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Battle_Scene_BackgroundPal, sizeof(Battle_Scene_BackgroundPal)));

    CreateEnviroment();

    Platform1 = builder
        .withSize(SIZE_32_16)
        .withData(Platform_DoubleTiles, sizeof(Platform_DoubleTiles))
        .withLocation(203, YLowerBound+14)
        .buildPtr();

    // Platform2 = builder
    //     .withSize(SIZE_32_8)
    //     .withLocation(XStart, YLowerBound)
    //     .buildPtr();

    Star = builder
        .withData(StarTiles, sizeof(StarTiles))
        .withSize(SIZE_32_32)
        .withAnimated(3, 10)
        .withLocation(200, 65)
        .buildPtr();

    Hero = builder
        .withData(HeroTiles, sizeof(HeroTiles))
        .withSize(SIZE_32_32)
        .withAnimated(4, 5)
        .withLocation(XStart, YLowerBound)
        .withinBounds()
        .buildPtr();
    Hero -> stopAnimating();

    engine->enqueueMusic(Battle_Scene_Audio, Battle_Scene_Audio_bytes, 88200);

    bg = std::unique_ptr<Background>(new Background(1, Battle_Scene_BackgroundTiles, sizeof(Battle_Scene_BackgroundTiles), Battle_Scene_BackgroundMap, sizeof(Battle_Scene_BackgroundMap)));
    bg.get()->useMapScreenBlock(16);

    bg->scroll(5, 95);
}

void BattleScene::checkMainStageObstacles(){
    if(getMainStage()){
        if (Hero->getX() <= 175 && Hero->getY() == YLowerBound)
        {
            setOffPlatform();
        }

        if (Hero->collidesWith(*Platform1) && Hero->getY() >= YHigherBound+10 && Hero->getX() >= 175){
            setOnPlatform();
        }
        else if (Hero->collidesWith(*Platform1) && Hero->getY() >= YLowerBound)
        {
        Hero->moveTo(170, YLowerBound);
        }

        if(Hero -> getX() >= 200 && Hero -> getY() == YHigherBound && getOnPlatform()){
            NextEnviroment();
            Hero -> moveTo(10, YLowerBound);
            bg->scroll(5, 0);
        }
    }
}

void BattleScene::checkSubStageObstacles()
{
    if (!getMainStage()){
        if (Hero->getX() <= 4 && Hero->getY() == YLowerBound)
        {
            MainEnviroment();
            Hero->moveTo(208, YLowerBound);
            bg->scroll(5, 95);
        }
    }
}

void BattleScene::tick(u16 keys) {

    Hero->stopAnimating();

    checkMainStageObstacles();
    checkSubStageObstacles();

    if (Hero->getY() == YLowerBound && getIsJumped())
    {
        setIsJumped(false);
        setGravityOn(false);
    }
    else if (Hero->getY() == YHigherBound && getIsJumped())
    {
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

        if (Hero->getY() < YLowerBound)
            Hero->setVelocity(-2, 2);
        else
            Hero->setVelocity(-2, 0);
    }

    else if (keys & KEY_RIGHT)
    {   
        Hero -> flipHorizontally(true);
        Hero -> animate();

        if (Hero->getY() < YLowerBound)
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
        TextStream::instance() << Hero->getX() << "&&" << Hero->getY(); //coordinate checker....
        Hero->animateToFrame(7);
    }
}
