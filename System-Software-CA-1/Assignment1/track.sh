#!/bin/bash
inotifywait -m -r -q --exclude '/\.|~|4913' /home/aayush/Desktop/System-Software-CA-1/Assignment1/shareDirectory -e create -e modify -e delete  |
    while read path action file; do
        username=$(stat --format %U $path 2>/dev/null)
        date=$(date +"%Y-%m-%d %H:%M:%S")
        echo "'$file' @ '$path' : '$action' by '$username' @ '${date%.*}'" >> /home/aayush/Desktop/System-Software-CA-1/Assignment1/filechanges.log
    done


