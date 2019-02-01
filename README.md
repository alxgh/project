# REABISO

This was a university project which i decided to put here.

It's not written in the best way; But it's a good example of a c program with ui.

## How it works?
It makes average from datasets [in `DataSet` folder][It works for few letters]

Then it reads the given image and detect the letters.

Then it looks up in `dictionary.txt` and starts searching for words (and their reverses) horizontally and vertically.

Then it saves the result in `result` folder.

## Run command

```
gcc.exe -o "./project.exe" "./project-ui.c" "./include/helper.c" "./include/ui.c" -std=c11 -I"=bmpio/include" -L"=./bmpio/lib" -static-libgcc -lfcp
```