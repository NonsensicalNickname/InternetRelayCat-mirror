cmake_minimum_required (VERSION 3.11)
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # Important: Specify a version or a commit hash here.
)
FetchContent_MakeAvailable(tomlplusplus ftxui)
 
project(IRCat LANGUAGES CXX VERSION 1.0.0)
add_executable(irc.out src/irc_client.cpp)
target_link_libraries(irc.out
  PRIVATE tomlplusplus::tomlplusplus
)
target_link_libraries(irc.out
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component 
)

