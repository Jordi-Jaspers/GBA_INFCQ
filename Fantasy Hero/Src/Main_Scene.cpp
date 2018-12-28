#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>

//include objecten voor deze scene
#include "Main_Scene.h"
#include "Sound_Effect.h"
#include "Main_Environment.h"
#include "Main_Scene_Audio.h"
#include "Object_Sprites.h"
#include "Main_Scene_Background.h"

Main_Environment envMain;
bool removeData = false;
bool removeEnemy = true;

MainScene::MainScene(const std::shared_ptr<GBAEngine> engine) : Scene(engine), scrollX(0), scrollY(0), scrollLevel(0) {}

std::vector<Sprite *> MainScene::sprites(){
    std::vector<Sprite *> sprites;
    sprites.clear();

    if(!removeData){
        sprites.push_back(Object.get());
    }
    if(!removeEnemy){
        sprites.push_back(Enemy.get());
    }

    sprites.push_back(Hero.get());
    return sprites;
}

std::vector<Background *> MainScene::backgrounds(){return {bgMoving.get(), bgLevel.get()};}

void MainScene::load()
{
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(Object_Sprites_Sharedpal, sizeof(Object_Sprites_Sharedpal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Main_Scene_BackgroundPal, sizeof(Main_Scene_BackgroundPal)));

    envMain.CreateEnvironment();
    engine -> disableText();

    SpriteBuilder<Sprite> builder;

    Object = builder
    .withData(Platform_DoubleTiles, sizeof(Platform_DoubleTiles))
    .withSize(SIZE_32_16)
    .withLocation(48, envMain.getYLowerBound() + 14)
    .buildPtr();

    Hero = builder
        .withData(HeroTiles, sizeof(HeroTiles))
        .withSize(SIZE_32_32)
        .withAnimated(4, 5)
        .withLocation(envMain.getXStart(), envMain.getYLowerBound())
        .withinBounds()
        .buildPtr();
    Hero->stopAnimating();

    Enemy = builder
        .withData(EnemyTiles, sizeof(EnemyTiles))
        .withSize(SIZE_32_32)
        .withAnimated(1, 4)
        .withLocation(96, envMain.getYLowerBound())
        .withinBounds()
        .buildPtr();
    Enemy->stopAnimating();

    engine->enqueueMusic(Main_Scene_Audio, Main_Scene_Audio_bytes, 88200);

    bgLevel = std::unique_ptr<Background>(new Background(0, Main_Scene_Background1Tiles, sizeof(Main_Scene_Background1Tiles), Main_Scene_Background1Map, sizeof(Main_Scene_Background1Map)));
    bgLevel -> useMapScreenBlock(24);

    //schakel BG 3 uit om te kunnen spelen.....
    bgMoving = std::unique_ptr<Background>(new Background(2, Main_Scene_BackgroundTiles, sizeof(Main_Scene_BackgroundTiles), Main_Scene_BackgroundMap, sizeof(Main_Scene_BackgroundMap)));
    bgMoving->useMapScreenBlock(16);
}

void MainScene::checkEnvironment1()
{
    if(envMain.getEnvironment1()){
        bgLevel->scroll(10, 5);
        if(Hero -> getX() >= envMain.getXRightBound() - 5){
            bgLevel -> updateMap(Main_Scene_Background2Map);
            bgLevel -> scroll(10, 5);
            Hero -> moveTo(envMain.getXStart() + 7, Hero -> getY());
            envMain.goToEnvironment2();
        }
    }
}

void MainScene::checkEnvironment2(u16 keys){
    if(envMain.getEnvironment2()){
        if(Hero -> getX() <= envMain.getXLeftBound() + 5){
            bgLevel->updateMap(Main_Scene_Background1Map);
            bgLevel->scroll(10, 5);
            Hero -> moveTo(envMain.getXRightBound() - 7, Hero -> getY());
            envMain.goToEnvironment1();
        }

        if (Hero->getX() >= envMain.getXRightBound() - 5 && keys & KEY_DOWN){
            bgLevel->updateMap(Main_Scene_Background3Map);
            scrollLevel = 25;
            bgLevel->scroll(scrollLevel, 5);
            Hero->moveTo(envMain.getXRightBound(), envMain.getYLowerBound());
            bgMoving -> clearMap();
            envMain.goToEnvironment3();
        }
    }
}

void MainScene::checkEnvironment3(){
    if(envMain.getEnvironment3()){
        if (Hero->getX() < 200 && scrollLevel >= 0)
        {
            scrollLevel -= 1;
            bgLevel->scroll(scrollLevel, 5);
        }

        if (Hero->getX() > 200 && scrollLevel <= 25)
        {
            scrollLevel += 1;
            bgLevel->scroll(scrollLevel, 5);
        }

        if(Hero -> getX() >= envMain.getXRightBound() && envMain.getIsJumped()){
            bgLevel->updateMap(Main_Scene_Background2Map);
            bgLevel->scroll(10, 5);
            Hero->moveTo(Hero -> getX(), Hero->getY());
            bgMoving -> updateMap(Main_Scene_BackgroundMap);
            envMain.goToEnvironment2();
        }
    }
}

void MainScene::tick(u16 keys)
{
    engine -> updateSpritesInScene();
    Hero->stopAnimating();

    scrollX += 0.5;
    bgMoving->scroll(scrollX, scrollY);

    checkEnvironment3();
    checkEnvironment2(keys);
    checkEnvironment1();

    if (Hero->getY() >= envMain.getYLowerBound())
    {
        envMain.setIsJumped(false);
        envMain.setGravityOn(false);
    }
    else if (Hero->getY() <= envMain.getYHigherBound() && envMain.getIsJumped())
    {
        envMain.setGravityOn(true);
    }

    if (envMain.getIsJumped() && !envMain.getGravityOn())
    {
        Hero->animateToFrame(5);
        Hero->setVelocity(0, -2);
    }
    else if (envMain.getGravityOn())
    {
        Hero->animateToFrame(1);
        Hero->setVelocity(0, 2);
    }
    else if (!envMain.getGravityOn() && !keys)
    {
        Hero->animateToFrame(1);
        Hero->setVelocity(0, 0);
    }

    if (keys & KEY_START)
    {
        if (engine->isDisabled())
            engine.get()->enableText();
        else
            engine.get()->disableText();
    }

    else if (keys & KEY_LEFT)
    {
        Hero->flipHorizontally(false);
        Hero->animate();

        if (Hero->getY() < envMain.getYLowerBound())
            Hero->setVelocity(-2, 2);
        else
            Hero->setVelocity(-2, 0);
    }

    else if (keys & KEY_RIGHT)
    {
        Hero->flipHorizontally(true);
        Hero->animate();

        if (Hero->getY() < envMain.getYLowerBound())
            Hero->setVelocity(2, 2);
        else
            Hero->setVelocity(2, 0);
    }

    else if (keys & KEY_UP)
    {
        if (!envMain.getIsJumped())
        {
            envMain.setIsJumped(true);
            engine->enqueueSound(Jump_Audio, Jump_Audio_bytes, 88200);
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
