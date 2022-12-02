
qt5_add_resources(
    ui_style
    ${CMAKE_CURRENT_SOURCE_DIR}/app.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BrixActivityWatch
        ${ui_style}
        MANUAL_FINALIZATION
    )

# Define target properties for Android with Qt 6 as:
# set_property(TARGET BrixActivityWatch APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(BrixActivityWatch SHARED
            ${ui_style}
        )

    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable( BrixActivityWatch
            ${ui_style}
        )
    endif()
endif()

set_target_properties(BrixActivityWatch PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BrixActivityWatch)
endif()


target_include_directories(
    BrixActivityWatch PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)


target_sources(
    BrixActivityWatch PRIVATE
    main.cpp)

if(WIN32)
    if(MSVC)
        target_sources(
            BrixActivityWatch PRIVATE
            BrixActivityWatch.rc
        )
    endif()
endif(WIN32)

target_include_directories(
    BrixActivityWatch PUBLIC
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/>
)

target_link_libraries(
    BrixActivityWatch PRIVATE
    project_warnings
    project_options
    ${DESIGNER_LIBRARIES}
    ${DESIGNER_EXTERNAL_LIBRARIES}
)

if(UNIX)
    target_link_libraries(
        BrixActivityWatch PRIVATE
        ${CMAKE_DL_LIBS}
    )
endif(UNIX)


target_compile_features(BrixActivityWatch PRIVATE cxx_std_17)

target_link_libraries(BrixActivityWatch PRIVATE ${QT_LIBRARIES})

add_subdirectory(httpClient)
add_subdirectory(utils)
add_subdirectory(view)
add_subdirectory(watcher)


