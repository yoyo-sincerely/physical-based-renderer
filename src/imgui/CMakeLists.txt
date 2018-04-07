set(libsrc 
imgui_internal.h
imconfig.h
imgui.cpp
imgui.h
imgui_draw.cpp
imgui_demo.cpp
imgui_user/imgui_user.cpp
imgui_user/imgui_user.h
imgui_user/imgui_tabs.cpp
imgui_user/imgui_tabs.h
imguizmo/imguizmo.cpp
imguizmo/imguizmo.h)

# add_library (imgui ${libsrc})

# if(MSVC)
# set(imgui_flags
# _CRT_SECURE_NO_WARNINGS
# _CRT_SECURE_NO_DEPRECATE)
# endif()

# set(imgui_flags ${imgui_flags}
# IMGUI_DISABLE_OBSOLETE_FUNCTIONS)

# target_compile_definitions(imgui PUBLIC ${imgui_flags})
# target_include_directories (imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(imgui glfw)