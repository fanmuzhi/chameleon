# -*- mode: makefile -*-
#
# Top level makefile for the RAM diagnostic test patches.
#
# Bjoren Davis, March 5, 2018.
#
PATCHES=dcd dct dev icd ict sys
PRODUCTS=denali hayes shasta sc24
TARGETS=rom fpga
ALLPxT=$(foreach prod,$(PRODUCTS),$(foreach targ,$(TARGETS),$(prod)-$(targ)))
ALLPATxPxT=$(foreach pat,$(PATCHES),$(foreach pxt,$(ALLPxT),$(pat)-$(pxt)))

.PHONY: default clean $(ALLPxT) all $(ALLPATxPxT) FORCE

default: all

FORCE:

clean:
	rm -rf $(foreach pat,$(PATCHES),$(pat)/obj)


define PATxPxT_template
$(1)-$(2): FORCE
	$(MAKE) -C $(1) $(2)

$(2): $(1)-$(2)

endef

$(eval $(foreach pat,$(PATCHES),$(foreach pxt,$(ALLPxT),\
	    $(call PATxPxT_template,$(pat),$(pxt)))))

all: $(ALLPATxPxT)
