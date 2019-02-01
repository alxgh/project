gcc.exe -o ".\project.exe" ".\project.c" ".\include\helper.c" ".\include\ui.c" -std=c11 -I".\bmpio\include" -L".\bmpio\lib" -static-libgcc -lfcp

cls

.\project.exe