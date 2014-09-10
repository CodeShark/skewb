include os.mk

skewb: build/skewb$(EXE_EXT)

build/skewb$(EXE_EXT): src/skewb.cpp
	$(CXX) $(CXX_FLAGS) $< -o $@
