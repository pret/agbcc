/* dwarf2out.h - Various declarations for functions found in dwarf2out.c
   Copyright (C) 1998 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

extern void dwarf2out_init 		(FILE *asm_out_file, 
						char *main_input_filename);
extern void dwarf2out_finish		(void);

extern void dwarf2out_define		(unsigned, char *);
extern void dwarf2out_undef 		(unsigned, char *);                                       
extern void dwarf2out_start_source_file 	(char *);
extern void dwarf2out_end_source_file 	(void);

extern void dwarf2out_begin_block	(unsigned);
extern void dwarf2out_end_block		(unsigned);
extern void dwarf2out_label		(rtx);			
extern void dwarf2out_decl		(tree);	
extern void dwarf2out_line		(char *, unsigned);			

extern void debug_dwarf			(void);
struct die_struct;
extern void debug_dwarf_die		(struct die_struct *);
