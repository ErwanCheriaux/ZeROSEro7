# Use clang to improve coding style

### 1st step: Install clang

Go to this page: http://releases.llvm.org/download.html  
Download newest clang version for your OS (I recommend Pre-Built Binaries)
Install it:
* copy the content of the zip's main directory into ```/usr/```
* ```bin``` folder must be merged in ```bin/```, ```include``` into ```include/```, ...

### 2nd step: Configure coding style  

Create a ```.clang-format``` file. It will define all coding style rules.  
Examples and details could be found [here](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)  
***This file must be in on of the parent directory of the sources or at the same directory of the command execution.***

### 3st step: Use clang-format

Just run
```bash
clang-format <filename.c>
```
to see corrected file into terminal

### 4th step: Use in a project

To use clang-format, you can add to your ```Makefile```
```make
format: $(SRCS:%.c=%.format)
%.format: %.c
    clang-format $^ > $@
    mv $@ $^
```
and  ```make format``` like
```make
$(OUT): $(OBJS)
    make format
    $(CC) $(CFLAGS) $(DB) -o $@ $^ $(LDLIBS)
```
