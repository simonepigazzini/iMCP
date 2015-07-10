#!/bin/bash

if [ ${1} = "all" ]; then
    names=$(ls cpp | tr " " "\n")
    
    for name_tmp in $names 
    do
	if [ ${name_tmp: -1} != "~" ] && [ ${name_tmp: -1} != "#" ]; then
	    echo compiling $name_tmp
	    g++ -O2 -lm -o ${name_tmp%".cpp"} "cpp/"$name_tmp `root-config --cflags --glibs`  
	else
	    echo skipped $name_tmp
	fi
    done

else
    exe_name=${1%".cpp"}
    exe_name=${exe_name#"cpp/"}
    g++ -O2 -lm -o $exe_name ${1} `root-config --cflags --glibs`  
fi
