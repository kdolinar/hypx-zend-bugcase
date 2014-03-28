/**************************************************************
 *                                                            *
 *                      Hypno-Antology                        *
 *                                                            *
 *                            by                              *
 *                                                            *
 *                       Jesej d.o.o.                         *
 *                                                            *
 *                                                            *
 *   This is  a copy righted software.  Any reproduction or   *
 *   any  other  way  of usage  is  strictly  prohibited by   *
 *   European copy right law and patent marks ....            *
 *                                                            *
 *   File:        expert.h                                    *
 *                                                            *
 *   Description: the functions  for initializing of expert   *
 *   system  and  the related  functions  for inference  of   *
 *   strategy steps based on inquiry to user.                 *
 *                                                            *
 *   Authors:     Kajetan Dolinar, Janez-Tomaz Krmelj         *
 *                                                            *
 **************************************************************/

#ifndef _HYPX_EXPERT_H_
#define _HYPX_EXPERT_H_

#include "inquiry.h"

hypx_therapeutic_inquiry hypx_expert_make_inquiry(int phase);

#endif
