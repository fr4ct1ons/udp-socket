﻿# CMakeList.txt: projeto de CMake para Server, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

# Adicione a origem ao executável deste projeto.
add_executable (Client1 "Client_1.cpp" "Client.h")
add_executable (Client2 "Client_2.cpp" "Client.h")

#target_link_libraries(Server wsock32 ws2_32)

# TODO: Adicione testes e instale destinos, se necessário.
