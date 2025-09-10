cmake_minimum_required(VERSION 3.16)
project(Shark_ui LANGUAGES CXX)

# Сообщаем корню, что мы запущены как standalone
set(SHARK_UI_STANDALONE ON CACHE INTERNAL "UI standalone build")

# Если цели не определены (standalone), подтягиваем родителя ОДИН РАЗ
if(NOT TARGET dto)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_BINARY_DIR}/_root_build)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
qt_standard_project_setup()

qt_add_executable(Shark_ui
  main.cpp
  screen_main_window.cpp
  screen_main_window.h
  screen_main_window.ui
  screen_login.h screen_login.cpp screen_login.ui
  screen_register.h screen_register.cpp screen_register.ui

  errorbus.h
  exception_router.h
  exception_login.h
  exception_network.h
  exception_sql.h
  exception_valid.h
  nw_connection_monitor.h nw_connection_monitor.cpp
  model_chat_list.h model_chat_list.cpp
  model_chat_list_delegate.h model_chat_list_delegate.cpp
  model_user_list.h model_user_list.cpp
  model_user_list_delegate.h model_user_list_delegate.cpp
  screen_main_work.h screen_main_work.cpp screen_main_work.ui
  images/resources.qrc
  model_chat_messages.h model_chat_messages.cpp
  screen_chat_list.ui
  screen_chat_list.h screen_chat_list.cpp
  screen_chatting.h screen_chatting.cpp screen_chatting.ui
  model_chat_mess_delegate.h model_chat_mess_delegate.cpp

)

# AUTOUIC/MOC заголовки лежат в бинарной папке
target_include_directories(Shark_ui PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/Shark_ui_autogen/include
  ${CMAKE_CURRENT_SOURCE_DIR}                # Shark_UI/*
  ${CMAKE_CURRENT_SOURCE_DIR}/../src         # src/*
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/client  # src/client/*
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/core    # src/core/*
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/dto     # src/dto/*
)

target_link_libraries(Shark_ui PRIVATE
  Qt6::Core
  Qt6::Widgets
  clientlib
  core
  dto
)
target_link_libraries(Shark_ui PRIVATE Qt6::Core)
