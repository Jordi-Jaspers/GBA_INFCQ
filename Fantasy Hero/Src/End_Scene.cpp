#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <algorithm>
#include <libgba-sprite-engine/background/text_stream.h>
#include <sstream>
#include "End_Scene.h"
#include "End_Scene_Audio.h"
#include "Sound_Effect.h"
#include "Main_Environment.h"

Main_Environment envEnd;

EndScene::EndScene(const std::shared_ptr<GBAEngine> engine) : Scene(engine)
{
}

std::vector<Sprite *> EndScene::sprites(){return{};}

std::vector<Background *> EndScene::backgrounds(){return {};}

void EndScene::load()
{
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager());
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager());
    engine->enqueueMusic(End_Scene_Audio, End_Scene_Audio_bytes, 88200);

    if(envEnd.getDead()){
        engine->enqueueSound(Dead_Audio, Dead_Audio_bytes, 88200);

        TextStream::instance().setText("Hoe kun je nu doodgaan sukkel...", 8, 0);
        TextStream::instance().setText("Dit spel is voor simpele mensen...", 9, 0);
        TextStream::instance().setText("CTRL+R voor restart.", 10, 0);
    }    
    else{
        TextStream::instance().setText("Je hebt het monster gedood!", 8, 0);
        TextStream::instance().setText("Goed Gedaan!", 9, 0);
        TextStream::instance().setText("CTRL+R voor restart.", 10, 0);
    }
}

void EndScene::tick(u16 keys){}
