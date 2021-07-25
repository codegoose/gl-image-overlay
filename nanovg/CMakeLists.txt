add_library(nanovg STATIC
    "src/nanovg.c"
)

target_include_directories(nanovg PUBLIC
    "include"
)