#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#else
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename)
{

    Elf_Ehdr elf;
    int fd = fs_open(filename, 0, 0);
    fs_read(fd, (void *)&elf, sizeof(Elf_Ehdr));
    Elf_Phdr phdr[elf.e_phnum];

    for (size_t i = 0; i < elf.e_phnum; i++)
    {
        fs_lseek(fd, elf.e_phoff + i * elf.e_phentsize, SEEK_SET);
        fs_read(fd, (void *)&phdr[i], elf.e_phentsize);
        if (phdr[i].p_type == PT_LOAD)
        {
            fs_lseek(fd, phdr[i].p_offset, SEEK_SET);
            fs_read(fd, (void *)phdr[i].p_vaddr, phdr[i].p_filesz);
            memset((void *)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
        }
    }
    return elf.e_entry;
}

void naive_uload(PCB *pcb, const char *filename)
{
    uintptr_t entry = loader(pcb, filename);
    Log("Filename:%s", filename);
    Log("Jump to entry = %p", entry);
    ((void (*)())entry)();
}
