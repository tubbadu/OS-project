#!/usr/bin/python

import os

BASEDIR = "/mnt"
# QEMUDIR = "qemu"
FILESDIR = "files"

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
	print(f"mv {os.path.join(BASEDIR, FILESDIR, file)} {os.path.join(BASEDIR, file)} ")
	os.replace(os.path.join(BASEDIR, FILESDIR, file), os.path.join(BASEDIR, file))