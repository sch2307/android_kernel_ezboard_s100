#!/bin/sh

if [ -f .config ]; then
	make mrproper
fi

PWD=$(pwd)
curdir="$(basename $PWD)"
build_path="../build_$curdir"
module_install="../../out"
image_filename="$build_path/arch/arm/boot/Image"
target_filename="/tftpboot/zImage.ezs5pv210-s100-android"

# tool chain check
#toolchain_path="$SDK_DIR/toolchain/CodeSourcery"
toolchain_path="/home/choijuone/arm-2009q3"
cross_compile="$toolchain_path/bin/arm-none-linux-gnueabi-"


# build .config
if [ ! -d $build_path ]; then
	mkdir $build_path
	chmod 777 $build_path
	CROSS_COMPILE=$cross_compile ARCH=arm make O=$build_path ezs5pv210_defconfig
fi


# build kernel image
if [ "$1" = "" ] ; then
	CROSS_COMPILE=$cross_compile ARCH=arm make O=$build_path zImage -j 4
	#CROSS_COMPILE=$cross_compile ARCH=arm make O=$build_path 
else
	CROSS_COMPILE=$cross_compile ARCH=arm make O=$build_path $1 $2 $3
fi


# build kernel modules
if [ "$1" = "modules" ] ; then
	CROSS_COMPILE=$cross_compile ARCH=arm INSTALL_MOD_PATH=$module_install make O=$build_path modules
	CROSS_COMPILE=$cross_compile ARCH=arm INSTALL_MOD_PATH=$module_install make O=$build_path modules_install
fi


# kernel image copy to out
if [ -f $image_filename ]; then
   echo "copy from $image_filename to $target_filename"
   chmod 777 $image_filename
   cp -a $image_filename $target_filename
fi
