/* ELF support for BFD.
   Copyright (C) 1991-2018 Free Software Foundation, Inc.

   Written by Fred Fish @ Cygnus Support, from information published
   in "UNIX System V Release 4, Programmers Guide: ANSI C and
   Programming Support Tools".

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

/* This file is part of ELF support for BFD, and contains the portions
   that are common to both the internal and external representations.
   For example, ELFMAG0 is the byte 0x7F in both the internal (in-memory)
   and external (in-file) representations.  */

#ifndef _ELF_COMMON_H
#define _ELF_COMMON_H

/* Fields in e_ident[].  */

#define EI_MAG0		0	/* File identification byte 0 index */
#define ELFMAG0		   0x7F	/* Magic number byte 0 */

#define EI_MAG1		1	/* File identification byte 1 index */
#define ELFMAG1		    'E'	/* Magic number byte 1 */

#define EI_MAG2		2	/* File identification byte 2 index */
#define ELFMAG2		    'L'	/* Magic number byte 2 */

#define EI_MAG3		3	/* File identification byte 3 index */
#define ELFMAG3		    'F'	/* Magic number byte 3 */

#define EI_CLASS	4	/* File class */
#define ELFCLASSNONE	      0	/* Invalid class */
#define ELFCLASS32	      1	/* 32-bit objects */
#define ELFCLASS64	      2	/* 64-bit objects */

#define EI_DATA		5	/* Data encoding */
#define ELFDATANONE	      0	/* Invalid data encoding */
#define ELFDATA2LSB	      1	/* 2's complement, little endian */
#define ELFDATA2MSB	      2	/* 2's complement, big endian */

#define EI_VERSION	6	/* File version */

#define EI_OSABI	7	/* Operating System/ABI indication */
#define ELFOSABI_NONE	      0	/* UNIX System V ABI */
#define ELFOSABI_HPUX	      1	/* HP-UX operating system */
#define ELFOSABI_NETBSD	      2	/* NetBSD */
#define ELFOSABI_GNU	      3	/* GNU */
#define ELFOSABI_LINUX	      3	/* Alias for ELFOSABI_GNU */
#define ELFOSABI_SOLARIS      6	/* Solaris */
#define ELFOSABI_AIX	      7	/* AIX */
#define ELFOSABI_IRIX	      8	/* IRIX */
#define ELFOSABI_FREEBSD      9	/* FreeBSD */
#define ELFOSABI_TRU64	     10	/* TRU64 UNIX */
#define ELFOSABI_MODESTO     11	/* Novell Modesto */
#define ELFOSABI_OPENBSD     12	/* OpenBSD */
#define ELFOSABI_OPENVMS     13	/* OpenVMS */
#define ELFOSABI_NSK	     14	/* Hewlett-Packard Non-Stop Kernel */
#define ELFOSABI_AROS	     15	/* AROS */
#define ELFOSABI_FENIXOS     16 /* FenixOS */
#define ELFOSABI_CLOUDABI    17 /* Nuxi CloudABI */
#define ELFOSABI_OPENVOS     18 /* Stratus Technologies OpenVOS */

#define ELFOSABI_C6000_ELFABI 64 /* Bare-metal TMS320C6000 */
#define ELFOSABI_C6000_LINUX 65 /* Linux TMS320C6000 */
#define ELFOSABI_ARM_FDPIC   65 /* ARM FDPIC */
#define ELFOSABI_ARM	     97	/* ARM */
#define ELFOSABI_STANDALONE 255	/* Standalone (embedded) application */

#define EI_ABIVERSION	8	/* ABI version */

#define EI_PAD		9	/* Start of padding bytes */


/* Values for e_type, which identifies the object file type.  */

#define ET_NONE		0	/* No file type */
#define ET_REL		1	/* Relocatable file */
#define ET_EXEC		2	/* Executable file */
#define ET_DYN		3	/* Shared object file */
#define ET_CORE		4	/* Core file */
#define ET_LOOS		0xFE00	/* Operating system-specific */
#define ET_HIOS		0xFEFF	/* Operating system-specific */
#define ET_LOPROC	0xFF00	/* Processor-specific */
#define ET_HIPROC	0xFFFF	/* Processor-specific */

/* Values for e_machine, which identifies the architecture.  These numbers
   are officially assigned by registry@sco.com.  See below for a list of
   ad-hoc numbers used during initial development.  */

