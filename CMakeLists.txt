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

set(CMAKE_C_COMPILER gcc-13)

set(GLIBC_PATH "/opt/glibc-2.35/lib/x86_64-linux-gnu")
set(LIBGCC_PATH "/usr/lib/gcc/x86_64-linux-gnu/13")

execute_process(COMMAND ${CMAKE_C_COMPILER} -print-file-name=libasan.so
		OUTPUT_VARIABLE ASAN_SO OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${CMAKE_C_COMPILER} -print-file-name=libubsan.so
		OUTPUT_VARIABLE UBSAN_SO OUTPUT_STRIP_TRAILING_WHITESPACE)
set(LIBGCC_S_SO "${LIBGCC_PATH}/libgcc_s.so")

get_filename_component(GCC_SAN_DIR "${ASAN_SO}" DIRECTORY)
get_filename_component(GCC_UB_DIR  "${UBSAN_SO}" DIRECTORY)
get_filename_component(GCC_LIB_DIR "${LIBGCC_S_SO}" DIRECTORY)

set(COMMON_LDFLAGS
		-nodefaultlibs
		-Wl,--disable-new-dtags                # emit DT_RPATH, not RUNPATH
		# runtime loader from your alt glibc
		-Wl,-dynamic-linker=${GLIBC_PATH}/ld-linux-x86-64.so.2
		-Wl,-rpath,${GLIBC_PATH}

		# GCC sanitizer dir at link and runtime
		-L${GCC_SAN_DIR} -L${GCC_UB_DIR} -L${GCC_LIB_DIR}
		-Wl,-rpath,${GCC_SAN_DIR}
		-Wl,-rpath,${GCC_UB_DIR}
		-Wl,-rpath,${GCC_LIB_DIR}

		-L${GLIBC_PATH}
#		-lasan -lubsan -lpthread -ldl -lrt -lc -lgcc_s -lgcc
#		-static-libasan -static-libubsan
		-fsanitize=address,undefined,bounds,alignment,object-size
		-fsanitize=shift,signed-integer-overflow,null,return
		-fsanitize=float-divide-by-zero,float-cast-overflow
		-fno-sanitize-recover
		-fno-builtin-strdup

)

set(COMMON_LIBS
		asan ubsan
		gcc_s gcc
		pthread dl rt
		m
		c

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
		-fno-builtin-strdup
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
