## C- To Java compiler
repo: https://github.com/jubeatWwW/NCTU_COMPILER_2016

* `testdata` is for some test file
* `input` is for the input file of testdata which is named by `[testdata].in`

### To run the compiler
* For quick execution `bash ./jcompiler -f filename.c -r -i inputfile.in -d`
* Or move into `compiler` folder, `make` `./compiler path/file`
    ,and move to `jasmin` folder
    `java -jar jasmin.jar ../compiler/output.j` `java output`
* For example `./jcompiler -f testdata/assignment.c -r -i input/assignment.in`

### Script
* `-f Necessary. File to be compiled`
* `-r Optional. Run file after compiled`
* `-i Optional. Input file`
* `-d Optional. Clean dependency after execution`
