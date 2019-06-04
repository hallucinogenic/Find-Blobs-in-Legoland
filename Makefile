GCCBUILD := gcc -Wall main.c

MKDIRBIN := mkdir ../growler/bin/
# ============
# INSTALLATION
# ============

build:
	$(MKDIRBIN)
	$(GCCBUILD) -o ../growler/bin/find-blobs