#define EM_NONE		  0	/* No machine */
#define EM_res011	 11	/* Reserved */
#define EM_res012	 12	/* Reserved */
#define EM_res013	 13	/* Reserved */
#define EM_res014	 14	/* Reserved */
#define EM_res016	 16	/* Reserved */
#define EM_res024	 24	/* Reserved */
#define EM_res025	 25	/* Reserved */
#define EM_res026	 26	/* Reserved */
#define EM_res027	 27	/* Reserved */
#define EM_res028	 28	/* Reserved */
#define EM_res029	 29	/* Reserved */
#define EM_res030	 30	/* Reserved */
#define EM_res031	 31	/* Reserved */
#define EM_res032	 32	/* Reserved */
#define EM_res033	 33	/* Reserved */
#define EM_res034	 34	/* Reserved */
#define EM_res035	 35	/* Reserved */
#define EM_ARM		 40	/* ARM */
#define EM_res121	121	/* Reserved */
#define EM_res122	122	/* Reserved */
#define EM_res123	123	/* Reserved */
#define EM_res124	124	/* Reserved */
#define EM_res125	125	/* Reserved */
#define EM_res126	126	/* Reserved */
#define EM_res127	127	/* Reserved */
#define EM_res128	128	/* Reserved */
#define EM_res129	129	/* Reserved */
#define EM_res130	130	/* Reserved */
#define EM_res133	133	/* Reserved */
#define EM_res143	143	/* Reserved */
#define EM_res145	145	/* Reserved */
#define EM_res146	146	/* Reserved */
#define EM_res147	147	/* Reserved */
#define EM_res148	148	/* Reserved */
#define EM_res149	149	/* Reserved */
#define EM_res150	150	/* Reserved */
#define EM_res151	151	/* Reserved */
#define EM_res152	152	/* Reserved */
#define EM_res153	153	/* Reserved */
#define EM_res154	154	/* Reserved */
#define EM_res155	155	/* Reserved */
#define EM_res156	156	/* Reserved */
#define EM_res157	157	/* Reserved */
#define EM_res158	158	/* Reserved */
#define EM_res159	159	/* Reserved */
#define EM_ARM184	184	/* Reserved by ARM */

/* Values for e_version.  */

#define EV_NONE		0		/* Invalid ELF version */
#define EV_CURRENT	1		/* Current version */

/* Value for e_phnum. */
#define PN_XNUM		0xffff		/* Extended numbering */

/* Values for program header, p_type field.  */

#define PT_NULL		0		/* Program header table entry unused */
#define PT_LOAD		1		/* Loadable program segment */
#define PT_DYNAMIC	2		/* Dynamic linking information */
#define PT_INTERP	3		/* Program interpreter */
#define PT_NOTE		4		/* Auxiliary information */
#define PT_SHLIB	5		/* Reserved, unspecified semantics */
#define PT_PHDR		6		/* Entry for header table itself */
#define PT_TLS		7		/* Thread local storage segment */
#define PT_LOOS		0x60000000	/* OS-specific */
#define PT_HIOS		0x6fffffff	/* OS-specific */
#define PT_LOPROC	0x70000000	/* Processor-specific */
#define PT_HIPROC	0x7FFFFFFF	/* Processor-specific */

#define PT_GNU_EH_FRAME	(PT_LOOS + 0x474e550) /* Frame unwind information */
#define PT_SUNW_EH_FRAME PT_GNU_EH_FRAME      /* Solaris uses the same value */
#define PT_GNU_STACK	(PT_LOOS + 0x474e551) /* Stack flags */
#define PT_GNU_RELRO	(PT_LOOS + 0x474e552) /* Read-only after relocation */

/* Mbind segments */
#define PT_GNU_MBIND_NUM 4096
#define PT_GNU_MBIND_LO (PT_LOOS + 0x474e555)
#define PT_GNU_MBIND_HI (PT_GNU_MBIND_LO + PT_GNU_MBIND_NUM - 1)

/* Program segment permissions, in program header p_flags field.  */

#define PF_X		(1 << 0)	/* Segment is executable */
#define PF_W		(1 << 1)	/* Segment is writable */
#define PF_R		(1 << 2)	/* Segment is readable */
/* #define PF_MASKOS	0x0F000000    *//* OS-specific reserved bits */
#define PF_MASKOS	0x0FF00000	/* New value, Oct 4, 1999 Draft */
#define PF_MASKPROC	0xF0000000	/* Processor-specific reserved bits */

/* Values for section header, sh_type field.  */

#define SHT_NULL	0		/* Section header table entry unused */
#define SHT_PROGBITS	1		/* Program specific (private) data */
#define SHT_SYMTAB	2		/* Link editing symbol table */
#define SHT_STRTAB	3		/* A string table */
#define SHT_RELA	4		/* Relocation entries with addends */
#define SHT_HASH	5		/* A symbol hash table */
#define SHT_DYNAMIC	6		/* Information for dynamic linking */
#define SHT_NOTE	7		/* Information that marks file */
#define SHT_NOBITS	8		/* Section occupies no space in file */
#define SHT_REL		9		/* Relocation entries, no addends */
#define SHT_SHLIB	10		/* Reserved, unspecified semantics */
#define SHT_DYNSYM	11		/* Dynamic linking symbol table */

#define SHT_INIT_ARRAY	  14		/* Array of ptrs to init functions */
#define SHT_FINI_ARRAY	  15		/* Array of ptrs to finish functions */
#define SHT_PREINIT_ARRAY 16		/* Array of ptrs to pre-init funcs */
#define SHT_GROUP	  17		/* Section contains a section group */
#define SHT_SYMTAB_SHNDX  18		/* Indicies for SHN_XINDEX entries */

