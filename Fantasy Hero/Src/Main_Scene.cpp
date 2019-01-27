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
#include "End_Scene.h"

Main_Environment envMain;

int textCounter1;

bool textBool1;
bool removeEnemy = true;
bool platformSwitch;

MainScene::MainScene(const std::shared_ptr<GBAEngine> engine) : Scene(engine), scrollX(0), scrollY(0), scrollLevel(0) {}

std::vector<Sprite *> MainScene::sprites(){
    std::vector<Sprite *> sprites;
    sprites.clear();

    if(envMain.getEnvironment1()){
        sprites.push_back(PlatformSprite.get());
        for (auto &p : platforms)
        {
            sprites.push_back(p->getSprite());
        }
    }

    sprites.push_back(Hero.get());
    if (!removeEnemy)
    {
        sprites.push_back(Enemy.get());
    }
    return sprites;
}

std::vector<Background *> MainScene::backgrounds()
{
    std::vector<Background *> bgs;
    bgs.clear();
    bgs.push_back(bgMoving.get());
    bgs.push_back(bgLevel.get());
    return bgs;
}

void MainScene::load()
{
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(Object_Sprites_Sharedpal, sizeof(Object_Sprites_Sharedpal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Main_Scene_BackgroundPal, sizeof(Main_Scene_BackgroundPal)));

    envMain.CreateEnvironment();

    spriteBuilder = std::unique_ptr<SpriteBuilder<Sprite>>(new SpriteBuilder<Sprite>);

    PlatformSprite = spriteBuilder-> withData(Platform_SingleTiles, sizeof(Platform_SingleTiles))
    .withSize(SIZE_16_16)
    .withLocation(214, envMain.getYLowerBound() + 15 - 48)
    .buildPtr();

    Hero = spriteBuilder -> withData(HeroTiles, sizeof(HeroTiles))
        .withSize(SIZE_32_32)
        .withAnimated(4, 5)
        .withLocation(envMain.getXStart(), envMain.getYLowerBound())
        .withinBounds()
        .buildPtr();
    Hero->stopAnimating();

    Enemy = spriteBuilder -> withData(HeroTiles, sizeof(HeroTiles))
        .withSize(SIZE_32_32)
        .withAnimated(4, 5)
        .withLocation(96, envMain.getYLowerBound())
        .withinBounds()
        .buildPtr();

    engine->enqueueMusic(Main_Scene_Audio, Main_Scene_Audio_bytes, 88200);

    bgLevel = std::unique_ptr<Background>(new Background(1, Main_Scene_BackgroundTiles, sizeof(Main_Scene_BackgroundTiles), Main_Scene_Background1Map, sizeof(Main_Scene_Background1Map)));
    bgLevel -> useMapScreenBlock(23);

    //schakel BG 3 uit om te kunnen spelen.....
    bgMoving = std::unique_ptr<Background>(new Background(2, Main_Scene_BackgroundTiles, sizeof(Main_Scene_BackgroundTiles), Moving_Background2Map, sizeof(Moving_Background2Map)));
    bgMoving->useMapScreenBlock(7);

    engine -> setTransition(false);
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
            platforms.push_back(createPlatform(102, envMain.getYLowerBound() + 15));
            platforms.push_back(createPlatform(166, envMain.getYLowerBound() + 15));
            engine->updateSpritesInScene();
            platformSwitch = false;
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

        //sprite switch to prevent overloading...
        if (Hero->getX() <= 122)
        {
            if (platformSwitch){
                removePlatforms();
                platforms.push_back(createPlatform(54, envMain.getYLowerBound() + 15));
                platforms.push_back(createPlatform(102, envMain.getYLowerBound() + 15));
                engine->updateSpritesInScene();
                platformSwitch = false;
            }

            //platform 1...
            if (Hero->getX() <= 34 && Hero->getX() >= 32 && Hero->getY() >= envMain.getYLowerBound() || Hero->getX() >= 58 && Hero->getX() <= 62 && Hero->getY() >= envMain.getYLowerBound())
            {
                envMain.setOffPlatform1();
                envMain.setOnSurface();
            }

            if (Hero->collidesWith(*platforms.at(0)->getSprite()) && Hero->getY() >= envMain.getYHigherBound() + 15 && Hero->getX() >= 34 && Hero->getX() <= 58)
            {
                envMain.setOffSurface();
                envMain.setOnPlatform1();
                Hero->moveTo(Hero->getX(), envMain.getYLowerBound());
            }
            else if (Hero->collidesWith(*platforms.at(0)->getSprite()) && Hero->getY() >= envMain.getYLowerBound() && Hero->getX() <= 34 && Hero->getX() >= 25)
            {
                Hero->moveTo(24, envMain.getYLowerBound());
            }
            else if (Hero->collidesWith(*platforms.at(0)->getSprite()) && Hero->getY() >= envMain.getYLowerBound() && Hero->getX() >= 58 && Hero->getX() <= 63)
            {
                Hero->moveTo(64, envMain.getYLowerBound());
            }

            //spikes between platforms...
            if(Hero -> getX() >= 54 && Hero -> getX() <= 86 && Hero -> getY() >= 108 && !engine->getTransition()){
                engine->dequeueAllSounds();
                engine->setTransition(true);

                bgMoving->clearMap();
                bgMoving->clearData();
                bgLevel->clearMap();
                bgLevel->clearData();

                envMain.setDead(true);

                removePlatforms();

                engine->transitionIntoScene(new EndScene(engine), new FadeOutScene(2));
            }

            //Platform 2...
            if (Hero->getX() <= 82 && Hero->getX() >= 80 && Hero->getY() >= envMain.getYLowerBound() || Hero->getX() >= 106 && Hero->getX() <= 110 && Hero->getY() >= envMain.getYLowerBound())
            {
                envMain.setOffPlatform1();
                envMain.setOnSurface();
            }

            if (Hero->collidesWith(*platforms.at(1)->getSprite()) && Hero->getY() >= envMain.getYHigherBound() + 15 && Hero->getX() >= 86 && Hero->getX() <= 104)
            {
                envMain.setOffSurface();
                envMain.setOnPlatform1();
                Hero->moveTo(Hero->getX(), envMain.getYLowerBound());
            }
            else if (Hero->collidesWith(*platforms.at(1)->getSprite()) && Hero->getY() >= envMain.getYLowerBound() && Hero->getX() <= 80 && Hero->getX() >= 73)
            {
                Hero->moveTo(72, envMain.getYLowerBound());
            }
            else if (Hero->collidesWith(*platforms.at(1)->getSprite()) && Hero->getY() >= envMain.getYLowerBound() && Hero->getX() >= 104 && Hero->getX() <= 113)
            {
                Hero->moveTo(114, envMain.getYLowerBound());
            }
        }
        else if (Hero->getX() > 122)
        {
            if (!platformSwitch){
                removePlatforms();
                platforms.push_back(createPlatform(166, envMain.getYLowerBound() + 15));
                platforms.push_back(createPlatform(182, envMain.getYLowerBound() + 15 - 16));
                platforms.push_back(createPlatform(198, envMain.getYLowerBound() + 15 - 32));
                //platforms.push_back(createPlatform(214, envMain.getYLowerBound() + 15 - 48));
                engine->updateSpritesInScene();
                platformSwitch = true;
            }

            //platform 3...
            if (Hero->getX() <= 144 && Hero->getX() >= 142 && Hero->getY() >= envMain.getYLowerBound())
            {
                envMain.setOffPlatform1();
                envMain.setOnSurface();
            }

            if (Hero->collidesWith(*platforms.at(0)->getSprite()) && Hero->getY() >= envMain.getYHigherBound() + 15 && Hero->getX() >= 142)
            {
                envMain.setOffSurface();
                envMain.setOnPlatform1();
                Hero->moveTo(Hero->getX(), envMain.getYLowerBound());
            }
            else if (Hero->collidesWith(*platforms.at(0)->getSprite()) && Hero->getY() >= envMain.getYLowerBound() && Hero->getX() <= 149 && Hero->getX() >= 137)
            {
                Hero->moveTo(138, envMain.getYLowerBound());
            }

            //platform 4...
            if (Hero->getX() <= 162 && Hero->getX() >= 160 && Hero->getY() >= envMain.getYLowerBound())
            {
                envMain.setOffPlatform2();
                envMain.setOnPlatform1();
            }
            if (Hero->collidesWith(*platforms.at(1)->getSprite()) && Hero->getY() >= envMain.getYHigherBound() + 15 && Hero->getX() >= 160)
            {
                envMain.setOffPlatform1();
                envMain.setOnPlatform2();
                Hero->moveTo(Hero->getX(), envMain.getYLowerBound());
            }
            else if (Hero->collidesWith(*platforms.at(1)->getSprite()) && Hero->getY() >= envMain.getYLowerBound() && Hero->getX() <= 143 && Hero->getX() >= 155)
            {
                Hero->moveTo(156, envMain.getYLowerBound());
            }

            //platform 5...
            if (Hero->getX() <= 180 && Hero->getX() >= 178 && Hero->getY() >= envMain.getYLowerBound())
            {
                envMain.setOffPlatform3();
                envMain.setOnPlatform2();
            }
            if (Hero->collidesWith(*platforms.at(2)->getSprite()) && Hero->getY() >= envMain.getYHigherBound() + 15 && Hero->getX() >= 178)
            {
                envMain.setOffPlatform2();
                envMain.setOnPlatform3();
                Hero->moveTo(Hero->getX(), envMain.getYLowerBound());
            }
            else if (Hero->collidesWith(*platforms.at(2)->getSprite()) && Hero->getY() >= envMain.getYLowerBound() && Hero->getX() <= 159 && Hero->getX() >= 171)
            {
                Hero->moveTo(172, envMain.getYLowerBound());
            }

            //platform 6...
            if (Hero->getX() <= 193 && Hero->getX() >= 191 && Hero->getY() >= envMain.getYLowerBound())
            {
                envMain.setOffPlatform4();
                envMain.setOnPlatform3();
            }
            if (Hero->collidesWith(*PlatformSprite.get()) && Hero->getY() >= envMain.getYHigherBound() + 15 && Hero->getX() >= 191)
            {
                envMain.setOffPlatform3();
                envMain.setOnPlatform4();
                Hero->moveTo(Hero->getX(), envMain.getYLowerBound());
            }
            else if (Hero->collidesWith(*PlatformSprite.get()) && Hero->getY() >= envMain.getYLowerBound() && Hero->getX() <= 174 && Hero->getX() >= 186)
            {
                Hero->moveTo(187, envMain.getYLowerBound());
            }
        }   
    }
}       

