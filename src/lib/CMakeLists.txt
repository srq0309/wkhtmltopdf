cmake_minimum_required(VERSION 3.8.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# wkqt
include_directories(thirdparty/include)
if (WIN32)
    set(DEPLIBS_ROOT ${WKQT_PATH}/deplibs)
    set(ZLIB_LIBRARY ${DEPLIBS_ROOT})
    set(PNG_LIBRARY ${DEPLIBS_ROOT})
    set(JPEG_LIBRARY ${DEPLIBS_ROOT})
    set(QT_QMAKE_EXECUTABLE ${WKQT_PATH}/qt/bin/qmake.exe)
else()
    set(CMAKE_PREFIX_PATH ${WKQT_PATH}/qt)
endif()

find_package(Qt4 REQUIRED QtCore QtGui QtWebkit QtNetwork QtXmlpatterns QtSvg)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_definitions(-DBUILDING_WKHTMLTOX)
add_definitions(-DBUILDING_DLL)
add_definitions(-DCRT_SECURE_NO_WARNINGS)

file(GLOB WKX_SRC
    *.hh
    *.cc
    pdf.h
    lib.def
)

if(WIN32)
    set(DEPLIBS
        Qt4::QtCore
        Qt4::QtGui
        Qt4::QtNetwork
        Qt4::QtSvg
        Qt4::QtXmlPatterns
        Qt4::QtWebKit
        ${DEPLIBS_ROOT}/lib/libeay32.lib
        ${DEPLIBS_ROOT}/lib/libjpeg.lib
        ${DEPLIBS_ROOT}/lib/libpng.lib
        ${DEPLIBS_ROOT}/lib/ssleay32.lib
        ${DEPLIBS_ROOT}/lib/zlib.lib
        ws2_32.lib
        version.lib
        shlwapi.lib
        Crypt32.lib
        Imm32.lib
        Winmm.lib
    )
else()
    set(DEPLIBS
        png
        jpeg
        z
        ssl
        Xrender
        fontconfig
    )
endif()

add_library(wkhtmltox SHARED ${WKX_SRC})
target_link_libraries(wkhtmltox
    ${DEPLIBS}
)