#define SHT_LOOS	0x60000000	/* First of OS specific semantics */
#define SHT_HIOS	0x6fffffff	/* Last of OS specific semantics */

#define SHT_GNU_INCREMENTAL_INPUTS 0x6fff4700   /* incremental build data */
#define SHT_GNU_ATTRIBUTES 0x6ffffff5	/* Object attributes */
#define SHT_GNU_HASH	0x6ffffff6	/* GNU style symbol hash table */
#define SHT_GNU_LIBLIST	0x6ffffff7	/* List of prelink dependencies */

/* The next three section types are defined by Solaris, and are named
   SHT_SUNW*.  We use them in GNU code, so we also define SHT_GNU*
   versions.  */
#define SHT_SUNW_verdef	0x6ffffffd	/* Versions defined by file */
#define SHT_SUNW_verneed 0x6ffffffe	/* Versions needed by file */
#define SHT_SUNW_versym	0x6fffffff	/* Symbol versions */

#define SHT_GNU_verdef	SHT_SUNW_verdef
#define SHT_GNU_verneed	SHT_SUNW_verneed
#define SHT_GNU_versym	SHT_SUNW_versym

#define SHT_LOPROC	0x70000000	/* Processor-specific semantics, lo */
#define SHT_HIPROC	0x7FFFFFFF	/* Processor-specific semantics, hi */
#define SHT_LOUSER	0x80000000	/* Application-specific semantics */
/* #define SHT_HIUSER	0x8FFFFFFF    *//* Application-specific semantics */
#define SHT_HIUSER	0xFFFFFFFF	/* New value, defined in Oct 4, 1999 Draft */

/* Values for section header, sh_flags field.  */

#define SHF_WRITE	(1 << 0)	/* Writable data during execution */
#define SHF_ALLOC	(1 << 1)	/* Occupies memory during execution */
#define SHF_EXECINSTR	(1 << 2)	/* Executable machine instructions */
#define SHF_MERGE	(1 << 4)	/* Data in this section can be merged */
#define SHF_STRINGS	(1 << 5)	/* Contains null terminated character strings */
#define SHF_INFO_LINK	(1 << 6)	/* sh_info holds section header table index */
#define SHF_LINK_ORDER	(1 << 7)	/* Preserve section ordering when linking */
#define SHF_OS_NONCONFORMING (1 << 8)	/* OS specific processing required */
#define SHF_GROUP	(1 << 9)	/* Member of a section group */
#define SHF_TLS		(1 << 10)	/* Thread local storage section */
#define SHF_COMPRESSED	(1 << 11)	/* Section with compressed data */

/* #define SHF_MASKOS	0x0F000000    *//* OS-specific semantics */
#define SHF_MASKOS	0x0FF00000	/* New value, Oct 4, 1999 Draft */
#define SHF_GNU_BUILD_NOTE    (1 << 20)	/* Section contains GNU BUILD ATTRIBUTE notes.  */
#define SHF_MASKPROC	0xF0000000	/* Processor-specific semantics */

/* This used to be implemented as a processor specific section flag.
   We just make it generic.  */
#define SHF_EXCLUDE	0x80000000	/* Link editor is to exclude
					   this section from executable
					   and shared library that it
					   builds when those objects
					   are not to be further
					   relocated.  */

#define SHF_GNU_MBIND	0x01000000	/* Mbind section.  */

/* Compression types.  */
#define ELFCOMPRESS_ZLIB   1		/* Compressed with zlib.  */
#define ELFCOMPRESS_LOOS   0x60000000	/* OS-specific semantics, lo */
#define ELFCOMPRESS_HIOS   0x6FFFFFFF	/* OS-specific semantics, hi */
#define ELFCOMPRESS_LOPROC 0x70000000	/* Processor-specific semantics, lo */
#define ELFCOMPRESS_HIPROC 0x7FFFFFFF	/* Processor-specific semantics, hi */

/* Values of note segment descriptor types for core files.  */

#define NT_PRSTATUS	1		/* Contains copy of prstatus struct */
#define NT_FPREGSET	2		/* Contains copy of fpregset struct */
#define NT_PRPSINFO	3		/* Contains copy of prpsinfo struct */
#define NT_TASKSTRUCT	4		/* Contains copy of task struct */
#define NT_AUXV		6		/* Contains copy of Elfxx_auxv_t */
#define NT_PRXFPREG	0x46e62b7f	/* Contains a user_xfpregs_struct; */
					/*   note name must be "LINUX".  */
#define NT_ARM_VFP	0x400		/* ARM VFP registers */
/* The following definitions should really use NT_AARCH_..., but defined
   this way for compatibility with Linux.  */
#define NT_ARM_TLS	0x401		/* AArch TLS registers */
					/*   note name must be "LINUX".  */
#define NT_ARM_HW_BREAK	0x402		/* AArch hardware breakpoint registers */
					/*   note name must be "LINUX".  */
#define NT_ARM_HW_WATCH	0x403		/* AArch hardware watchpoint registers */
					/*   note name must be "LINUX".  */
