@echo "f:\labcaijika\cvi2013\sdk\bin\rc.exe" /d _CVI_ /i "f:\labcaijika\cvi2013\sdk\include" /fo %2 %1
@"f:\labcaijika\cvi2013\sdk\bin\rc.exe" /d _CVI_ /i "f:\labcaijika\cvi2013\sdk\include" /fo %2 %1 > Rc.out 2>&1
@if errorlevel 1 goto err
@echo RC complete
@exit 0
:err
@echo RC failed
@exit 1
