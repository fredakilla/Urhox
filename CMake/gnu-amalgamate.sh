#!/bin/bash

if [ $# -ne 4 ];
then
    echo "Usage: gnu-amalgamate.sh <ar command> <target directory> <output directory> <output library name>"
    exit 1
fi

CWD=`pwd`
AR="$1"
TARGET="$2"
OUTDIR="$3"
LIBNAME="$4"

if [ ! -e $AR ];
then
    echo "No such file: $AR"
    exit 1
fi

if [ ! -d $TARGET ];
then
    echo "No such target directory: $TARGET"
    exit 1
fi

if [ ! -d $OUTDIR ];
then
    echo "No such output directory: $OUTDIR"
    exit 1
fi

if [ -z "$LIBNAME" ];
then
    echo "Empty library name: $LIBNAME"
    exit 1
fi


echo "Using ar : $AR"
echo "Amalgamating $TARGET"

cd $TARGET
rm -f $LIBNAME
mkdir -p tmp
cp *.a ./tmp/

# Build an MRI script file
MRI="$CWD/deps.mri"
echo "create $LIBNAME" >$MRI
for file in ./tmp/* ; do
    if [ -e "$file" ];
    then
        echo "addlib $file" >>$MRI
    fi
done
echo "save" >>$MRI
echo "end" >>$MRI

echo "Executing..."
cat $MRI

# Now actually do something
$AR -M < $MRI

# move amalgamated lib to output dir
#mv -f $LIBNAME $OUTDIR

# Clean up
rm $MRI
rm -rf ./tmp


cd $CWD
