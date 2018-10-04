STORAGELIB = submodules/storagelib
# List of all the Userlib files
STORELIBSRC =   $(STORAGELIB)/src/lld/24FCXXX.c \
				$(STORAGELIB)/src/storage.c \
				$(STORAGELIB)/src/crc16.c

# Required include directories
STORELIBINC =   $(STORAGELIB) \
                $(STORAGELIB)/include/lld \
                $(STORAGELIB)/include
