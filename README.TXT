How to reproduce the fault: 

> phpize
> ./configure --enable-hypx
> make
> make install
> valgrind --tool=memcheck --read-var-info=yes --trace-children=yes --leak-check=no --show-possibly-lost=no --undef-value-errors=yes --track-origins=yes php index.php
==26270== Memcheck, a memory error detector
==26270== Copyright (C) 2002-2011, and GNU GPL'd, by Julian Seward et al.
==26270== Using Valgrind-3.7.0 and LibVEX; rerun with -h for copyright info
==26270== Command: php index.php
==26270==
 . . .
==26270== Invalid read of size 4
==26270==    at 0x44EAA4: zend_std_get_method (zend_object_handlers.c:847)
==26270==    by 0x47A035: ZEND_INIT_METHOD_CALL_SPEC_VAR_CONST_HANDLER (zend_vm_execute.h:10461)
==26270==    by 0x452D1D: execute (zend_vm_execute.h:107)
==26270==    by 0x42C398: zend_execute_scripts (zend.c:1236)
==26270==    by 0x1: ???
==26270==  Address 0xffffffff is not stack'd, malloc'd or (recently) free'd
 . . .
Segmentation fault
