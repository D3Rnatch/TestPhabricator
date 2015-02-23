#!/bin/sh

setxkbmap fr

echo ""
echo "*********************************"
echo "** INSTALL ALL NEEDED PACKAGES **"
echo "*********************************"
echo ""

sudo apt update
sudo apt install vim \
git \
python-opencv python-pygame \
doxygen doxygen-doc

echo ""
echo "********************"
echo "** CONFIGURE GIT  **"
echo "********************"
echo ""

git config --global color.diff auto
git config --global color.status auto
git config --global color.branch auto
git config --global push.default simple

echo ""
echo "********************"
echo "** CONFIGURE VIM  **"
echo "********************"
echo ""

echo "set background=dark" >> ~/.vimrc
echo "syntax on" >> ~/.vimrc
echo "set number" >> ~/.vimrc
echo "set autoindent" >> ~/.vimrc
echo "set shiftwidth=4" >> ~/.vimrc

echo ""
echo "***************"
echo "** CLONE PFE **"
echo "***************"
echo ""

mkdir ~/PFE
cd ~/PFE
git clone http://github.com/osblouf/Hovercraft_PFE

