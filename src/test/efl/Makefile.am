if BUILD_EFL_TESTS

MEDIA = \"$(abs_top_srcdir)/media\"

INCLUDES = -I$(top_srcdir)/src

noinst_LTLIBRARIES = libwfits-efl.la

libwfits_efl_la_SOURCES = \
	application.cpp			\
	background.cpp			\
	elmtestharness.cpp		\
	evasobject.cpp			\
	window.cpp			\
	test_actionslider.cpp		\
	test_background.cpp		\
	test_bubble.cpp			\
	test_button.cpp			\
	test_check.cpp			\
	test_clock.cpp			\
	test_colorselector.cpp		\
	test_dayselector.cpp		\
	test_engine_set.cpp		\
	test_entry.cpp			\
	test_fileselector_button.cpp	\
	test_fileselector_entry.cpp	\
	test_fileselector.cpp		\
	test_flip.cpp			\
	test_flipselector.cpp		\
	test_glview.cpp			\
	test_frame.cpp			\
	test_hover.cpp			\
	test_hoversel.cpp		\
	test_icon.cpp			\
	test_image.cpp			\
	test_inwin.cpp			\
	test_list.cpp			\
	test_label.cpp			\
	test_map.cpp			\
	test_mapbuf.cpp			\
	test_notify.cpp			\
	test_panel.cpp			\
	test_popup.cpp			\
	test_progressbar.cpp		\
	test_window.cpp			\
	test_window_resize.cpp		\
	test_window_move.cpp		\
	test_window_maximize.cpp	\
	test_window_fullscreen.cpp

AM_CXXFLAGS =				\
	-DMEDIA_PATH=$(MEDIA)		\
	$(WAYLAND_CFLAGS)		\
	$(EFL_CFLAGS)			\
	$(CHECK_CFLAGS)

endif
