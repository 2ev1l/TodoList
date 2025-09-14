@echo off
setlocal

REM --- Настройка ---
REM Укажите здесь имя вашей папки для сборки.
set BUILD_DIR=build\Desktop_Qt_6_8_3_MinGW_64_bit-Debug
REM -----------------

REM Проверяем, существует ли папка сборки
if not exist "%BUILD_DIR%" (
    echo Build directory '%BUILD_DIR%' not found.
    echo Please make sure the project is configured in Qt Creator first.
    pause
    goto :eof
)

echo Updating translations by calling CMake directly...
cmake --build "%BUILD_DIR%" --target update_translations

if %errorlevel% neq 0 (
    echo Failed to update translations.
    pause
    exit /b 1
)

echo.
echo Translation source file (app_ru.ts) has been updated successfully!
echo You can now open it in Qt Linguist.
pause