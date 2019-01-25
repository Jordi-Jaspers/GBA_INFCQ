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
bool removeData = true;
bool removeEnemy = true;

MainScene::MainScene(const std::shared_ptr<GBAEngine> engine) : Scene(engine), scrollX(0), scrollY(0), scrollLevel(0) {}

std::vector<Sprite *> MainScene::sprites(){
    std::vector<Sprite *> sprites;
    sprites.clear();

    for (auto &p : platforms)
    {
        sprites.push_back(p->getSprite());
    }

    if(!removeData){
        sprites.push_back(Object.get());
    }
    if(!removeEnemy){
        sprites.push_back(Enemy.get());
    }

    sprites.push_back(Hero.get());
    return sprites;
}

std::vector<Background *> MainScene::backgrounds()
{
    std::vector<Background *> bgs;
    bgs.clear();

    bgs.push_back(bgLevel.get());
    bgs.push_back(bgMoving.get());

    return bgs;
}

void MainScene::load()
{
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(Object_Sprites_Sharedpal, sizeof(Object_Sprites_Sharedpal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Main_Scene_BackgroundPal, sizeof(Main_Scene_BackgroundPal)));

    envMain.CreateEnvironment();
    engine -> disableText();

    spriteBuilder = std::unique_ptr<SpriteBuilder<Sprite>>(new SpriteBuilder<Sprite>);

    Object = spriteBuilder -> withData(Platform_SingleTiles, sizeof(Platform_SingleTiles))
    .withSize(SIZE_16_16)
    .withLocation(54, envMain.getYLowerBound() + 15)
    .buildPtr();

    PlatformSprite = spriteBuilder-> withData(Platform_SingleTiles, sizeof(Platform_SingleTiles))
    .withSize(SIZE_16_16)
    .withLocation(0, envMain.getYLowerBound() + 15)
    .buildPtr();

    Hero = spriteBuilder -> withData(HeroTiles, sizeof(HeroTiles))
        .withSize(SIZE_32_32)
        .withAnimated(4, 5)
        .withLocation(envMain.getXStart(), envMain.getYLowerBound())
        .withinBounds()
        .buildPtr();
    Hero->stopAnimating();

    Enemy = spriteBuilder -> withData(EnemyTiles, sizeof(EnemyTiles))
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

std::unique_ptr<Platform> MainScene::createPlatform(int xCo, int yCo){
    return std::unique_ptr<Platform>(
        new Platform(spriteBuilder -> withLocation(xCo, yCo)
                                     .buildWithDataOf(*PlatformSprite.get())));
}

void MainScene::removePlatforms(){
    platforms.erase(platforms.begin(), platforms.end());
}

void MainScene::checkEnvironment1()
{
    if(envMain.getEnvironment1()){
        if(envMain.getBuildEnvironment()){
            removePlatforms();
            platforms.push_back(createPlatform(54, envMain.getYLowerBound() + 15));
            platforms.push_back(createPlatform(70, envMain.getYLowerBound() + 15 - 16));
            platforms.push_back(createPlatform(86, envMain.getYLowerBound() + 15 - 16));
            platforms.push_back(createPlatform(102, envMain.getYLowerBound() + 15));
            engine->updateSpritesInScene();
            envMain.setBuildEnvironment(false);
        }

        bgLevel->scroll(10, 5);
        if(Hero -> getX() >= envMain.getXRightBound() - 5){
            bgLevel -> updateMap(Main_Scene_Background2Map);
            bgLevel -> scroll(10, 5);
            Hero -> moveTo(envMain.getXStart() + 7, Hero -> getY());
            envMain.setBuildEnvironment(true);
            envMain.goToEnvironment2();
        }
    }
}

void MainScene::checkEnvironment2(u16 keys){
    if(envMain.getEnvironment2()){
        if (envMain.getBuildEnvironment()){
            removePlatforms();
            platforms.push_back(createPlatform(54, envMain.getYLowerBound() + 15));
            platforms.push_back(createPlatform(70, envMain.getYLowerBound() + 15 - 16));
            platforms.push_back(createPlatform(86, envMain.getYLowerBound() + 15 - 16));
            platforms.push_back(createPlatform(102, envMain.getYLowerBound() + 15));
            engine->updateSpritesInScene();
            envMain.setBuildEnvironment(false);
        }

        if(Hero -> getX() <= envMain.getXLeftBound() + 5){
            bgLevel->updateMap(Main_Scene_Background1Map);
            bgLevel->scroll(10, 5);
            Hero -> moveTo(envMain.getXRightBound() - 7, Hero -> getY());
            envMain.setBuildEnvironment(true);
            envMain.goToEnvironment1();
        }

        if (Hero->getX() >= envMain.getXRightBound() - 5 && keys & KEY_DOWN){
            bgLevel->updateMap(Main_Scene_Background3Map);
            scrollLevel = 25;
            bgLevel->scroll(scrollLevel, 5);
            Hero->moveTo(envMain.getXRightBound(), envMain.getYLowerBound());
            bgMoving -> clearMap();
            envMain.setBuildEnvironment(true);
            envMain.goToEnvironment3();
        }
    }
}

void MainScene::checkEnvironment3(){
    if(envMain.getEnvironment3()){
        if(envMain.getBuildEnvironment()){
            removePlatforms();
            platforms.push_back(createPlatform(54, envMain.getYLowerBound() + 15));
            platforms.push_back(createPlatform(70, envMain.getYLowerBound() + 15 - 16));
            platforms.push_back(createPlatform(86, envMain.getYLowerBound() + 15 - 16));
            platforms.push_back(createPlatform(102, envMain.getYLowerBound() + 15));
            engine->updateSpritesInScene();
            envMain.setBuildEnvironment(false);
        }

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
            envMain.setBuildEnvironment(true);
            envMain.goToEnvironment2();
        }
    }
}

void MainScene::tick(u16 keys)
{
    Hero -> stopAnimating();

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
