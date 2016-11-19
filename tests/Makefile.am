AM_CPPFLAGS = $(WARN_CFLAGS) \
              $(DISABLE_DEPRECATED_CFLAGS) \
              $(ZAKFORMCGI_CFLAGS) \
              -I$(top_srcdir)/src

LIBS = $(ZAKFORMCGI_LIBS) \
       -export-dynamic

LDADD = $(top_builddir)/src/libzakformcgi.la

noinst_PROGRAMS = \
                    form \
                    form_xml

EXTRA_DIST = \
             form_xml.xml
