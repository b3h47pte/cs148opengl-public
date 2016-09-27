import os
import sys
import shutil

def CopyDir(inputDirectory, outputDirectory):
    if inputDirectory == outputDirectory:
        return

    try:
        if os.path.exists(outputDirectory):
            shutil.rmtree(outputDirectory)
        shutil.copytree(inputDirectory, outputDirectory, ignore = shutil.ignore_patterns(("*.swp")))
        print "CopyDir Success: " + inputDirectory + " to " + outputDirectory
    except:
        print >> sys.stderr, "CopyDir Error: Could not copy from " + inputDirectory + " to " + outputDirectory
        sys.exit(1)

if  __name__ == "__main__":
    if len(sys.argv) != 3:
        print >> sys.stderr, "Dir Copy used improperly: python dirCopy.py [INPUT DIRECTORY] [OUTPUT DIRECTORY]"
    else:
        CopyDir(sys.argv[1], sys.argv[2])

