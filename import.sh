#!/bin/sh

path="data/$1_list"

ls data/init/$1 | while read fname
do
    echo ${fname%%.csv} >> ${path}
done

