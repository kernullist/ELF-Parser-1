#include "ELFFile.hpp"

using namespace ELF;

ELFFile::ELFFile(const char* elfFile) : elfFile(elfFile)
{
    elfHeader = (Elf32Ehdr*) elfFile;
}

void ELFFile::Print()
{
    printIdentificationBytes();
    //printSectionHeaders();
    //printProgramHeaders();

    PHTParser phtParser(elfHeader);
    phtParser.print();

    SHTParser shtParser(elfHeader);
    shtParser.print();
}
bool ELFFile::validMagic()
{
    if(elfHeader->e_ident[EI_MAG0] != ELF_MAG0) return false;
    if(elfHeader->e_ident[EI_MAG1] != ELF_MAG1) return false;
    if(elfHeader->e_ident[EI_MAG2] != ELF_MAG2) return false;
    if(elfHeader->e_ident[EI_MAG3] != ELF_MAG3) return false;

    printMachine();
    printType();

    return true;
}
void ELFFile::printIdentificationBytes()
{
    printConditional("[ELF] Magic", "valid", validMagic());
    printClass();
    printDataEncoding();
    printConditional("[ELF] Header Version", "current", elfHeader->e_version == EV_CURRENT);
}

void ELFFile::printMachine()
{
    printf("[ELF] Target machine is ");
    switch(elfHeader->e_machine)
    {
        case EM_68K: printf("68K"); break;
        case EM_860: printf("Intel 860"); break;
        case EM_86K: printf("86K"); break;
        case EM_386: printf("Intel 80386"); break;
        case EM_M32: printf("M32"); break;
        case EM_MIPS: printf("MIPS"); break;
        case EM_SPARC: printf("SPARC"); break;
        case EM_NONE: printf("not specified."); break;
        default: printf("unknown: %#08x", elfHeader->e_machine);
    }
    printf("\n");
}
void ELFFile::printType()
{
    printf("[ELF] Type is ");
    switch(elfHeader->e_type)
    {
        case ET_CORE:  printf("CORE"); break;
        case ET_DYN: printf("Shared Object"); break;
        case ET_REL: printf("Relocatable"); break;
        case ET_EXEC: printf("Exectutable"); break;
        default: printf("unknown: %#08x", elfHeader->e_type);
    }
    printf(".\n");
}
void ELFFile::printClass()
{
    printf("[ELF] Class is ");
    switch(elfHeader->e_ident[EI_CLASS])
    {
        case ELFCLASS32: printf("32 bit"); break;
        case ELFCLASS64: printf("64 bit"); break;
        case ELFCLASSNONE: printf("invalid"); break;
        default: printf("unkown: %#08x", elfHeader->e_ident[EI_CLASS]);
    }
    printf(".\n");
}
void ELFFile::printDataEncoding()
{
    printf("[ELF] Encoding is ");
    switch(elfHeader->e_ident[E_DATA])
    {
        case ELFDATA2LSB: printf("LSB (little endian)"); break;
        case ELFDATA2MSB: printf("MSB (big endian)"); break;
        case ELFDATANONE: printf("not specified."); break;
        default: printf("unkown: %#08x", elfHeader->e_ident[E_DATA]);
    }
    printf(".\n");
}
