function(pixelforge_enable_warnings target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE
                /W4
                /permissive-
        )

        if(PIXELFORGE_WARNINGS_AS_ERRORS)
            target_compile_options(${target_name} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target_name} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wshadow
                -Wnon-virtual-dtor
                -Wconversion
                -Wsign-conversion
        )

        if(PIXELFORGE_WARNINGS_AS_ERRORS)
            target_compile_options(${target_name} PRIVATE -Werror)
        endif()
    endif()
endfunction()