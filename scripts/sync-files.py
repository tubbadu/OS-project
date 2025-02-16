#!/usr/bin/python

import shutil
import os

BASEDIR = os.getcwd()
# QEMUDIR = "qemu"
FILESDIR = "src"

def getFiles(directory):
	# Get the absolute path of the given directory
	abs_directory = os.path.abspath(directory)
	allfiles = []
	# Walk through the directory
	for root, dirs, files in os.walk(abs_directory):
		for file in files:
			# Get the relative path of the file
			relfile = os.path.relpath(os.path.join(root, file), abs_directory)
			allfiles.append(relfile)
	return allfiles

files = getFiles(os.path.join(BASEDIR, FILESDIR))

for file in files:
	print(f"cp {os.path.join(BASEDIR, FILESDIR, file)} {os.path.join(BASEDIR, file)} ")
	shutil.copyfile(os.path.join(BASEDIR, FILESDIR, file), os.path.join(BASEDIR, file))