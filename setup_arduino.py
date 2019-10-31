#!/usr/bin/env python3

import sys
import os
import subprocess

SCRIPT_DIR=os.path.dirname(__file__);   # We want the relative path.

def linkFile(filename, toFilename=None):
    """
        Generates a link file to the given filename.
    """
    if(toFilename == None):
        toFilename = filename;
    toFilename = os.path.basename(toFilename);
    filename = os.path.join(os.path.basename(os.path.abspath(SCRIPT_DIR)), filename);    # Have to add the project name in front in order for this to work.

    print("Creating file '%s' linking to '%s'."%(toFilename, filename));

    shellCmd = ["ln", "-sf", filename];
    shellCmd.append(os.path.join(SCRIPT_DIR, "..", toFilename));
    subprocess.run(shellCmd);

# Copy single include headers.
linkFile(os.path.join(SCRIPT_DIR, "Arduino_alib-cpp.h"));
linkFile(os.path.join(SCRIPT_DIR, "Arduino_alib-cpp-comm.h"));
linkFile(os.path.join(SCRIPT_DIR, "includes"));

# Copy source.
baseDir = os.path.join(SCRIPT_DIR, "source");
for rootDir, dirs, files in os.walk(baseDir):
    fileHeader = rootDir.split(baseDir)[-1];
    if(len(fileHeader) > 0):
        fileHeader = "_".join(fileHeader.split('/')[1:]);
        fileHeader += "_";

    for f in files:
        linkFile(os.path.join(rootDir, f), fileHeader + f);
