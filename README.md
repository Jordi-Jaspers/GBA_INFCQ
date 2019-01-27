#Author:  
Jordi Jaspers  
  
#Date of initial commit:  
16/11/2018  
  
#GBA_INFCQ [Biography]:  
This repository contains the GBA-Project for the course [INFCQ] 2018-2019. The Game is a combination between Final Fantasy and mario sprites.  
You can move between different stages and kill enemies. There is a working ending screen and a splash screen at the beginning.  
not quite a fatastic game but I tried. TURN OF BACKGROUND LAYER 3 TO PLAY THE GAME!  
  
the hero can attack by using the "A-key" on the gameboy. there is gravity and collision detection.  
Interact with the star for 5 seconds to go to the next level. walk all the way till the end.   
At the green pipe use the arrow down key to get to the boss level.  

ERRORS WITHING THE GAME:  
because there is still something wrong with the implementation of 2 backgrounds at the same time.    
the next scene cant load or there are some bad pixels withing the game itself. That is also    
also the reason why I commented that BG3 has to be of in order to play the game with minimal errors.  

There are some small errors within the gravity at the stairs. i think it's  because i overloaded the game with  
sprites, so the interactions with the sprites aren't that relaiable anymore.  

when I delete the mapping at the boss scene there are some green line through the screen. very weird  
because i didn't change anything within that level and i just started appearing.  
 
SOLUTION: a switch to remove and add platforms is a solution for the overload of sprites.  
  
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
    -> Score System         -> not yet....  
    -> created 2nd scene    -> Done!
    -> double moving scene  -> Done! don't know why there are ugly bits on the second bg layer. don't use it.....  
      
4.  Create Ending Screen    -> Done! (For dying and finishing the game.)
  
5.  Create an ER-Diagram    -> Done! (Don't know if it's good though...)
  
  
#References:  
Music           -->     https://downloads.khinsider.com/game-soundtracks/  
GBA Specs       -->     https://www.cs.rit.edu/~tjh8300/CowBite/CowBiteSpec.htm  
Used Sprites    -->     https://www.spriters-resource.com/  
Sprite engine   -->     https://github.com/wgroeneveld/gba-sprite-engine  
Syntax errors   -->     https://stackoverflow.com/
    
  