void MainScene::checkEnvironment2(u16 keys){
    if(envMain.getEnvironment2()){
        if (envMain.getBuildEnvironment()){
            removePlatforms();
            engine->updateSpritesInScene();
            envMain.setBuildEnvironment(false);
        }

        if(Hero -> getX() >= 85 && Hero -> getY() >= envMain.getYLowerBound()){
            envMain.setOffPlatform4();
            envMain.setOnSurface();
        }

        if(Hero -> getX() >= 0 && Hero -> getX() <= 70 && Hero -> getY() >= 108 && !engine->getTransition()){
            engine->dequeueAllSounds();
            engine->setTransition(true);

            bgMoving->clearMap();
            bgMoving->clearData();
            bgLevel->clearMap();
            bgLevel->clearData();

            envMain.setDead(true);

            removePlatforms();

            engine->transitionIntoScene(new EndScene(engine), new FadeOutScene(2));
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

void MainScene::checkEnvironment3(u16 keys){
    if(envMain.getEnvironment3()){
        if(envMain.getBuildEnvironment()){
            removePlatforms();
            removeEnemy = false;
            Enemy->flipHorizontally(true);
            engine->updateSpritesInScene();
            envMain.setBuildEnvironment(false);
        }

        if(Hero -> collidesWith(*Enemy) && keys & KEY_A){
            if (!textBool1)
            {
            TextStream::instance() << "You killed the enemy! Great Job!";
            textBool1 = true;
            }
            removeEnemy = true;
            engine->updateSpritesInScene();
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
            bgMoving -> updateMap(Moving_Background2Map);
            envMain.setBuildEnvironment(true);
            removeEnemy = true;
            envMain.goToEnvironment2();
        }
    }
}

void MainScene::tick(u16 keys)
{
    Hero -> stopAnimating();

    scrollX += 0.5;
    bgMoving->scroll(scrollX, scrollY);

    textCounter1++;
    if (textCounter1 == 100)
    {
        TextStream::instance().clear();
        textCounter1 = 0;
        textBool1 = false;
    }

    checkEnvironment3(keys);
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
