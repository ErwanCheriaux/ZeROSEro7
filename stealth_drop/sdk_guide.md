# Guide to install and use CC3220 SDK

### Install SDK
The following process is based on the [Quick Start Guide](http://dev.ti.com/tirex/content/simplelink_cc32xx_sdk_1_50_00_06/docs/simplelink_mcu_sdk/Quick_Start_Guide.html).  
* Install FLASH tool (UNIFLASH) from [here](http://www.ti.com/tool/uniflash).  
>If this error occur during installation: ```Failed to locate system libraries required for UniFlash operation: libudev.so.0```, you should verify libdev1 library is installed (```/lib/x86_64-linux-gnu/libudev.so.1``` file exists?).  
If this library is not installed, you can use: ```sudo apt-get install libudev1```.  
Then, link libudev0 to libudev1 ```cd /lib/x86_64-linux-gnu/ && sudo ln -sf libudev.so.1 libudev.so.0```

If you path to uniflash is ```/home/username/ti/uniflash_4.2.1```, please assume that ```<ti_install_folder>``` =  ```/home/username/ti```  
Then:
* Install SIMPLELINK-CC3220-SDK from [here](http://www.ti.com/tool/SIMPLELINK-CC3220-SDK)
* Get compilation tool-chain by downloading [gcc-arm-none-eabi-6-2017-q1-update-linux](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
* Set compilation variables:  
Into simplelink folder (```<ti_install_folder>/simplelink_cc32xx_sdk_1_50_00_06/```) you must update ```import.mak``` file. To do this, you should comment ```FREERTOS_INSTALL_DIR``` and ```CCS_ARMCOMPILER``` variables and set ```GCC_ARMCOMPILER``` and ```XDC_INSTALL_DIR``` variables:
```make
# I installed simplelink, uniflash and gcc into a common folder <ti_install_folder> but you can use a different architecture
XDC_INSTALL_DIR        ?= <ti_install_folder>/xdctools_3_50_03_33_core
GCC_ARMCOMPILER        ?= <ti_install_folder>/gcc-arm-none-eabi-6-2017-q1-update
RM     = rm -f
RMDIR  = rm -rf
DEVNULL = /dev/null
ECHOBLANKLINE = echo
```
- Add simplelink path to our project.  
Into ```<rose_folder>/stealth_drop/simplelink_path.mak``` write
```make
SIMPLELINK_CC32XX_SDK_INSTALL_DIR ?= <ti_install_folder>/simplelink_cc32xx_sdk_1_50_00_06
```
by replacing ```<ti_install_folder>``` by your own path.

### Useful links
Simplelink Quick Start Guide  
http://dev.ti.com/tirex/content/simplelink_cc32xx_sdk_1_50_00_06/docs/simplelink_mcu_sdk/Quick_Start_Guide.html

Online Simplelink Ressources  
http://dev.ti.com/tirex/#/?link=Software%2FSimpleLink%20CC3220%20SDK%2FExamples%2FDevelopment%20Tools%2FCC3220SF-LAUNCHXL  

Uniflash Quick Start Guide  
http://processors.wiki.ti.com/index.php/UniFlash_v4_Quick_Guide

### Execution
To run uniflash, you can run this command: ```~/ti/uniflash_4.2.1/node-webkit/nw &``` (if you installed uniflash into ```~/ti/```)