#define NT_ARM_SVE	0x405		/* AArch SVE registers.  */
					/*   note name must be "LINUX".  */
#define NT_SIGINFO	0x53494749	/* Fields of siginfo_t.  */
#define NT_FILE		0x46494c45	/* Description of mapped files.  */

/* Note segments for core files on dir-style procfs systems.  */

#define NT_PSTATUS	10		/* Has a struct pstatus */
#define NT_FPREGS	12		/* Has a struct fpregset */
#define NT_PSINFO	13		/* Has a struct psinfo */
#define NT_LWPSTATUS	16		/* Has a struct lwpstatus_t */
#define NT_LWPSINFO	17		/* Has a struct lwpsinfo_t */
#define NT_WIN32PSTATUS	18		/* Has a struct win32_pstatus */

/* Note segment for SystemTap probes.  */
#define NT_STAPSDT	3

/* Note segments for core files on FreeBSD systems.  Note name is
   "FreeBSD".  */

#define	NT_FREEBSD_THRMISC	7	/* Thread miscellaneous info. */
#define	NT_FREEBSD_PROCSTAT_PROC	8	/* Procstat proc data. */
#define	NT_FREEBSD_PROCSTAT_FILES	9	/* Procstat files data. */
#define	NT_FREEBSD_PROCSTAT_VMMAP	10	/* Procstat vmmap data. */
#define	NT_FREEBSD_PROCSTAT_GROUPS	11	/* Procstat groups data. */
#define	NT_FREEBSD_PROCSTAT_UMASK	12	/* Procstat umask data. */
#define	NT_FREEBSD_PROCSTAT_RLIMIT	13	/* Procstat rlimit data. */
#define	NT_FREEBSD_PROCSTAT_OSREL	14	/* Procstat osreldate data. */
#define	NT_FREEBSD_PROCSTAT_PSSTRINGS	15	/* Procstat ps_strings data. */
#define	NT_FREEBSD_PROCSTAT_AUXV	16	/* Procstat auxv data. */
#define	NT_FREEBSD_PTLWPINFO	17	/* Thread ptrace miscellaneous info. */

/* Note segments for core files on NetBSD systems.  Note name
   must start with "NetBSD-CORE".  */

#define NT_NETBSDCORE_PROCINFO	1	/* Has a struct procinfo */
#define NT_NETBSDCORE_FIRSTMACH	32	/* start of machdep note types */


/* Note segments for core files on OpenBSD systems.  Note name is
   "OpenBSD".  */

#define NT_OPENBSD_PROCINFO	10
#define NT_OPENBSD_AUXV		11
#define NT_OPENBSD_REGS		20
#define NT_OPENBSD_FPREGS	21
#define NT_OPENBSD_XFPREGS	22
#define NT_OPENBSD_WCOOKIE	23


/* Note segments for core files on SPU systems.  Note name
   must start with "SPU/".  */

#define NT_SPU		1

/* Values of note segment descriptor types for object files.  */

#define NT_VERSION	1		/* Contains a version string.  */
#define NT_ARCH		2		/* Contains an architecture string.  */

/* Values for notes in non-core files using name "GNU".  */

#define NT_GNU_ABI_TAG		1
#define NT_GNU_HWCAP		2	/* Used by ld.so and kernel vDSO.  */
#define NT_GNU_BUILD_ID		3	/* Generated by ld --build-id.  */
#define NT_GNU_GOLD_VERSION	4	/* Generated by gold.  */
#define NT_GNU_PROPERTY_TYPE_0  5	/* Generated by gcc.  */

#define NT_GNU_BUILD_ATTRIBUTE_OPEN	0x100
#define NT_GNU_BUILD_ATTRIBUTE_FUNC	0x101

#define GNU_BUILD_ATTRIBUTE_TYPE_NUMERIC	'*'
#define GNU_BUILD_ATTRIBUTE_TYPE_STRING		'$'
#define GNU_BUILD_ATTRIBUTE_TYPE_BOOL_TRUE	'+'
#define GNU_BUILD_ATTRIBUTE_TYPE_BOOL_FALSE	'!'

#define GNU_BUILD_ATTRIBUTE_VERSION	1
#define GNU_BUILD_ATTRIBUTE_STACK_PROT	2
#define GNU_BUILD_ATTRIBUTE_RELRO	3
#define GNU_BUILD_ATTRIBUTE_STACK_SIZE	4
#define GNU_BUILD_ATTRIBUTE_TOOL	5
#define GNU_BUILD_ATTRIBUTE_ABI		6
#define GNU_BUILD_ATTRIBUTE_PIC		7
#define GNU_BUILD_ATTRIBUTE_SHORT_ENUM	8

#define NOTE_GNU_PROPERTY_SECTION_NAME	".note.gnu.property"
#define GNU_BUILD_ATTRS_SECTION_NAME	".gnu.build.attributes"

/* Values used in GNU .note.gnu.property notes (NT_GNU_PROPERTY_TYPE_0).  */
#define GNU_PROPERTY_STACK_SIZE			1
#define GNU_PROPERTY_NO_COPY_ON_PROTECTED	2

