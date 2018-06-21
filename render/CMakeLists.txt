AUX_SOURCE_DIRECTORY(. DIR_RENDER_SRCS)
ADD_LIBRARY(Render_LIBS ${DIR_RENDER_SRCS})


ADD_LIBRARY(Render_LIBS_omp ${DIR_RENDER_SRCS})
target_compile_options(Render_LIBS_omp PUBLIC -fopenmp)
target_compile_definitions(Render_LIBS_omp PUBLIC USE_OPENMP)
