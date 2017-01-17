## Run the compiler

* `testdata` is for some test file
* `input` is for the input file of testdata which is named by `[testdata].in`

### To run the compiler
* For quick execution `./jcompiler -f filename.c -r -i inputfile.in -d`
* Or move into `compiler` folder, `make` `./compiler path/file`
    ,and move to `jasmin` folder
    `java -jar jasmin.jar ../compiler/output.j` `java output`

### Script
* `-f Necessary. File to be compiled`
* `-r Optional. Run file after compiled`
* `-i Optional. Input file`
* `-d Optional. Clean dependency after execution`