/* Processor-specific semantics, lo */
#define GNU_PROPERTY_LOPROC  0xc0000000
/* Processor-specific semantics, hi */
#define GNU_PROPERTY_HIPROC  0xdfffffff
/* Application-specific semantics, lo */
#define GNU_PROPERTY_LOUSER  0xe0000000
/* Application-specific semantics, hi */
#define GNU_PROPERTY_HIUSER  0xffffffff

/* Values used in GNU .note.ABI-tag notes (NT_GNU_ABI_TAG).  */
#define GNU_ABI_TAG_LINUX	0
#define GNU_ABI_TAG_HURD	1
#define GNU_ABI_TAG_SOLARIS	2
#define GNU_ABI_TAG_FREEBSD	3
#define GNU_ABI_TAG_NETBSD	4
#define GNU_ABI_TAG_SYLLABLE	5
#define GNU_ABI_TAG_NACL	6

/* Values for NetBSD .note.netbsd.ident notes.  Note name is "NetBSD".  */

#define NT_NETBSD_IDENT		1
#define NT_NETBSD_MARCH		5

/* Values for OpenBSD .note.openbsd.ident notes.  Note name is "OpenBSD".  */

#define NT_OPENBSD_IDENT	1

/* Values for FreeBSD .note.ABI-tag notes.  Note name is "FreeBSD".  */

#define NT_FREEBSD_ABI_TAG	1

/* These three macros disassemble and assemble a symbol table st_info field,
   which contains the symbol binding and symbol type.  The STB_ and STT_
   defines identify the binding and type.  */

#define ELF_ST_BIND(val)		(((unsigned int)(val)) >> 4)
#define ELF_ST_TYPE(val)		((val) & 0xF)
#define ELF_ST_INFO(bind,type)		(((bind) << 4) + ((type) & 0xF))

/* The 64bit and 32bit versions of these macros are identical, but
   the ELF spec defines them, so here they are.  */
#define ELF32_ST_BIND  ELF_ST_BIND
#define ELF32_ST_TYPE  ELF_ST_TYPE
#define ELF32_ST_INFO  ELF_ST_INFO
#define ELF64_ST_BIND  ELF_ST_BIND
#define ELF64_ST_TYPE  ELF_ST_TYPE
#define ELF64_ST_INFO  ELF_ST_INFO

/* This macro disassembles and assembles a symbol's visibility into
   the st_other field.  The STV_ defines specify the actual visibility.  */

#define ELF_ST_VISIBILITY(v)		((v) & 0x3)
/* The remaining bits in the st_other field are not currently used.
   They should be set to zero.  */

#define ELF32_ST_VISIBILITY  ELF_ST_VISIBILITY
#define ELF64_ST_VISIBILITY  ELF_ST_VISIBILITY


#define STN_UNDEF	0		/* Undefined symbol index */

#define STB_LOCAL	0		/* Symbol not visible outside obj */
#define STB_GLOBAL	1		/* Symbol visible outside obj */
#define STB_WEAK	2		/* Like globals, lower precedence */
#define STB_LOOS	10		/* OS-specific semantics */
#define STB_GNU_UNIQUE	10		/* Symbol is unique in namespace */
#define STB_HIOS	12		/* OS-specific semantics */
#define STB_LOPROC	13		/* Processor-specific semantics */
#define STB_HIPROC	15		/* Processor-specific semantics */

#define STT_NOTYPE	0		/* Symbol type is unspecified */
#define STT_OBJECT	1		/* Symbol is a data object */
#define STT_FUNC	2		/* Symbol is a code object */
#define STT_SECTION	3		/* Symbol associated with a section */
#define STT_FILE	4		/* Symbol gives a file name */
#define STT_COMMON	5		/* An uninitialised common block */
#define STT_TLS		6		/* Thread local data object */
#define STT_RELC	8		/* Complex relocation expression */
#define STT_SRELC	9		/* Signed Complex relocation expression */
#define STT_LOOS	10		/* OS-specific semantics */
#define STT_GNU_IFUNC	10		/* Symbol is an indirect code object */
#define STT_HIOS	12		/* OS-specific semantics */
#define STT_LOPROC	13		/* Processor-specific semantics */
#define STT_HIPROC	15		/* Processor-specific semantics */

/* The following constants control how a symbol may be accessed once it has
   become part of an executable or shared library.  */

#define STV_DEFAULT	0		/* Visibility is specified by binding type */
#define STV_INTERNAL	1		/* OS specific version of STV_HIDDEN */
#define STV_HIDDEN	2		/* Can only be seen inside currect component */
#define STV_PROTECTED	3		/* Treat as STB_LOCAL inside current component */

/* Relocation info handling macros.  */

#define ELF32_R_SYM(i)		((i) >> 8)
#define ELF32_R_TYPE(i)		((i) & 0xff)
#define ELF32_R_INFO(s,t)	(((s) << 8) + ((t) & 0xff))

#define ELF64_R_SYM(i)		((i) >> 32)
#define ELF64_R_TYPE(i)		((i) & 0xffffffff)
#define ELF64_R_INFO(s,t)	(((bfd_vma) (s) << 31 << 1) + (bfd_vma) (t))

