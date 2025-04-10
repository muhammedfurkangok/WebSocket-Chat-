cmake_minimum_required(VERSION 3.20)

# Proje ismi
project(WebSocketChat)

# C++ standardını ayarla
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include dizinleri
include_directories(include)

# Kaynak dosyalar
set(SERVER_SOURCES
    src/frkServer.cpp
    src/frkNodeV4.cpp
)

set(CLIENT_SOURCES
    src/frkClient.cpp
    src/frkNodeV4.cpp
)

# Örnek dosyalar (main fonksiyonu içeren dosyalar)
set(SERVER_MAIN example/frkChatServer.cpp)
set(CLIENT_MAIN example/frkChatClient.cpp)

# Server uygulamasını oluştur
add_executable(frkChatServer ${SERVER_SOURCES} ${SERVER_MAIN})

# Client uygulamasını oluştur
add_executable(frkChatClient ${CLIENT_SOURCES} ${CLIENT_MAIN})

# UNIX sistemleri için pthread kütüphanesini bağla
if (UNIX)
    target_link_libraries(frkChatServer pthread)
    target_link_libraries(frkChatClient pthread)
endif()