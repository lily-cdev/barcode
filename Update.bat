@echo off
rmdir Debug /s /q
mkdir Debug
git init
git add .
git config --global user.email "therealarcticsnow@gmail.com"
git config --global user.name "lily-cdev"
git commit -m "fish"
git branch -m main
git remote add origin https://github.com/lily-cdev/barcode.git
git push -u origin main --force