#!/bin/bash

year=2021
day=$2

root_dir=$1
src_dir=$root_dir/src
day_dir=$src_dir/$day

util_dir=$root_dir/util
secret_dir=$root_dir/secret

setup_dir()
{
    if [ ! -d $day_dir ]; then
        mkdir -p $day_dir
    fi

    if [ ! -f $day_dir/$day.cpp ]; then
        cp $util_dir/template.cpp $day_dir/$day.cpp
    fi
}

pull_input()
{
    wget --load-cookies $secret_dir/cookies.txt -O $day_dir/input.in https://adventofcode.com/$year/day/$day/input
}

setup_dir
pull_input