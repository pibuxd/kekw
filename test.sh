#!/bin/bash
# testing script

# color variables
green=$(tput setaf 72);
red=$(tput setaf 1);
blue=$(tput setaf 32);
magenta=$(tput setaf 5);
bold=$(tput bold);
reset=$(tput sgr0);

make
printf "\n"

for file in test/in/*; do
  filename=${file##*/}
  filename=${filename%.*}

  ./kekw.out $file > tocheck.out

  if diff -b tocheck.out test/out/${filename}.out > /dev/null; then
    printf "test: ${blue}${filename} ${green}OK${reset}\n"
  else
    printf "test: ${blue}${filename} ${red}ERROR${reset}\n"
  fi
done

printf "\n"
make clean