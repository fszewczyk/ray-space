set(STB_IMAGE_SOURCE
	src/stb_image.h 
	src/stb_image_write.h
)

add_library(stb_image STATIC 
	${STB_IMAGE_SOURCE}
)

set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE C)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)