# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    CMakeLists.txt                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/09 04:33:35 by abelov            #+#    #+#              #
#    Updated: 2025/09/09 04:33:35 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

cmake_minimum_required(VERSION 3.27)

#set(CMAKE_C_COMPILER clang)
#set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_COMPILER gcc-13)
set(COMMON_LDFLAGS
		-fsanitize=address,undefined,bounds,alignment,object-size
		-fsanitize=shift,signed-integer-overflow,null,return
		-fsanitize=float-divide-by-zero,float-cast-overflow
		-fno-sanitize-recover
)

set(COMMON_LIBS
		m
)

set(COMMON_CFLAGS
		-Wall -Wextra -Werror
		-Wwrite-strings
		-Wimplicit
		-Wno-error=unused-parameter
		-Wno-error=unused-variable
		-O0
		-masm=att
		-fno-exceptions
		-ffunction-sections
		-fdata-sections
		-fno-omit-frame-pointer
		-fdwarf2-cfi-asm
		-fcf-protection=none
		-mmanual-endbr
		-g3 -gdwarf-3
		-fasynchronous-unwind-tables
		-fsanitize=address,undefined,bounds,alignment,object-size
		-fsanitize=shift,signed-integer-overflow,null,return
		-fsanitize=float-divide-by-zero,float-cast-overflow
		-fno-sanitize-recover
)

include_directories(include)

project(bsq2 C)

set(TARGET_EXE_NAME "${PROJECT_NAME}")

add_executable(${TARGET_EXE_NAME}
		src/main.c
)

target_compile_definitions(${TARGET_EXE_NAME} PRIVATE USE_SIMPLE )
target_compile_options(${TARGET_EXE_NAME} PRIVATE ${COMMON_CFLAGS})
target_link_options(${TARGET_EXE_NAME} PRIVATE ${COMMON_LDFLAGS})
target_link_libraries(${TARGET_EXE_NAME} PRIVATE ${COMMON_LIBS})
set_target_properties(${TARGET_EXE_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
		OUTPUT_NAME "${PROJECT_NAME}")
