URHO_DIR=/home/valera/projects/Urho3D-1.5
URHO3D_INCLUDE_DIR=$(URHO_DIR)/build/include
3RD_PARTY_INCLUDE_DIR=$(URHO_DIR)/build/include/Urho3D/ThirdParty
LIB_DIR=$(URHO_DIR)/build/lib

all:

build:
	g++ -o sampleApp sampleApp.cpp -O3 -I$(URHO3D_INCLUDE_DIR) -I$(3RD_PARTY_INCLUDE_DIR) -L$(LIB_DIR) -lUrho3D

clean:
	rm -rf sampleApp
