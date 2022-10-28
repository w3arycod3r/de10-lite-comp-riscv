
make || goto :error
cp out/bootrom.mif ../../quartus/bootrom.mif
cd ../../
call flash.bat

:error
@echo Compilation failed...