.PHONY: all cleansub
all:
	mkpsxiso -y ./isoconfig.xml
cleansub:
	$(MAKE) clean
	rm -f kite.cue kite.bin
	
TARGET = kite
TYPE = iso

SRCS = main.c \
TIM/kitebase.tim \
TIM/stroke.tim \
TIM/vignette.tim \

include ../common.mk 
