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

SpriteBuilder<Sprite> builder;
int textCounter;
bool textBool;

std::vector<Sprite *> MainScene::sprites()
{
    return {
        Hero.get(),
        Enemy.get()};
}

std::vector<Background *> MainScene::backgrounds()
{
    return {
        bgLevel.get(),
        bgMoving.get()};
}

void MainScene::load()
{
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(Object_Sprites_Sharedpal, sizeof(Object_Sprites_Sharedpal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Main_Scene_BackgroundPal, sizeof(Main_Scene_BackgroundPal)));

    CreateEnviroment();

    Hero = builder
               .withData(HeroTiles, sizeof(HeroTiles))
               .withSize(SIZE_32_32)
               .withAnimated(4, 5)
               .withLocation(XStart, YLowerBound)
               .withinBounds()
               .buildPtr();
    Hero->stopAnimating();

    engine->enqueueMusic(Main_Scene_Audio, Main_Scene_Audio_bytes, 88200);

    bgLevel = std::unique_ptr<Background>(new Background(1, Main_Scene_BackgroundTiles, sizeof(Main_Scene_BackgroundTiles), Main_Scene_BackgroundMap, sizeof(Main_Scene_BackgroundMap)));
    bgLevel.get()->useMapScreenBlock(16);

    bgLevel->scroll(0, 95);
}

void MainScene::tick(u16 keys)
{
    Hero->stopAnimating();

    textCounter++;
    if (textCounter == 100)
    {
        TextStream::instance().clear();
        textCounter = 0;
        textBool = false;
    }

    if (Hero->getY() >= YLowerBound)
    {
        setIsJumped(false);
        setGravityOn(false);
    }
    else if (Hero->getY() <= YHigherBound && getIsJumped())
    {
        setGravityOn(true);
    }

    if (getIsJumped() && !getGravityOn())
    {
        Hero->animateToFrame(5);
        Hero->setVelocity(0, -2);
    }
    else if (getGravityOn())
    {
        Hero->animateToFrame(1);
        Hero->setVelocity(0, 2);
    }
    else if (!gravityOn && !keys)
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

        if (Hero->getY() < YLowerBound)
            Hero->setVelocity(-2, 2);
        else
            Hero->setVelocity(-2, 0);
    }

    else if (keys & KEY_RIGHT)
    {
        Hero->flipHorizontally(true);
        Hero->animate();

        if (Hero->getY() < YLowerBound)
            Hero->setVelocity(2, 2);
        else
            Hero->setVelocity(2, 0);
    }

    else if (keys & KEY_UP)
    {
        if (!getIsJumped())
        {
            setIsJumped(true);
            engine->enqueueSound(Jump_Audio, Jump_Audio_bytes, 88200);
            Hero->animateToFrame(5);
            Hero->setVelocity(0, -2);
        }
    }

    else if (keys & KEY_A)
    {
        engine->enqueueSound(Slash_Audio, Slash_Audio_bytes, 88200);
        Hero->animateToFrame(7);

        if (!textBool)
        {
            TextStream::instance() << Hero->getX() << "&&" << Hero->getY(); //coordinate checker....
            textBool = true;
        }
    }
}
