#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include <time.h>

//include objecten voor deze scene
#include "Battle_Scene.h"
#include "Sound_Effect.h"
#include "Battle_Scene_Audio.h"
#include "Battle_Scene_Background.h"
#include "Object_Sprites.h"
#include "Battle_Environment.h"

//include overgang naar volgende scene
#include "Main_Scene.h"
#include "End_Scene.h"

Battle_Environment envBattle;

int textCounter;
int starCounter;

bool textBool;
bool removeStar = true;
bool removePlatform = false;

BattleScene::BattleScene(const std::shared_ptr<GBAEngine> engine) : Scene(engine), scrollX(0), scrollY(0) {}

std::vector<Background *> BattleScene::backgrounds() {
    return {
        bg.get()
    };
}

std::vector<Sprite *> BattleScene::sprites() {
    std::vector<Sprite *> sprites;
    sprites.clear();

    if(!removeStar){
        sprites.push_back(Star.get());
    }
    if(!removePlatform){
        sprites.push_back(Platform.get());
    }
    sprites.push_back(Hero.get());
    return sprites;
}

void BattleScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(Object_Sprites_Sharedpal, sizeof(Object_Sprites_Sharedpal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Battle_Scene_BackgroundPal, sizeof(Battle_Scene_BackgroundPal)));
    SpriteBuilder<Sprite> builder;

    envBattle.CreateEnviroment();
    removeStar = true;
    removePlatform = false;

    Platform = builder
        .withData(Platform_DoubleTiles, sizeof(Platform_DoubleTiles))
        .withSize(SIZE_32_16)
        .withLocation(203, envBattle.getYLowerBound() + 14)
        .buildPtr();

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
        .withLocation(envBattle.getXStart(), envBattle.getYLowerBound())
        .withinBounds()
        .buildPtr();
    Hero -> stopAnimating();

    engine->enqueueMusic(Battle_Scene_Audio, Battle_Scene_Audio_bytes, 88200);

    bg = std::unique_ptr<Background>(new Background(1, Battle_Scene_BackgroundTiles, sizeof(Battle_Scene_BackgroundTiles), Battle_Scene_BackgroundMap, sizeof(Battle_Scene_BackgroundMap)));
    bg.get()->useMapScreenBlock(16);
    bg->scroll(5, 95);
}

void BattleScene::checkMainStageObstacles(){
    if(envBattle.getMainStage()){
        if (Hero->getX() <= 185 && Hero->getY() >= envBattle.getYLowerBound())
        {
            envBattle.setOffPlatform();
        }

        if (Hero->collidesWith(*Platform) && Hero->getY() >= envBattle.getYHigherBound() + 15 && Hero->getX() >= 185)
        {
            envBattle.setOnPlatform();
            Hero->moveTo(Hero->getX(), envBattle.getYLowerBound());
        }
        else if (Hero->collidesWith(*Platform) && Hero->getY() >= envBattle.getYLowerBound())
        {
            Hero->moveTo(170, envBattle.getYLowerBound());
        }

        if(Hero -> getX() >= 200 && Hero -> getY() == envBattle.getYHigherBound() && envBattle.getOnPlatform()){
            envBattle.NextEnviroment();
            Hero -> moveTo(10, envBattle.getYLowerBound());
            bg->scroll(5, 0);
            removeStar = false;
            removePlatform = true;
        }

        if (Hero->getY() <= envBattle.getYHigherBound() && !engine->getTransition())
        {
            if (Hero->getX() >= 45 && Hero->getX() <= 70 || Hero->getX() >= 80 && Hero->getX() <= 100 || Hero->getX() >= 110 && Hero->getX() <= 135){
                engine->dequeueAllSounds();
                engine->setTransition(true);

                TextStream::instance().clear();

                bg->clearMap();
                bg->clearData();
                envBattle.setDead(true);
                removePlatform = true;

                engine->transitionIntoScene(new EndScene(engine), new FadeOutScene(2));
            }
        }
    }
}

void BattleScene::checkSubStageObstacles()
{
    if (!envBattle.getMainStage()){
        if (Hero->getX() <= 4 && Hero->getY() == envBattle.getYLowerBound())
        {
            envBattle.MainEnviroment();
            Hero->moveTo(208, envBattle.getYLowerBound());
            bg->scroll(5, 95);
            removePlatform = false;
            removeStar = true;
        }

        if (Hero->collidesWith(*Star) && Hero->getX() >= 190 && Hero->getX() <= 208)
        {
            starCounter ++;

            if (!textBool && starCounter >= 30)
            {
                TextStream::instance() << "by touching the star";
                TextStream::instance() << "you started remembering";
                TextStream::instance() << "something...";
                textBool = true;
            }

            if (!engine->getTransition() && starCounter == 100)
            {
                engine->dequeueAllSounds();
                engine->setTransition(true);
                TextStream::instance().clear();
                bg->clearMap();
                bg->clearData();
                engine->transitionIntoScene(new MainScene(engine), new FadeOutScene(2));
            }
        }else{
            starCounter = 0;
        }
    }
}

void BattleScene::tick(u16 keys) {
    engine -> updateSpritesInScene();
    Hero->stopAnimating();

    checkMainStageObstacles();
    checkSubStageObstacles();

    textCounter ++;
    if(textCounter == 100){
        TextStream::instance().clear();
        textCounter = 0;
        textBool = false;
    }

    if (Hero->getY() >= envBattle.getYLowerBound())
    {
        envBattle.setIsJumped(false);
        envBattle.setGravityOn(false);
    }
    else if (Hero->getY() <= envBattle.getYHigherBound() && envBattle.getIsJumped())
    {
        envBattle.setGravityOn(true);
    }

    if (envBattle.getIsJumped() && !envBattle.getGravityOn()){
        Hero->animateToFrame(5);
        Hero->setVelocity(0, -2);
    }
    else if(envBattle.getGravityOn()){
        Hero->animateToFrame(1);
        Hero->setVelocity(0, 2);
    }
    else if(!envBattle.getGravityOn() && !keys){
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

        if (Hero->getY() < envBattle.getYLowerBound())
            Hero->setVelocity(-2, 2);
        else
            Hero->setVelocity(-2, 0);
    }

    else if (keys & KEY_RIGHT)
    {   
        Hero -> flipHorizontally(true);
        Hero -> animate();

        if (Hero->getY() < envBattle.getYLowerBound())
            Hero->setVelocity(2, 2);
        else
            Hero->setVelocity(2, 0);
    }

    else if (keys & KEY_UP)
    {
        if (!envBattle.getIsJumped()){
            envBattle.setIsJumped(true);
            engine -> enqueueSound(Jump_Audio, Jump_Audio_bytes, 88200);
            Hero->animateToFrame(5);
            Hero->setVelocity(0, -2);
        }
    }

    else if (keys & KEY_A)
    {
        engine->enqueueSound(Slash_Audio, Slash_Audio_bytes, 88200);
        Hero->animateToFrame(7);


        if(!textBool){
            TextStream::instance() << Hero->getX() << "&&" << Hero->getY(); //coordinate checker....
            textBool = true;
        }
    }
}
