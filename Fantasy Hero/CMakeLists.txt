project(Fantasy_Hero)

add_executable(${PROJECT_NAME}.elf
        src/Battle_Scene_Audio.h
		src/Battle_Scene_Background.h
		src/Battle_Scene.h
		src/main.cpp
		src/shared.h
		src/Stage_Start_Audio.h
		src/Start_Scene_Audio.h
		src/Start_Scene_Background.h
		src/Start_Scene_Background1.h
		src/Start_Scene_Background2.h
        src/start_Scene.cpp
        src/start_Scene.h
		)

target_link_libraries(${PROJECT_NAME}.elf gba-sprite-engine)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
        )