/* Dynamic section tags.  */

#define DT_NULL		0
#define DT_NEEDED	1
#define DT_PLTRELSZ	2
#define DT_PLTGOT	3
#define DT_HASH		4
#define DT_STRTAB	5
#define DT_SYMTAB	6
#define DT_RELA		7
#define DT_RELASZ	8
#define DT_RELAENT	9
#define DT_STRSZ	10
#define DT_SYMENT	11
#define DT_INIT		12
#define DT_FINI		13
#define DT_SONAME	14
#define DT_RPATH	15
#define DT_SYMBOLIC	16
#define DT_REL		17
#define DT_RELSZ	18
#define DT_RELENT	19
#define DT_PLTREL	20
#define DT_DEBUG	21
#define DT_TEXTREL	22
#define DT_JMPREL	23
#define DT_BIND_NOW	24
#define DT_INIT_ARRAY	25
#define DT_FINI_ARRAY	26
#define DT_INIT_ARRAYSZ 27
#define DT_FINI_ARRAYSZ 28
#define DT_RUNPATH	29
#define DT_FLAGS	30
#define DT_ENCODING	32
#define DT_PREINIT_ARRAY   32
#define DT_PREINIT_ARRAYSZ 33
#define DT_SYMTAB_SHNDX    34

/* Note, the Oct 4, 1999 draft of the ELF ABI changed the values
   for DT_LOOS and DT_HIOS.  Some implementations however, use
   values outside of the new range (see below).	 */
#define OLD_DT_LOOS	0x60000000
#define DT_LOOS		0x6000000d
#define DT_HIOS		0x6ffff000
#define OLD_DT_HIOS	0x6fffffff

#define DT_LOPROC	0x70000000
#define DT_HIPROC	0x7fffffff

/* The next 2 dynamic tag ranges, integer value range (DT_VALRNGLO to
   DT_VALRNGHI) and virtual address range (DT_ADDRRNGLO to DT_ADDRRNGHI),
   are used on Solaris.  We support them everywhere.  Note these values
   lie outside of the (new) range for OS specific values.  This is a
   deliberate special case and we maintain it for backwards compatability.
 */
#define DT_VALRNGLO	0x6ffffd00
#define DT_GNU_PRELINKED 0x6ffffdf5
#define DT_GNU_CONFLICTSZ 0x6ffffdf6
#define DT_GNU_LIBLISTSZ 0x6ffffdf7
#define DT_CHECKSUM	0x6ffffdf8
#define DT_PLTPADSZ	0x6ffffdf9
#define DT_MOVEENT	0x6ffffdfa
#define DT_MOVESZ	0x6ffffdfb
#define DT_FEATURE	0x6ffffdfc
#define DT_POSFLAG_1	0x6ffffdfd
#define DT_SYMINSZ	0x6ffffdfe
#define DT_SYMINENT	0x6ffffdff
#define DT_VALRNGHI	0x6ffffdff

#define DT_ADDRRNGLO	0x6ffffe00
#define DT_GNU_HASH	0x6ffffef5
#define DT_TLSDESC_PLT	0x6ffffef6
#define DT_TLSDESC_GOT	0x6ffffef7
#define DT_GNU_CONFLICT	0x6ffffef8
#define DT_GNU_LIBLIST	0x6ffffef9
#define DT_CONFIG	0x6ffffefa
#define DT_DEPAUDIT	0x6ffffefb
#define DT_AUDIT	0x6ffffefc
#define DT_PLTPAD	0x6ffffefd
#define DT_MOVETAB	0x6ffffefe
#define DT_SYMINFO	0x6ffffeff
#define DT_ADDRRNGHI	0x6ffffeff

#define DT_RELACOUNT	0x6ffffff9
#define DT_RELCOUNT	0x6ffffffa
#define DT_FLAGS_1	0x6ffffffb
#define DT_VERDEF	0x6ffffffc
#define DT_VERDEFNUM	0x6ffffffd
#define DT_VERNEED	0x6ffffffe
#define DT_VERNEEDNUM	0x6fffffff

/* This tag is a GNU extension to the Solaris version scheme.  */
#define DT_VERSYM	0x6ffffff0

#define DT_LOPROC	0x70000000
#define DT_HIPROC	0x7fffffff

/* These section tags are used on Solaris.  We support them
   everywhere, and hope they do not conflict.  */

#define DT_AUXILIARY	0x7ffffffd
#define DT_USED		0x7ffffffe
#define DT_FILTER	0x7fffffff


/* Values used in DT_FEATURE .dynamic entry.  */
#define DTF_1_PARINIT	0x00000001
/* From

   http://docs.sun.com:80/ab2/coll.45.13/LLM/@Ab2PageView/21165?Ab2Lang=C&Ab2Enc=iso-8859-1

   DTF_1_CONFEXP is the same as DTF_1_PARINIT. It is a typo. The value
   defined here is the same as the one in <sys/link.h> on Solaris 8.  */
