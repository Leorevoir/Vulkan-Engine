CC = glslangValidator
CACHE_DIR = cache

SHADERS = $(wildcard *.vert *.frag *.comp *.geom *.tesc *.tese)
SPV_FILES = $(patsubst %, $(CACHE_DIR)/%.spv, $(SHADERS))

all: $(SPV_FILES)

$(CACHE_DIR)/%.spv: %
	mkdir -p $(CACHE_DIR)
	$(CC) -V $< -o $@

.PHONY: all
