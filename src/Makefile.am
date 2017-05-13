if PLATFORM_WIN32
libmodulesext = -0.dll
libmodulesdir = $(bindir)
else
libmodulesext = .so
libmodulesdir = $(libdir)
endif

libzakformmodulesdir = `pkg-config --variable=modulesdir libzakform`

LIBS = $(ZAKFORMCGI_LIBS)

AM_CPPFLAGS = $(ZAKFORMCGI_CFLAGS) \
              -DG_LOG_DOMAIN=\"ZakFormCGI\"

lib_LTLIBRARIES = libzakformcgi.la

libzakformcgi_la_SOURCES = commons.c \
                       form.c \
                       formelement.c \
                       formelementcheck.c \
                       formelementemail.c \
                       formelementfile.c \
                       formelementhidden.c \
                       formelementpassword.c \
                       formelementradio.c \
                       formelementselect.c \
                       formelementstring.c \
                       formelementtext.c \
                       formelementtextarea.c \
                       formelementsubmit.c

libzakformcgi_la_LDFLAGS = -no-undefined

libzakformcgi_include_HEADERS = \
                            libzakformcgi.h \
                            commons.h \
                            form.h \
                            formelement.h \
                            formelementcheck.h \
                            formelementemail.h \
                            formelementfile.h \
                            formelementhidden.h \
                            formelementpassword.h \
                            formelementradio.h \
                            formelementselect.h \
                            formelementstring.h \
                            formelementtext.h \
                            formelementtextarea.h \
                            formelementsubmit.h

libzakformcgi_includedir = $(includedir)/libzakformcgi

install-exec-hook:
	cd $(libzakformmodulesdir) && \
	ln -s -f $(libmodulesdir)/libzakformcgi$(libmodulesext) .

uninstall-hook:
	rm -f $(libzakformmodulesdir)/libzakformcgi$(libmodulesext)
