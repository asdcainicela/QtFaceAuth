@echo off
python "manage.py" %*
if %ERRORLEVEL% NEQ 0 (
    echo Error executing command.
    exit /b %ERRORLEVEL%
)
