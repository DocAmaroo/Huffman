#!/bin/bash

git add *
git status

printf 'Commit message: ' 
read commitMessage

git commit -am "$commitMessage"

git push
