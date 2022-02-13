project(imgui_sfml_example
  LANGUAGES CXX
)

add_executable(imgui_sfml_example
  main.cpp
)

target_link_libraries(imgui_sfml_example PRIVATE ImGui-SFML::ImGui-SFML)
