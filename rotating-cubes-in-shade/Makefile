# This Makefile assumes that you have the Urho3D library extracted in the folder
#     /home/valera/projects/Urho3D-1.5
#
# and that you have built it in the folder
#     /home/valera/projects/Urho3D-1.5/build
#
# with the following cmake command:
#     cmake -DURHO3D_SAMPLES=1 -DURHO3D_LIB_TYPE=SHARED ../
#
# If you have a different folder structure, please modify the below path variables
# according to your setup.

URHO_DIR=/home/valera/projects/Urho3D-1.5
URHO3D_INCLUDE_DIR=$(URHO_DIR)/build/include
3RD_PARTY_INCLUDE_DIR=$(URHO_DIR)/build/include/Urho3D/ThirdParty
LIB_DIR=$(URHO_DIR)/build/lib

all:

build:
	g++ -o sampleApp sampleApp.cpp -O3 -I$(URHO3D_INCLUDE_DIR) -I$(3RD_PARTY_INCLUDE_DIR) -L$(LIB_DIR) -lUrho3D

clean:
	rm -rf sampleApp
