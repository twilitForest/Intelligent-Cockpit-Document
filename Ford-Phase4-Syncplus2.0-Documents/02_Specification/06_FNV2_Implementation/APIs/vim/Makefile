include $(FORD_COMMON_BUILD)/config.mk

ARTIFACT = libvimidl.so

SRCS = $(call findsrc, ., cc)

include $(FORD_COMMON_BUILD)/idl_library_common.mk

PYTHON_EXPORTED_DIR := $(ARTIFACT_DIR)/python_out
PYTHON_PUBLISH_LOCATION := $(SYSTEM_DIR)/python/idl
PUBLISH_PYTHON_LIST := $(addprefix $(PYTHON_PUBLISH_LOCATION)/,$(notdir $(wildcard $(PYTHON_EXPORTED_DIR)/*)))

$(PUBLISH_PYTHON_LIST): | $(PYTHON_PUBLISH_LOCATION)

$(PYTHON_PUBLISH_LOCATION) :
	mkdir -p $@

# rule for copying proto public python modules
$(PYTHON_PUBLISH_LOCATION)/%.py: $(PYTHON_EXPORTED_DIR)/%.py
	@echo Publish python script  $@
	@cp -p $< $@

$(HINSTALL_REC) : $(PUBLISH_PYTHON_LIST)

clean : 
	rm -f $(PYTHON_PUBLISH_LOCATION)/*.py

