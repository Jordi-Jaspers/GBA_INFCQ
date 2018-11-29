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
#include "Stage_Start_Audio.h"


//include overgang naar volgende scene
//#include "Battle_Scene.h"

std::vector<Background *> StartScene::backgrounds() {
    return { bg.get() };
}

std::vector<Sprite *> StartScene::sprites() {
    return {};
}

void StartScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(Start_Scene_BackgroundPal, sizeof(Start_Scene_BackgroundPal)));
 
    bg = std::unique_ptr<Background>(new Background(1, Start_Scene_BackgroundTiles, sizeof(Start_Scene_BackgroundTiles), Start_Scene_BackgroundMap, sizeof(Start_Scene_BackgroundMap)));
    bg.get()->useMapScreenBlock(16);
    
    TextStream::instance().setText("PRESS START", 16, 10);
    TextStream::instance().setFontColor(16);
   
    engine->enqueueMusic(Start_Scene_Audio, Start_Scene_Audio_bytes);
}

void StartScene::tick(u16 keys) {
    if(scrollY !=  48){
        scrollY -= 1;
        bg.get()->scroll(scrollX, scrollY);
    }

    if(keys & KEY_START) {
        engine->enqueueSound(Stage_Start_Audio, Stage_Start_Audio_bytes);

        if(!engine->isTransitioning()) {
            //engine->transitionIntoScene(new Battle_Scene(engine), new FadeOutScene(2));
        }
    } else if(keys & KEY_LEFT) {
        
    } else if(keys & KEY_RIGHT) {
        
    } else if(keys & KEY_UP) {
       
    } else if(keys & KEY_DOWN) {
        
    }
}
