#!/bin/bash

hex2bin(){
  bin=$(echo "obase=2; ibase=16;$1" | bc )
  printf "%s" $bin
}


regbinmap(){
  bin=$(hex2bin $(/root/memtool -32 $1 1 | grep ":" | sed -e 's/^[^:]*:[ ]*//')) 
  
  count=$(printf "%s" $bin | wc -c)
  prepend=$(( 32 - $count ))
  printf "0%.0s" $(seq 1 1 $prepend)
  printf "%s" $bin
}

dumpreg(){
  printf "\n${1}:\n"
  for i in $(seq 31 -1 0); do 
    printf "%3d" $i
  done 
  echo 
  regbinmap $1 | sed -e 's/\([01]\)/  \1/g'
  printf "\n\n" 
}

# EIM_CS0 / GPIO2 IO23 : gpio 55
# ./memtool -32 20e013c 1
# ./memtool -32 209c004 1
# ./memtool -32 20a0004 1
# gpio 1 dir
dumpreg 209c004
# gpio 1 data
dumpreg 209c000
# gpio 2 dir
dumpreg 20a0004
# gpio 2 data
dumpreg 20a0000

