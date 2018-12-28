project(Fantasy_Hero)

add_executable(${PROJECT_NAME}.elf
        src/Battle_Scene_Audio.h
		src/Battle_Scene_Background.h
		src/Battle_Scene.cpp
		src/Battle_Scene.h
		src/Battle_Environment.h
		src/End_Scene.cpp
		src/End_Scene.h	
		src/End_Scene_Audio.h	
		src/Main_Scene_Audio.h
		src/Main_Scene_Background.h
		src/Main_Scene.cpp
		src/Main_Scene.h
		src/Main_Environment.h
		src/main.cpp
		src/Object_Sprites.h
		src/Sound_Effect.h
		src/Start_Scene_Audio.h
        src/start_Scene.h
        src/start_Scene.cpp
		src/Start_Scene_Background.h
		src/Start_Scene_Objects.h
		)

target_link_libraries(${PROJECT_NAME}.elf gba-sprite-engine)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
        )
