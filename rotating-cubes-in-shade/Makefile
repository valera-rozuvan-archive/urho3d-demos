# This Makefile assumes that you have the Urho3D library extracted in the folder:
#     /home/valera/projects/Urho3D-1.5
#
# and that you have built it in the folder:
#     /home/valera/projects/Urho3D-1.5/build
#
# with the following command:
#     cmake -DURHO3D_SAMPLES=1 -DURHO3D_LIB_TYPE=SHARED ../ && make
#
# If you have a different folder structure, please modify the below path variables
# according to your setup.
#
#
#
# Also, make sure that the library libUrho3D.so is discoverable at run-time. To do this,
# first check the folder:
#     /home/valera/projects/Urho3D-1.5/build/lib
#
# and see that *.so files are there. Then as user root, create the file:
#     /etc/ld.so.conf.d/urho3d.conf
#
# and add to it the following line:
#     /home/valera/projects/Urho3D-1.5/build/lib
#
# After saving this file, run the command as root user:
#     ldconfig
#
#
#
# NOTE: This Makefile was successfully tested on Ubuntu 15.10 (Wily Werewolf).

URHO_DIR=/home/valera/projects/Urho3D-1.5
URHO3D_INCLUDE_DIR=$(URHO_DIR)/build/include
3RD_PARTY_INCLUDE_DIR=$(URHO_DIR)/build/include/Urho3D/ThirdParty
LIB_DIR=$(URHO_DIR)/build/lib

all:

build_linux:
	g++ -o sampleApp sampleApp.cpp -O3 -I$(URHO3D_INCLUDE_DIR) -I$(3RD_PARTY_INCLUDE_DIR) -L$(LIB_DIR) -lUrho3D

clean:
	rm -rf sampleApp
