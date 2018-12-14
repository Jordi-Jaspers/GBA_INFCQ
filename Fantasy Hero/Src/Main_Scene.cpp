#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>

//include objecten voor deze scene
#include "Main_Scene.h"
#include "Sound_Effect.h"
#include "Environment.h"
#include "Main_Scene_Audio.h"
#include "Object_Sprites.h"
#include "Object_Sprites_Shared.h"
#include "Main_Scene_Background.h"

Environment envMain;

std::vector<Sprite *> MainScene::sprites()
{
    return {
        Hero.get()};
}

std::vector<Background *> MainScene::backgrounds()
{
    return {
        bgMoving.get(),
        bgLevel.get()
    };
}

void MainScene::load()
{
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(Object_Sprites_Sharedpal, sizeof(Object_Sprites_Sharedpal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Main_Scene_BackgroundPal, sizeof(Main_Scene_BackgroundPal)));

    envMain.CreateEnviroment();
    engine -> disableText();

    SpriteBuilder<Sprite> builder;

    Hero = builder
        .withData(HeroTiles, sizeof(HeroTiles))
        .withSize(SIZE_32_32)
        .withAnimated(4, 5)
        .withLocation(envMain.getXStart(), envMain.getYLowerBound())
        .withinBounds()
        .buildPtr();
    Hero->stopAnimating();

    engine->enqueueMusic(Main_Scene_Audio, Main_Scene_Audio_bytes, 88200);

    bgMoving = std::unique_ptr<Background>(new Background(3, Main_Scene_BackgroundTiles, sizeof(Main_Scene_BackgroundTiles), Main_Scene_Background1Map, sizeof(Main_Scene_Background1Map)));
    bgMoving->useMapScreenBlock(16);
    bgMoving->scroll(0, 0);

    bgLevel = std::unique_ptr<Background>(new Background(1, Main_Scene_BackgroundTiles, sizeof(Main_Scene_BackgroundTiles), Main_Scene_BackgroundMap, sizeof(Main_Scene_BackgroundMap)));
    bgLevel -> useMapScreenBlock(16);
    bgLevel -> scroll(0, 95);
}

void MainScene::tick(u16 keys)
{
    scrollX += 0.5;
    bgMoving->scroll(scrollX, 0);

    Hero->stopAnimating();

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
        TextStream::instance() << Hero->getX() << "&&" << Hero->getY(); //coordinate checker....
    }
}
