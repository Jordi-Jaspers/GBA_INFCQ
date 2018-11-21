#include "Objects.h"

#define WIDTH   240
#define HEIGHT  160

#define MEM_IO_register     0x04000000
#define MEM_Colour_pallete  0x05000000
#define VRAM                0x06000000
#define OAM_MEM_register    0x07000000

#define DISPLAY_MODE1               0x1000
#define DISPLAY_ENABLE_OBJECTS      0x0040

#define OAM_HIDE_MASK       0x300         
#define OAM_HIDE            0x200
#define Y_MASK              0x0FF
#define X_MASK              0x1FF

#define REG_DISPLAY     (*((volatile uint32 *)(MEM_IO_register)))
#define REG_VCOUNT      (*((volatile uint16 *)(MEM_IO_register + 0x0006)))
#define REG_KEY_INPUT   (*((volatile uint32 *)(MEM_IO_register + 0x0130)))

#define TILE_MEM        ((volatile tile_block *)(VRAM))
#define OAM_MEM         ((volatile object *)(OAM_MEM_register))
#define PALETTE_MEM     ((volatile palette *)(MEM_Colour_pallete + 0x200))

#define KEY_A       0x0001
#define KEY_B       0x0002
#define KEY_RIGHT   0X0010
#define KEY_LEFT    0x0020
#define KEY_UP      0x0040
#define KEY_DOWN    0x0080
#define KEY_ANY     0x03FF

int next_oam_mem;
int next_tile_mem = 1;

/**
 * @brief 
 * Set the color object
 * 16-bit colour format: BBBBB-GGGGG-RRRRR
 * Via logic operators zoals OR en AND voor de bits bij elkaar te plakken
 * 
 * @param red 
 * @param green 
 * @param blue 
 * @return uint16 
 */
uint16 set_color(uint16 red, uint16 green, uint16 blue) {

    uint16 c = (blue & 0x1f) << 10;
    c |= (green & 0x1f) << 5;
    c |= (red & 0x1f);
    return c;
}

/**
 * @brief 
 * creer een object met verschillende attributen
 * 
 * @param attr0 
 * @param attr1 
 * @param attr2 
 * @return volatile* create_object 
 */
volatile object* create_object(int attr0, int attr1, int attr2) {
    volatile object *obj = &OAM_MEM[next_oam_mem++];
    obj->attr0 = attr0;
    obj->attr1 = attr1;
    obj->attr2 = attr2;

    return obj;    
}

/**
 * @brief 
 * Kopieer het object dat al bestaat.
 * 
 * @param object 
 * @return volatile* copy_object 
 */
volatile object* copy_object(volatile object* other) {
    return create_object(other->attr0, other->attr1, other->attr2);
}

/**
 * @brief 
 * creer een sprite object
 * 
 * @param object 
 * @param initialx 
 * @param initialy 
 * @param width 
 * @param height 
 * @return sprite* 
 */
sprite* create_sprite(volatile object* obj, int initialx, int initialy, uint8 width, uint8 height) {
    sprite* s = (sprite*) malloc(sizeof(sprite));
    s->obj = obj;
    s->x = initialx;
    s->y = initialy;
    s->width = width;
    s->height = height;
    position(s);
    return s;
}

/**
 * @brief 
 * 
 * @param s 
 */
void position(sprite *s) {
    volatile object *obj = s->obj;
    int x = s->x;
    int y = s->y;
    obj->attr0 = (obj->attr0 &  ~Y_MASK) | (y & Y_MASK);
    obj->attr1 = (obj->attr1 & ~X_MASK) | (x & X_MASK);
}

/**
 * @brief 
 * is het object verstopt of niet?
 * 
 * @param s 
 * @return int 
 */
int is_hidden(sprite *s) {
    volatile object *obj = s->obj;
    return obj->attr0 & OAM_HIDE;
}

/**
 * @brief 
 * verstop het object.
 * 
 * @param s 
 */
void hide(sprite *s) {
    volatile object *obj = s->obj;
    obj->attr0 = (obj->attr0 & ~OAM_HIDE_MASK) | OAM_HIDE;
}

/**
 * @brief 
 * kijken als de gba aan het drawen is of niks aan het doen is.
 * zodat wij in de tussentijd andere dingen kunnen doen.
 */
void vsync(){
    while(REG_VCOUNT >= HEIGHT);
    while(REG_VCOUNT < HEIGHT);
}

/**
 * @brief 
 * inlezen als er een verandering is in MEM_IO_register.
 * zodat er een input herkent kan worden.
 * 
 * @return uint16 
 */
uint16 readKeys(){
    return ~REG_KEY_INPUT & KEY_ANY;
}

/**
 * @brief 
 * mode 1 wordt gebruik gemaakt van een buffer en kun je sprites beinvloeden ipv
 * mode 3 of 4 die iedere pixel onafhankelijk kan beinvloeden.
 * 
 */
void initScreen() {
    REG_DISPLAY = DISPLAY_MODE1 | DISPLAY_ENABLE_OBJECTS;
}

/**
 * @brief 
 * De main-game loop voor de GBA-game.
 * 
 * @return int 
 */
int main(){
    uint16 key_states;
    
    //init all objects....
    initScreen();

    //Game-loop
    while(true) {
        vsync();
        key_states = readKeys();

        if(key_states & KEY_RIGHT) {

        }
        if(key_states & KEY_LEFT) {
 
        }
        if(key_states & KEY_UP) {

        }
        if (key_states & KEY_DOWN) {

        }
        if(key_states & KEY_A) {

        }
        if(key_states & KEY_B) {

        }
    }
}