#define DTF_1_CONFEXP	0x00000002

/* Flag values used in the DT_POSFLAG_1 .dynamic entry.	 */
#define DF_P1_LAZYLOAD	0x00000001
#define DF_P1_GROUPPERM	0x00000002

/* Flag value in in the DT_FLAGS_1 .dynamic entry.  */
#define DF_1_NOW	0x00000001
#define DF_1_GLOBAL	0x00000002
#define DF_1_GROUP	0x00000004
#define DF_1_NODELETE	0x00000008
#define DF_1_LOADFLTR	0x00000010
#define DF_1_INITFIRST	0x00000020
#define DF_1_NOOPEN	0x00000040
#define DF_1_ORIGIN	0x00000080
#define DF_1_DIRECT	0x00000100
#define DF_1_TRANS	0x00000200
#define DF_1_INTERPOSE	0x00000400
#define DF_1_NODEFLIB	0x00000800
#define DF_1_NODUMP	0x00001000
#define DF_1_CONFALT	0x00002000
#define DF_1_ENDFILTEE	0x00004000
#define	DF_1_DISPRELDNE	0x00008000
#define	DF_1_DISPRELPND	0x00010000
#define	DF_1_NODIRECT	0x00020000
#define	DF_1_IGNMULDEF	0x00040000
#define	DF_1_NOKSYMS	0x00080000
#define	DF_1_NOHDR	0x00100000
#define	DF_1_EDITED	0x00200000
#define	DF_1_NORELOC	0x00400000
#define	DF_1_SYMINTPOSE	0x00800000
#define	DF_1_GLOBAUDIT	0x01000000
#define	DF_1_SINGLETON	0x02000000
#define	DF_1_STUB	0x04000000
#define	DF_1_PIE	0x08000000
#define	DF_1_KMOD	0x10000000
#define	DF_1_WEAKFILTER	0x20000000
#define	DF_1_NOCOMMON	0x40000000

/* Flag values for the DT_FLAGS entry.	*/
#define DF_ORIGIN	(1 << 0)
#define DF_SYMBOLIC	(1 << 1)
#define DF_TEXTREL	(1 << 2)
#define DF_BIND_NOW	(1 << 3)
#define DF_STATIC_TLS	(1 << 4)

/* These constants are used for the version number of a Elf32_Verdef
   structure.  */

#define VER_DEF_NONE		0
#define VER_DEF_CURRENT		1

/* These constants appear in the vd_flags field of a Elf32_Verdef
   structure.

   Cf. the Solaris Linker and Libraries Guide, Ch. 7, Object File Format,
   Versioning Sections, for a description:

   http://docs.sun.com/app/docs/doc/819-0690/chapter6-93046?l=en&a=view  */

#define VER_FLG_BASE		0x1
#define VER_FLG_WEAK		0x2
#define VER_FLG_INFO		0x4

/* These special constants can be found in an Elf32_Versym field.  */

#define VER_NDX_LOCAL		0
#define VER_NDX_GLOBAL		1

/* These constants are used for the version number of a Elf32_Verneed
   structure.  */

#define VER_NEED_NONE		0
#define VER_NEED_CURRENT	1

/* This flag appears in a Versym structure.  It means that the symbol
   is hidden, and is only visible with an explicit version number.
   This is a GNU extension.  */

#define VERSYM_HIDDEN		0x8000

/* This is the mask for the rest of the Versym information.  */

#define VERSYM_VERSION		0x7fff

/* This is a special token which appears as part of a symbol name.  It
   indictes that the rest of the name is actually the name of a
   version node, and is not part of the actual name.  This is a GNU
   extension.  For example, the symbol name `stat@ver2' is taken to
   mean the symbol `stat' in version `ver2'.  */

#define ELF_VER_CHR	'@'

/* Possible values for si_boundto.  */

#define SYMINFO_BT_SELF		0xffff	/* Symbol bound to self */
#define SYMINFO_BT_PARENT	0xfffe	/* Symbol bound to parent */
#define SYMINFO_BT_LOWRESERVE	0xff00	/* Beginning of reserved entries */

/* Possible bitmasks for si_flags.  */

#define SYMINFO_FLG_DIRECT	0x0001	/* Direct bound symbol */
#define SYMINFO_FLG_PASSTHRU	0x0002	/* Pass-thru symbol for translator */
#define SYMINFO_FLG_COPY	0x0004	/* Symbol is a copy-reloc */
#define SYMINFO_FLG_LAZYLOAD	0x0008	/* Symbol bound to object to be lazy loaded */

/* Syminfo version values.  */

#define SYMINFO_NONE		0
#define SYMINFO_CURRENT		1
#define SYMINFO_NUM		2

/* Section Group Flags.	 */

#define GRP_COMDAT		0x1	/* A COMDAT group */
#define GRP_MASKOS 	 0x0ff00000	/* Bits in this range reserved for OS specific use.  */
#define GRP_MASKPROC 	 0xf0000000	/* Bits in this range reserved for processor use.  */

/* Auxv a_type values.  */

