#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>

//include objecten voor deze scene
#include "Start_Scene.h"
#include "Start_Scene_Audio.h"
#include "Start_Scene_Background.h"
#include "Start_Scene_Objects.h"

//include overgang naar volgende scene
#include "Battle_Scene.h"

std::vector<Background *> StartScene::backgrounds() {
    return { bg.get() };
}

std::vector<Sprite *> StartScene::sprites() {
    return { startButton1.get(), startButton2.get(), knightDance.get() };
}

void StartScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(sharedPal, sizeof(sharedPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Start_Scene_BackgroundPal, sizeof(Start_Scene_BackgroundPal)));
    engine.get()->disableText();

    SpriteBuilder<Sprite> builder;
    
    startButton1 = builder
        .withData(Press_Start_Object1Tiles, sizeof(Press_Start_Object1Tiles))
        .withSize(SIZE_64_64)
        .withAnimated(2, 15)
        .withLocation(61, 90)
        .buildPtr();

    startButton2 = builder
        .withData(Press_Start_Object2Tiles, sizeof(Press_Start_Object2Tiles))
        .withSize(SIZE_64_64)
        .withAnimated(2, 15)
        .withLocation(125, 90)
        .buildPtr();

    knightDance = builder
        .withData(Knight_DanceTiles, sizeof(Knight_DanceTiles))
        .withSize(SIZE_16_32)
        .withAnimated(4, 25)
        .withLocation(115, 65)
        .buildPtr();

    bg = std::unique_ptr<Background>(new Background(1, Start_Scene_BackgroundTiles, sizeof(Start_Scene_BackgroundTiles), Start_Scene_BackgroundMap, sizeof(Start_Scene_BackgroundMap)));
    bg.get()->useMapScreenBlock(24);
   
    engine->enqueueMusic(Start_Scene_Audio, Start_Scene_Audio_bytes, 88200);
}

void StartScene::tick(u16 keys) {
    if(scrollY !=  0){
        scrollY -= 0.5;
        bg.get()->scroll(scrollX, scrollY);
    }

    if(keys & KEY_START) {
        engine->enqueueSound(Stage_Start_Audio, Stage_Start_Audio_bytes);

        if(!engine->isTransitioning()) {
            engine->dequeueAllSounds();
            engine->transitionIntoScene(new BattleScene(engine), new FadeOutScene(2));
            engine->setTransition(false);
        }

    } else if(keys & KEY_LEFT) {
        
    } else if(keys & KEY_RIGHT) {
        
    } else if(keys & KEY_UP) {
       
    } else if(keys & KEY_DOWN) {
        
    }
}
