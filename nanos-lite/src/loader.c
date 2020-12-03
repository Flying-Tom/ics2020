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
    //TODO();
    Elf_Ehdr elf;
    ramdisk_read((void *)&elf, 0, sizeof(elf));
    //Elf_Phdr phdr[elf.e_phnum];
    /*for (size_t i = 0; i < elf.e_phnum; i++)
    {
        ramdisk_read(void(*) phdr[i].p_vadrr, 0, elf.e_phentsize);
        if (phdr[i].p_type == PT_LOAD)
    }*/
    return 0;
}

void naive_uload(PCB *pcb, const char *filename)
{
    uintptr_t entry = loader(pcb, filename);
    Log("Jump to entry = %p", entry);
    ((void (*)())entry)();
}
