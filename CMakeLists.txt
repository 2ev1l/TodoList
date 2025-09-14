cmake_minimum_required(VERSION 3.16)
project(TodoList VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Поиск пакетов ---
find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)

# --- 2. Группируем исходные файлы ---
set(PROJECT_SOURCES
    main.cpp
)

set(FORMS "Forms/textwindow.cpp" "Forms/textwindow.h" "Forms/textwindow.ui")
set(WIDGETS "Widgets/textbuttonedit.cpp" "Widgets/textbuttonedit.h")
set(SERIALIZATION "Serialization/dataclass.h" "Serialization/dataclass.cpp" "Serialization/jsondataserializer.h" "Serialization/jsondataserializer.cpp" "Serialization/IFileActionsHandler.h" "Serialization/textbuttoneditdata.h" "Serialization/textbuttoneditdata.cpp" "Serialization/IReadWriteable.h" "Serialization/JsonSerializer.h")
set(UI "UI/textfactory.h" "UI/textfactory.cpp" "UI/dragdropmanager.h" "UI/dragdropmanager.cpp" "UI/contextmenuactions.h" "UI/contextmenuactions.cpp" "UI/markdownhighlighter.h" "UI/markdownhighlighter.cpp" "UI/ITextDataProvider.h" "UI/IDateTimeDataProvider.h" "UI/textfilter.h" "UI/textfilter.cpp" "UI/datetimefilter.h" "UI/datetimefilter.cpp")
set(CORE "Core/objectpool.h" "Core/IPoolableObject.h" "Core/poolablewidget.h" "Core/poolablewidget.cpp" "Core/IFiltered.h" "Core/IFilter.h" "Core/filterchain.h" "Core/filterchain.cpp")
set(PROJECT_RESOURCES "Icons/Icons.qrc" "Styles/Styles.qrc")

# Собираем все исходники в один список для lupdate
set(ALL_SOURCES
    ${PROJECT_SOURCES}
    ${FORMS}
    ${WIDGETS}
    ${SERIALIZATION}
    ${UI}
    ${CORE}
)

# --- 3. Создаем исполняемый файл ---
qt_add_executable(TodoList MANUAL_FINALIZATION main.cpp)

# --- 4. Явно добавляем все остальные исходники к цели ---
target_sources(TodoList PRIVATE
    ${FORMS}
    ${WIDGETS}
    ${SERIALIZATION}
    ${UI}
    ${CORE}
    ${PROJECT_RESOURCES}
)

# --- 5. Настраиваем пути для заголовочных файлов ---
target_include_directories(TodoList PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Serialization
    ${CMAKE_CURRENT_SOURCE_DIR}/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/Forms
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
)

# --- 6. Линкуем библиотеки ---
target_link_libraries(TodoList PRIVATE Qt6::Widgets)

# --- 7. Настраиваем переводы ---
# Файлы перевода
set(TS_FILES app_ru.ts app_en.ts)

# Эта команда настроит компиляцию .ts -> .qm во время сборки
qt_add_translations(TodoList TS_FILES ${TS_FILES})

# Автоматическое копирование .qm файлов в папку translation
# Создаем кастомную цель, которая будет выполняться при каждой сборке.
# Она будет отвечать за копирование всех необходимых файлов.
add_custom_target(copy_runtime_files ALL)

# Создаем папку 'translations' в директории сборки.
# Команда выполнится только один раз, если папки еще нет.
set(TRANSLATIONS_SUBDIR "${CMAKE_CURRENT_BINARY_DIR}/translations")
add_custom_command(
    TARGET copy_runtime_files POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TRANSLATIONS_SUBDIR}
    COMMENT "Creating translations directory..."
)

# Проходимся по каждому файлу перевода
foreach(TS_FILE ${TS_FILES})
    # Определяем имя .qm файла (app_ru.ts -> app_ru.qm)
    string(REPLACE ".ts" ".qm" QM_FILE ${TS_FILE})

    set(QM_SOURCE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${QM_FILE}")
    set(QM_DESTINATION_PATH "${TRANSLATIONS_SUBDIR}/${QM_FILE}")

    # Добавляем команду копирования для каждого .qm файла
    add_custom_command(
        TARGET copy_runtime_files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${QM_SOURCE_PATH}
                ${QM_DESTINATION_PATH}
        COMMENT "Copying ${QM_FILE} to translations folder..."
    )
endforeach()

# Явная цель для вызова lupdate
add_custom_target(update_translations
    COMMAND Qt6::lupdate
        -locations none
        -no-obsolete
        ${ALL_SOURCES}
        -ts ${TS_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Updating translation source files (${TS_FILES})..."
    VERBATIM
)


# --- 8. Настройка свойств для сборки ---
set_target_properties(TodoList PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TodoList
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# --- 9. Установка и финализация ---
include(GNUInstallDirs)

# Установка всех скомпилированных .qm файлов
foreach(TS_FILE ${TS_FILES})
    # Получаем имя .qm файла из .ts файла (например, app_ru.ts -> app_ru.qm)
    string(REPLACE ".ts" ".qm" QM_FILE ${TS_FILE})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${QM_FILE}
            DESTINATION ${CMAKE_INSTALL_DATADIR}/translations
    )
endforeach()

install(TARGETS TodoList
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(TodoList)
