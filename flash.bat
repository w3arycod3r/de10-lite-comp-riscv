@set DESIGN_NAME=quartus/DE10_Lite_Computer

quartus_cdb %DESIGN_NAME% -c %DESIGN_NAME% --update_mif
quartus_asm --read_settings_files=on --write_settings_files=off %DESIGN_NAME% -c %DESIGN_NAME%
quartus_pgm -m jtag -o "p;%DESIGN_NAME%.sof"
@REM quartus_pgm -m jtag -o "p;%DESIGN_NAME%.pof"
nios2-terminal