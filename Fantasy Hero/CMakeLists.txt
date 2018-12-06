project(Fantasy_Hero)

add_executable(${PROJECT_NAME}.elf
        src/Battle_Scene_Audio.h
		src/Battle_Scene_Background.h
		src/Battle_Scene_Objects.h
		src/Battle_Scene_Shared.h
		src/Battle_Scene.h
		src/Battle_Scene.cpp
		src/Knight_Dance_Object.h
		src/main.cpp
		src/Start_Scene_Shared.h
		src/Press_Start_Object1.h
		src/Press_Start_Object2.h
		src/Stage_Start_Audio.h
		src/Start_Scene_Audio.h
		src/Start_Scene_Background.h
        src/start_Scene.cpp
        src/start_Scene.h
		)

target_link_libraries(${PROJECT_NAME}.elf gba-sprite-engine)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
        )
