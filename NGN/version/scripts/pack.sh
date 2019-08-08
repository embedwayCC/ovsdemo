#!/bin/sh

while getopts "n:p:b:" Option
do
   case $Option in
   n) 
      name=$OPTARG     
        ;;
   p)
      path=$OPTARG
        ;;
    b)
      platform=$OPTARG
        ;;
    esac
done

get_version()
{
	local __base=${1:-1}
	total_id=$((`git rev-list --all|wc -l` + $__base))
}


get_version_id()
{
    get_version 0
    if [ $total_id -ge 1000 ]
    then
        minor_id=$(($total_id/1000+4))
        min_id=$(($total_id%1000))
    else
        minor_id=0
        min_id=$total_id
    fi
    version="1.$minor_id.$min_id"
}

pack_version()
{
    cd ${path}

    mkdir -p ${name}/bin/x86
    mkdir -p ${name}/bin/mips
    mkdir -p ${name}/include/
    mkdir -p ${name}/hostlib64
    mkdir -p ${name}/mipslib

    cp -rf ../include/* ${name}/include/
    if [ -f ../bin/x86/broker/broker ];then
        cp -rf ../bin/x86/broker/broker ${name}/bin/x86
    fi
    if [ -f ../bin/mips/broker/broker ];then
        cp -rf ../bin/mips/broker/broker ${name}/bin/mips
    fi
    if [ -d ../lib/x86/interface/ ];then
        cp -rf ../lib/x86/interface/* ${name}/hostlib64
    fi
    if [ -d ../lib/mips/interface/ ];then
        cp -rf ../lib/mips/interface/* ${name}/mipslib
    fi
    tar zcf ${name}_${version}.tar.gz ${name}
    rm -rf ${name}
}

get_version_id

pack_version

