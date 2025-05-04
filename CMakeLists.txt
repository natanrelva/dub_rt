cmake_minimum_required(VERSION 3.10)
project(AudioManipulationLayer)

set(CMAKE_CXX_STANDARD 17)

# Define caminho do Homebrew no Mac M2
set(CMAKE_PREFIX_PATH "/opt/homebrew;${CMAKE_PREFIX_PATH}")

# Incluir bibliotecas externas via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SOUNDTOUCH REQUIRED soundtouch)
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)
pkg_check_modules(OPUS REQUIRED opus)

# Incluir diretórios de cabeçalhos
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${PORTAUDIO_INCLUDE_DIRS}
    ${OPUS_INCLUDE_DIRS}
    ${SOUNDTOUCH_INCLUDE_DIRS}
    /opt/homebrew/include
)

# Incluir diretório da biblioteca
link_directories(/opt/homebrew/lib)

# Adiciona o GoogleTest como subdiretório (já clonado via git submodule)
add_subdirectory(extern/googletest)

# Debug: imprimir caminhos das libs
message("SOUNDTOUCH_INCLUDE_DIRS: ${SOUNDTOUCH_INCLUDE_DIRS}")
message("PORTAUDIO_INCLUDE_DIRS: ${PORTAUDIO_INCLUDE_DIRS}")
message("OPUS_INCLUDE_DIRS: ${OPUS_INCLUDE_DIRS}")
message("SOUNDTOUCH_LIBRARIES: ${SOUNDTOUCH_LIBRARIES}")
message("PORTAUDIO_LIBRARIES: ${PORTAUDIO_LIBRARIES}")
message("OPUS_LIBRARIES: ${OPUS_LIBRARIES}")

# Biblioteca principal
add_library(audio_manipulation
    src/ipc_manager.cpp
    src/scheduler.cpp
    src/buffer_manager.cpp
    src/speed_adjuster.cpp
    src/decoder.cpp
    src/encoder.cpp
    src/capture_module.cpp
    src/output_module.cpp
)

# Executável de teste
add_executable(test_audio_manipulation
    tests/test_decoder.cpp
    tests/test_encoder.cpp
    tests/test_capture_module.cpp
    tests/test_output_module.cpp
    tests/test_integration_level2.cpp
)

# Linkar com bibliotecas
target_link_libraries(test_audio_manipulation
    audio_manipulation
    ${PORTAUDIO_LIBRARIES}
    ${OPUS_LIBRARIES}
    ${SOUNDTOUCH_LIBRARIES}
    gtest
    gtest_main
    pthread
)

# Ativar testes
enable_testing()
add_test(NAME AudioManipulationTests COMMAND test_audio_manipulation)
