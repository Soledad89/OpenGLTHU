MAKE = make
CC = gcc
LN = ln -s
RM = rm -fv
CFLAGS += -c -O2 -Wall
CXXFLAGS = -arch i386 -arch x86_64
LDFLAGS += -framework OpenGL -framework GLUT -framework CoreFoundation

TARGET = week4
CFILES = week4.cpp
OFILES = $(CFILES:.c=.o)

INCLUDEDIR = /usr/local/include 
LIBDIR = /usr/local/lib

#INCLUDESOIL = -I/Users/wangbo1/Documents/编译程序/SOIL/include
LIBSOIL = -L/Users/wangbo1/Documents/编译程序/SOIL/lib
LIBS = -lSOIL
all: $(TARGET) $(OFILES) 
	
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDESOIL) $(CXXFLAGS) $< -o $@

#%.o: %.c
#	$(CC) $(CFLAGS) $(CXXFLAGS) -c  $^ -I $(INCLUDESOIL) 

$(TARGET): $(OFILES)
	#$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) 
	$(CC) $(OFILES) $(LDFLAGS) $(LIBSOIL) $(LIBS) -o $@


install:
	$(INSTALL_DIR) $(DESTDIR)/$(INCLUDEDIR)
	$(INSTALL_FILE) $(HFILES) $(DESTDIR)/$(INCLUDEDIR)
	$(INSTALL_DIR) $(DESTDIR)/$(LIBDIR)
	$(INSTALL_FILE) $(AFILE) $(DESTDIR)/$(LIBDIR)
	$(INSTALL_FILE) $(SOFILE) $(DESTDIR)/$(LIBDIR)
	( cd $(DESTDIR)/$(LIBDIR) && $(LN) $(SOFILE) $(LIBNAME).so.$(MAJOR) \
	  && $(LN) $(SOFILE) $(LIBNAME).so )

clean:
	$(RM) *.o
	$(RM) *~

distclean:
	$(RM) $(AFILE) $(SOFILE)

.PHONY: all lib clean distclean
