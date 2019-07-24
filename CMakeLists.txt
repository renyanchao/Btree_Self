cmake_minimum_required(VERSION 3.15)

aux_source_directory(. SRC_LIST)
add_executable(Btree ${SRC_LIST})