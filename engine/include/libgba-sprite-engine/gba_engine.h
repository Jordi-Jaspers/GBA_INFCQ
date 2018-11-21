//
// Created by Wouter Groeneveld on 28/07/18.
//

#ifndef GBA_SPRITE_ENGINE_GBAENGINE_H
#define GBA_SPRITE_ENGINE_GBAENGINE_H


#include <libgba-sprite-engine/sprites/sprite_manager.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/effects/scene_effect.h>
#include "scene.h"
#include "sound_control.h"

#define GBA_SCREEN_WIDTH 240
#define GBA_SCREEN_HEIGHT 160

class GBAEngine {
private:
    // WHY raw pointers? the engine does the transition and cleanup work itself
    Scene* currentScene;
    Scene* sceneToTransitionTo;
    SceneEffect* currentEffectForTransition;

    SpriteManager spriteManager;

    static std::unique_ptr<SoundControl> activeChannelA;
    static std::unique_ptr<SoundControl> activeChannelB;

    void vsync();
    void cleanupPreviousScene();
    void enqueueSound(const s8 *data, int totalSamples, int sampleRate, SoundChannel channel);

    static void startOnVBlank() { REG_IME = 1; }
    static void stopOnVBlank() { REG_IME = 0; }
    static void onVBlank();

public:
    GBAEngine();

    void setScene(Scene* scene);
    void dynamicallyAddSprite(Sprite* s) { spriteManager.add(s); }
    void transitionIntoScene(Scene* scene, SceneEffect* effect);
    bool isTransitioning() { return currentEffectForTransition != nullptr; }

    void dequeueAllSounds();
    void enqueueMusic(const s8 *data, int totalSamples, int sampleRate = 16000) {
        enqueueSound(data, totalSamples, sampleRate, ChannelA);
    }
    void enqueueSound(const s8 *data, int totalSamples, int sampleRate = 16000) {
        enqueueSound(data, totalSamples, sampleRate, ChannelB);
    }

    u16 readKeys();
    void update();
    void updateSpritesInScene();
    void delay(int times) {
        for(int i = 0; i < times; i++){}
    }
};


#endif //GBA_SPRITE_ENGINE_GBAENGINE_H
