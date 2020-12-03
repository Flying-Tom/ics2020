#include <proc.h>
#include <elf.h>
//#include "ramdisk.c"

#ifdef __LP64__
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#else
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Phdr Elf32_Phdr
#endif

extern int ramdisk_read(void *, size_t, size_t);

static uintptr_t loader(PCB *pcb, const char *filename)
{
	Elf_Ehdr elf;
	ramdisk_read((void *)&elf, 0, sizeof(Elf_Ehdr));
	Elf_Phdr phdr[elf.e_phnum];

	assert(elf.e_phentsize == sizeof(Elf_Phdr));
	printf("%d\n",elf.e_phnum);
	printf("%d\n",elf.e_phentsize);
	printf("%d\n",elf.e_entry);
	for (size_t i = 0; i < elf.e_phnum; i++)
	{
		ramdisk_read((void *)&phdr[i], elf.e_entry + i * elf.e_phentsize, elf.e_phentsize);
		if (phdr[i].p_type == PT_LOAD)
		{
			TODO();
			ramdisk_read((void *)phdr[i].p_vaddr, phdr[i].p_offset, phdr[i].p_memsz);
			memset((void *)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
		}
	}
	return 0;
}

void naive_uload(PCB *pcb, const char *filename)
{
	uintptr_t entry = loader(pcb, filename);
	Log("Jump to entry = %p", entry);
	((void (*)())entry)();
}
