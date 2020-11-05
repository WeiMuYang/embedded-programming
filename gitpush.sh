#!/bin/sh
current_time=$(date "+%Y%m%d%H%M%S")
git add .
git commit -m ${current_time}
git push  
