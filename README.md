#Author:  
Jordi Jaspers  
  
#Date of initial commit:  
16/11/2018  
  
#GBA_INFCQ [Biography]:  
This repository contains the GBA-Project for the course [INFCQ] 2018-2019. The Game is a combination between Final Fantasy and mario sprites.  
You can move between different stages and kill enemies. There is a working ending screen and a splash screen at the beginning.  
not quite a fatastic game but I tried. TURN OF BACKGROUND LAYER 3 TO PLAY THE GAME!  

ERRORS WITHING THE GAME:  
because there is still something wrong with the implementation of 2 backgrounds at the same time.  
the next scene cant load or there are some bad pixels withing the game itself. That is also  
also the reason why I commented that BG3 has to be of in order to play the game with minimal errors.

I put some code in a file calles "Extra code" because that was all the collision detection code.  
but because of the weird background errors. the code doesnt work fully in the game. the game itself becomes   
realy slow and glitchy when it is used.
   
DISCLAIMER:  
All the code that is copied or used from other sources will be mentioned in the reference section of this file.  
  
#TODO list:  
1.  Build & Implement the overal GBA engine  
    -> Done!  
2.  Create the Start_Scene <br />
    -> Add Sounds           -> Done!  
    -> Add Sprites          -> Done!  
    -> Add Background       -> Done!  
    -> Change Font Color    -> Done!  
    -> Selection screen     -> Takes too much time (Start, Credits)  
      
3.  Create the Battle_Scene && Main_Scene  
    -> Add Sounds           -> Done!  
    -> Add Sprites          -> Done!  
    -> Create background    -> Done!  
    -> Add background       -> Done!   
    -> Implement Controls   -> Done! 
    -> Collision Detection  -> Done! 
    -> Score System         -> not yet
    -> created 2nd scene    -> Done!
    -> double moving scene  -> Done! don't know why there are ugly bits on the second bg layer. don't use it.....  
      
4.  Create Ending Screen    -> Done! 
  
  
#References:  
Music           -->     https://downloads.khinsider.com/game-soundtracks/  
GBA Specs       -->     https://www.cs.rit.edu/~tjh8300/CowBite/CowBiteSpec.htm  
Used Sprites    -->     https://www.spriters-resource.com/  
Sprite engine   -->     https://github.com/wgroeneveld/gba-sprite-engine  
    
  
