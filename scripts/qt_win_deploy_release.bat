SET release_name=sulatester.exe
SET release_path=..\..\build-sulatester-Desktop_Qt_5_9_4_MinGW_32bit-Release\release
SET deploy_path=%release_path%\..\deploy


rmdir /s/q %deploy_path%
mkdir %deploy_path%
::del /s/q/f %deploy_path%\*
copy %release_path%\%release_name% %deploy_path%
windeployqt %deploy_path%\%release_name%
pause
