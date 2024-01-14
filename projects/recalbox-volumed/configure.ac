AC_INIT([volumed], [0.2], [davidb@230ruedubac.fr])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CHECK_HEADERS([string.h syslog.h stdio.h stdint.h stdbool.h fcntl.h math.h unistd.h])
AC_CHECK_LIB(pulse, pa_mainloop_new)
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