#define AT_NULL		0		/* End of vector */
#define AT_IGNORE	1		/* Entry should be ignored */
#define AT_EXECFD	2		/* File descriptor of program */
#define AT_PHDR		3		/* Program headers for program */
#define AT_PHENT	4		/* Size of program header entry */
#define AT_PHNUM	5		/* Number of program headers */
#define AT_PAGESZ	6		/* System page size */
#define AT_BASE		7		/* Base address of interpreter */
#define AT_FLAGS	8		/* Flags */
#define AT_ENTRY	9		/* Entry point of program */
#define AT_NOTELF	10		/* Program is not ELF */
#define AT_UID		11		/* Real uid */
#define AT_EUID		12		/* Effective uid */
#define AT_GID		13		/* Real gid */
#define AT_EGID		14		/* Effective gid */
#define AT_CLKTCK	17		/* Frequency of times() */
#define AT_PLATFORM	15		/* String identifying platform.  */
#define AT_HWCAP	16		/* Machine dependent hints about
					   processor capabilities.  */
#define AT_FPUCW	18		/* Used FPU control word.  */
#define AT_DCACHEBSIZE	19		/* Data cache block size.  */
#define AT_ICACHEBSIZE	20		/* Instruction cache block size.  */
#define AT_UCACHEBSIZE	21		/* Unified cache block size.  */
#define AT_IGNOREPPC	22		/* Entry should be ignored */
#define	AT_SECURE	23		/* Boolean, was exec setuid-like?  */
#define AT_BASE_PLATFORM 24		/* String identifying real platform,
					   may differ from AT_PLATFORM.  */
#define AT_RANDOM	25		/* Address of 16 random bytes.  */
#define AT_HWCAP2	26		/* Extension of AT_HWCAP.  */
#define AT_EXECFN	31		/* Filename of executable.  */
/* Pointer to the global system page used for system calls and other
   nice things.  */
#define AT_SYSINFO	32
#define AT_SYSINFO_EHDR	33 /* Pointer to ELF header of system-supplied DSO.  */

/* More complete cache descriptions than AT_[DIU]CACHEBSIZE.  If the
   value is -1, then the cache doesn't exist.  Otherwise:

   bit 0-3:  Cache set-associativity; 0 means fully associative.
   bit 4-7:  Log2 of cacheline size.
   bit 8-31:  Size of the entire cache >> 8.  */

#define AT_L1I_CACHESHAPE 34
#define AT_L1D_CACHESHAPE 35
#define AT_L2_CACHESHAPE  36
#define AT_L3_CACHESHAPE  37

#define AT_FREEBSD_EXECPATH     15      /* Path to the executable. */
#define AT_FREEBSD_CANARY       16      /* Canary for SSP. */
#define AT_FREEBSD_CANARYLEN    17      /* Length of the canary. */
#define AT_FREEBSD_OSRELDATE    18      /* OSRELDATE. */
#define AT_FREEBSD_NCPUS        19      /* Number of CPUs. */
#define AT_FREEBSD_PAGESIZES    20      /* Pagesizes. */
#define AT_FREEBSD_PAGESIZESLEN 21      /* Number of pagesizes. */
#define AT_FREEBSD_TIMEKEEP     22      /* Pointer to timehands. */
#define AT_FREEBSD_STACKPROT    23      /* Initial stack protection. */
#define AT_FREEBSD_EHDRFLAGS    24      /* e_flags field from ELF header. */
#define AT_FREEBSD_HWCAP        25      /* CPU feature flags. */

#define AT_SUN_UID      2000    /* Effective user ID.  */
#define AT_SUN_RUID     2001    /* Real user ID.  */
#define AT_SUN_GID      2002    /* Effective group ID.  */
#define AT_SUN_RGID     2003    /* Real group ID.  */
#define AT_SUN_LDELF    2004    /* Dynamic linker's ELF header.  */
#define AT_SUN_LDSHDR   2005    /* Dynamic linker's section headers.  */
#define AT_SUN_LDNAME   2006    /* String giving name of dynamic linker.  */
#define AT_SUN_LPAGESZ  2007    /* Large pagesize.   */
#define AT_SUN_PLATFORM 2008    /* Platform name string.  */
#undef AT_SUN_HWCAP
#define AT_SUN_HWCAP    2009	/* Machine dependent hints about
				   processor capabilities.  */
#define AT_SUN_IFLUSH   2010    /* Should flush icache? */
#define AT_SUN_CPU      2011    /* CPU name string.  */
#define AT_SUN_EMUL_ENTRY 2012	/* COFF entry point address.  */
#define AT_SUN_EMUL_EXECFD 2013	/* COFF executable file descriptor.  */
#define AT_SUN_EXECNAME 2014    /* Canonicalized file name given to execve.  */
#define AT_SUN_MMU      2015    /* String for name of MMU module.   */
#define AT_SUN_LDDATA   2016    /* Dynamic linker's data segment address.  */
#define AT_SUN_AUXFLAGS	2017	/* AF_SUN_ flags passed from the kernel.  */


#endif /* _ELF_COMMON_H */
