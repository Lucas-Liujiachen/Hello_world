<!-- markdownlint-disable MD033 -->

# ATF启动流程二：bl2

[back](./ATF.md)

BL2 image 将为后续 image 的加载执行相关的初始化操作。主要是内存、MMU、串口以及 EL3 软件运行环境的设置，并加载 BL3 的 image 到 RAM 中。通过查看 bl2.ld.S 文件可以得知，bl2 image 的入口函数为 bl2_entrypoint。该函数定义在`bl2/aarch63/bl2_entrypoint.S`。
bl2.ld.S 链接脚本用于定义 BL2 固件的内存布局和各部分的存放位置。

## 2.1 bl2.ld.S 链接脚本文件分析

<details>
    <summary>点击这里展开/收起代码块</summary>

```asm
OUTPUT_FORMAT(PLATFORM_LINKER_FORMAT)
OUTPUT_ARCH(PLATFORM_LINKER_ARCH)
ENTRY(bl2_entrypoint)

MEMORY {
    RAM (rwx): ORIGIN = BL2_BASE, LENGTH = BL2_LIMIT - BL2_BASE
}

SECTIONS
{
    . = BL2_BASE;
    ASSERT(. == ALIGN(PAGE_SIZE),
           "BL2_BASE address is not aligned on a page boundary.")

#if SEPARATE_CODE_AND_RODATA
    .text . : {
        __TEXT_START__ = .;
        *bl2_entrypoint.o(.text*)
        *(.text*)
        *(.vectors)
        . = ALIGN(PAGE_SIZE);
        __TEXT_END__ = .;
     } >RAM

     .ARM.extab . : {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
     } >RAM

     .ARM.exidx . : {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
     } >RAM

    .rodata . : {
        __RODATA_START__ = .;
        *(.rodata*)

        . = ALIGN(8);
        __PARSER_LIB_DESCS_START__ = .;
        KEEP(*(.img_parser_lib_descs))
        __PARSER_LIB_DESCS_END__ = .;

        . = ALIGN(PAGE_SIZE);
        __RODATA_END__ = .;
    } >RAM
#else
    ro . : {
        __RO_START__ = .;
        *bl2_entrypoint.o(.text*)
        *(.text*)
        *(.rodata*)

        . = ALIGN(8);
        __PARSER_LIB_DESCS_START__ = .;
        KEEP(*(.img_parser_lib_descs))
        __PARSER_LIB_DESCS_END__ = .;

        *(.vectors)
        __RO_END_UNALIGNED__ = .;

        . = ALIGN(PAGE_SIZE);
        __RO_END__ = .;
    } >RAM
#endif

    __RW_START__ = . ;

    .data . : {
        __DATA_START__ = .;
        *(.data*)
        __DATA_END__ = .;
    } >RAM

    stacks (NOLOAD) : {
        __STACKS_START__ = .;
        *(tzfw_normal_stacks)
        __STACKS_END__ = .;
    } >RAM

    .bss : ALIGN(16) {
        __BSS_START__ = .;
        *(SORT_BY_ALIGNMENT(.bss*))
        *(COMMON)
        __BSS_END__ = .;
    } >RAM

    xlat_table (NOLOAD) : {
        *(xlat_table)
    } >RAM

#if USE_COHERENT_MEM
    coherent_ram (NOLOAD) : ALIGN(PAGE_SIZE) {
        __COHERENT_RAM_START__ = .;
        *(tzfw_coherent_mem)
        __COHERENT_RAM_END_UNALIGNED__ = .;

        . = ALIGN(PAGE_SIZE);
        __COHERENT_RAM_END__ = .;
    } >RAM
#endif

    __RW_END__ = .;
    __BL2_END__ = .;

    __BSS_SIZE__ = SIZEOF(.bss);

#if USE_COHERENT_MEM
    __COHERENT_RAM_UNALIGNED_SIZE__ =
        __COHERENT_RAM_END_UNALIGNED__ - __COHERENT_RAM_START__;
#endif

    ASSERT(. <= BL2_LIMIT, "BL2 image has exceeded its limit.")
}
```

</details>

## 2.2 bl2_entrypoint 源码分析

### 2-2-1 源代码

<details>
    <summary>点击这里展开/收起代码</summary>

```asm
func bl2_entrypoint

  mov  x20, x0
  mov  x21, x1
  mov  x22, x2
  mov  x23, x3

  adr  x0, early_exceptions
  msr  vbar_el1, x0
  isb

  msr  daifclr, #DAIF_ABT_BIT

  mov  x1, #(SCTLR_I_BIT | SCTLR_A_BIT | SCTLR_SA_BIT)
  mrs  x0, sctlr_el1
  orr  x0, x0, x1
  bic  x0, x0, #SCTLR_DSSBS_BIT
  msr  sctlr_el1, x0
  isb

  adr  x0, __RW_START__
  adr  x1, __RW_END__
  sub  x1, x1, x0
  bl  inv_dcache_range

  adrp  x0, __BSS_START__
  add  x0, x0, :lo12:__BSS_START__
  adrp  x1, __BSS_END__
  add  x1, x1, :lo12:__BSS_END__
  sub  x1, x1, x0
  bl  zeromem

#if USE_COHERENT_MEM
  adrp  x0, __COHERENT_RAM_START__
  add  x0, x0, :lo12:__COHERENT_RAM_START__
  adrp  x1, __COHERENT_RAM_END_UNALIGNED__
  add  x1, x1, :lo12:__COHERENT_RAM_END_UNALIGNED__
  sub  x1, x1, x0
  bl  zeromem
#endif

  bl  plat_set_my_stack

#if STACK_PROTECTOR_ENABLED
  bl  update_stack_protector_canary
#endif

  mov  x0, x20
  mov  x1, x21
  mov  x2, x22
  mov  x3, x23
  bl  bl2_setup

#if ENABLE_PAUTH
  bl  pauth_init_enable_el1
#endif /* ENABLE_PAUTH */

  bl  bl2_main

  no_ret  plat_panic_handler

endfunc bl2_entrypoint
```

</details>

## 2-3

## 2-4
