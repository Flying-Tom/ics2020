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
    //Log("Loader:%s", filename);
    int fd = fs_open(filename, 0, 0);
    fs_read(fd, (void *)&elf, sizeof(Elf_Ehdr));

    for (size_t i = 0; i < elf.e_phnum; i++)
    {
        Elf_Phdr phdr;
        printf("i:%d\n", i);
        fs_lseek(fd, elf.e_phoff + i * elf.e_phentsize, SEEK_SET);
        fs_read(fd, (void *)&phdr, elf.e_phentsize);
        if (phdr.p_type == PT_LOAD)
        {
            fs_lseek(fd, phdr.p_offset, SEEK_SET);
            fs_read(fd, (void *)phdr.p_vaddr, phdr.p_filesz);
            memset((void *)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
        }
    }
    fs_close(fd);
    return elf.e_entry;
}

void naive_uload(PCB *pcb, const char *filename)
{
    uintptr_t entry = loader(pcb, filename);
    Log("Filename:%s", filename);
    Log("Jump to entry = %p", entry);
    ((void (*)())entry)();
}
