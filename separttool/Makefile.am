AM_CFLAGS = -I$(top_srcdir)/include $(GLOBAL_CFLAGS) $(libplist_CFLAGS) $(openssl_CFLAGS) $(libgeneral_CFLAGS)
AM_CXXFLAGS = $(AM_CFLAGS) $(GLOBAL_CXXFLAGS)
AM_LDFLAGS = $(libplist_LIBS) $(openssl_LIBS) $(libgeneral_LIBS)

if ENABLE_SEPARTTOOL

bin_PROGRAMS = separttool

separttool_CFLAGS = $(AM_CFLAGS) $(libfwkeyfetch_CFLAGS)
separttool_CXXFLAGS = $(AM_CXXFLAGS)
separttool_LDFLAGS = $(AM_LDFLAGS) $(libfwkeyfetch_LIBS)
separttool_LDADD = ../img4tool/libimg4tool.la
separttool_SOURCES = 	main.cpp

endif #ENABLE_SEPARTTOOL