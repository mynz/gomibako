.PATH: ${.CURDIR}

KMOD= gomibako
SRCS= gomibako.c device_if.h bus_if.h

.include <bsd.kmod.mk>
