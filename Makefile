UNAME := $(shell uname)

ifeq ($(UNAME),Darwin)
INCFLAGS = -I vecmath/include/

FRAMEWORKS =  -framework Glut 
FRAMEWORKS += -framework OpenGL 
FRAMEWORKS += -framework Cocoa 
FRAMEWORKS += -framework AGL
FRAMEWORKS += -framework Carbon

LINKFLAGS += -L vecmath/lib/ -lvecmath
else
INCFLAGS = -I /usr/include/GL
INCFLAGS += -I /mit/6.837/public/include/vecmath

LINKFLAGS = -lglut -lGL -lGLU
LINKFLAGS += -L /mit/6.837/public/lib -lvecmath
endif

LINKFLAGS += -lfltk -lfltk_gl

CFLAGS    = -g
CFLAGS    += -DSOLN
CC        = g++
SRCS      = bitmap.cpp camera.cpp MatrixStack.cpp modelerapp.cpp modelerui.cpp ModelerView.cpp Joint.cpp SkeletalModel.cpp Mesh.cpp main.cpp
OBJS      = $(SRCS:.cpp=.o)
PROG      = a2

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS) $(FRAMEWORKS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)

bitmap.o: bitmap.h
camera.o: camera.h
Mesh.o: Mesh.h
MatrixStack.o: MatrixStack.h
modelerapp.o: modelerapp.h ModelerView.h modelerui.h bitmap.h camera.h
modelerui.o: modelerui.h ModelerView.h bitmap.h camera.h modelerapp.h
ModelerView.o: ModelerView.h camera.h
SkeletalModel.o: MatrixStack.h ModelerView.h Joint.h modelerapp.h

