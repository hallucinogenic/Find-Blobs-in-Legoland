GCCBUILD := gcc -Wall main.c

# ============
# INSTALLATION
# ============

build:
	$(GCCBUILD) -o find-blobs

