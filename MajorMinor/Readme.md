1. ins .ko of major_minor.c i.e. major_minor.ko (dynamically)
2. use commnad lsmod | head to check the module
3. ins .ko of major_minor_dev.c i.e. major_minor_dev.ko (statically)
	Here after Generating the major number from major_minor.ko
	Insert the Major Number in major_minor_dev.c macro
4. use commnad lsmod | head to check the module
5. Enter ls -al /dev to check the two different devices
