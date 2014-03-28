PHP_ARG_ENABLE(hypx, whether to enable Hypx support,
[ --enable-hypx   Enable Hypx support])

if test "$PHP_HYPX" = "yes"; then
  AC_DEFINE(HAVE_HYPX, 1, [Whether there is Hypx])
  PHP_NEW_EXTENSION(hypx, src/hypx.c src/expert.c, $ext_shared)
fi
