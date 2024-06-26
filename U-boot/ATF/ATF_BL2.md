<!-- markdownlint-disable MD033 -->

# ATF启动流程二：bl2

[back](./ATF.md)

BL2 image 将为后续 image 的加载执行相关的初始化操作。主要是内存、MMU、串口以及 EL3 软件运行环境的设置，并加载 BL3 的 image 到 RAM 中。通过查看 bl2.ld.S 文件可以得知，bl2 image 的入口函数为 bl2_entrypoint。该函数定义在`bl2/aarch63/bl2_entrypoint.S`。
bl2.ld.S 链接脚本用于定义 BL2 固件的内存布局和各部分的存放位置。

## 2.1 bl2.ld.S 链接脚本文件分析

### 2-1-1 源代码

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

### 2-1-2 分析

和 BL1 相比，BL2 的.ld.S 脚本文件：

1. 各个段全部放置在 RAM 中，而 BL1 很多放在 ROM 内。不过二者的 RAM 权限都是`rwx`。
2. BL2 没有对 CPU 的操作。
3. 其他情况大致相同，只不过替换成 BL2 的相应的宏。

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

### 2-1-2 源码解析

```asm
mov  x20, x0
mov  x21, x1
mov  x22, x2
mov  x23, x3
```

保存来自 BL1 的寄存器 x0 ~ x3 的值，以便未来使用。

这里的寄存器 x0 ~ x3 的值是由 BL1 传递到 BL2 的之后阶段会用到的配置参数。比如：设备树信息、镜像加载地址、启动参数、启动模式、硬件特定配置等。

---

```asm
adr  x0, early_exceptions
msr  vbar_el1, x0
isb
```

`early_exceptions`：这是一个简单的无堆栈异常处理程序。BL2 和 BL31 阶段使用的非常简单的无堆栈异常处理程序。
BL31 在设置堆栈之前使用它。BL2 始终使用它。

`vbar_el1`：寄存器保存了 EL1 级别的异常向量表的基地址。这个基地址用于异常向量表的跳转。当 EL1 发生异常时，处理器会根据 vbar_el1 中存储的地址跳转到相应的异常处理程序。

`adr  x0, early_exceptions`：计算出`early_exceptions`无堆栈异常处理程序相对于当前 PC 的地址，然后存储到 x0 寄存器中。

`msr  vbar_el1, x0`：把通用寄存器 x0 中的值，存放到系统寄存器 vbar_el1 中。相当于把无堆栈异常处理程序的入口地址放到系统寄存器中，之后遇到异常就可以直接通过这个寄存器中的地址进行跳转。

`isb`：指令同步屏障指令，作用是确保 isb 指令之前的所有指令都已经执行完毕，并刷新处理器流水线。

---

```asm
msr  daifclr, #DAIF_ABT_BIT
```

`DAIF_ABT_BIT`：是 ARM 架构下的一个宏定义，用于表示异常屏蔽寄存器中的特定位，用以控制和查询数据异常(Abort)屏蔽(Mask)状态。

```plaintext  
DAIF 寄存器是一个程序状态寄存器(PSR)，包含以下四个异常屏蔽位:  
  D (Debug mask bit)：调试异常屏蔽位  
  A (SError mask bit)：同步错误异常屏蔽位  
  I (IRQ mask bit)：中断请求异常屏蔽位  
  F (FIQ mask bit)：快速中断请求异常屏蔽位  
这些位用于启用或屏蔽对应类型的异常。DAIF 寄存器在访问时可以屏蔽或使能这些异常，以控制系统的中断和异常行为。  
DAIF 寄存器的用法主要有以下几个方面：
1. 屏蔽某个异常：msr daifset #mask 来设置特定的异常屏蔽位。
2. 清除某个异常屏蔽：msr daifclr #mask 来清除特定的异常屏蔽位。

#mask是一个立即数，表示需要设置和清除的屏蔽位。如#(1 << 8)、#DAIF_ABT_BIT

3. 读取 DAIF 寄存器的值：mrs x0, daif
```

通过查看宏定义可以知道`#define DAIF_ABT_BIT   (U(1) << 2)`，因此这里相当于把`A (SError mask bit)：同步错误异常屏蔽位`的异常屏蔽清除。

---

```asm
mov  x1, #(SCTLR_I_BIT | SCTLR_A_BIT | SCTLR_SA_BIT)
mrs  x0, sctlr_el1
orr  x0, x0, x1
bic  x0, x0, #SCTLR_DSSBS_BIT
msr  sctlr_el1, x0
isb
```

宏定义如下：

```C
#define SCTLR_A_BIT          (ULL(1) << 1)  // 对齐检查使能（Alignment Check Enable）
#define SCTLR_SA_BIT         (ULL(1) << 3)  // 堆栈对齐检查使能（Stack Alignment Check Enable）
#define SCTLR_I_BIT          (ULL(1) << 12) // 指令缓存使能（Instruction Cache Enable）
#define SCTLR_DSSBS_BIT      (ULL(1) << 44) // Data Speculation Barrier
```

`mov  x1, #(SCTLR_I_BIT | SCTLR_A_BIT | SCTLR_SA_BIT)`指令相当于把系统控制寄存器的对齐检查使能、堆栈对齐检查使能、指令缓存使能三个功能打开，放到通用寄存器 x1 中，以便后续操作。

`mrs  x0, sctlr_el1`：把系统控制寄存器 sctlr_el1 的值读到通用寄存器 x0 中，这样就可以对系统寄存器进行修改。

`orr  x0, x0, x1`：把 x0 寄存器和 x1 寄存器进行按位或操作，其意义是把系统控制寄存器的上述三个功能打开。

`bic  x0, x0, #SCTLR_DSSBS_BIT`：`bic`指令的作用是清除指定寄存器的特定位，这里的作用是删除 x0 寄存器的 SCTLR_DSSBS_BIT 这一位，其含义是关闭系统控制寄存器的数据推测障碍功能。

`msr  sctlr_el1, x0`：把修改后的系统控制寄存器的值放回对应的寄存器。

> 问： 为什么修改系统控制寄存器 sctlr_el1 需要像将其读到通用寄存器中修改，最后写入呢？为什么不能直接对这个寄存器进行操作呢？  
> 答：  
> 1.保留其他位的值：确保只修改需要修改的位，而不影响其他控制位的当前状态。  
> 2.确保一致性和原子性：防止直接修改寄存器可能带来的不一致或中断。  
> 3.访问控制：某些寄存器可能限制直接写入操作，通过先读取再修改的方法，可以确保合法访问。  

---

```asm
adr  x0, __RW_START__
adr  x1, __RW_END__
sub  x1, x1, x0
bl  inv_dcache_range
```

`adr  x0, __RW_START__`：把读写数据段的起始地址相对于 PC 的偏移地址读到寄存器 x0 中。

`adr  x1, __RW_END__`：把读写数据段的结束地址相对于 PC 的偏移地址读到寄存器 x1 中。

`sub  x1, x1, x0`：把读写数据段的结束地址和起始地址做差，得到读写数据段的大小，放到寄存器 x1 中。

`bl  inv_dcache_range`：跳转执行函数 inv_dcache_range ，通过查看源码得知，这个函数的作用是将 x0 作为基地址，x1 作为大小的内存区域失效。

> 注：  
> 使 BL2 映像使用的 RW 内存无效。这包括数据和 NOBITS 部分。这样做是为了防止由于早期引导加载程序阶段使用而导致系统缓存中的脏缓存行损坏此内存。

---

```asm
adrp  x0, __BSS_START__
add  x0, x0, :lo12:__BSS_START__
adrp  x1, __BSS_END__
add  x1, x1, :lo12:__BSS_END__
sub  x1, x1, x0
bl  zeromem

adrp  x0, __COHERENT_RAM_START__
add  x0, x0, :lo12:__COHERENT_RAM_START__
adrp  x1, __COHERENT_RAM_END_UNALIGNED__
add  x1, x1, :lo12:__COHERENT_RAM_END_UNALIGNED__
sub  x1, x1, x0
bl  zeromem
#endif
```

第一部分代码，将`.bss`段的起始地址放到 x0 寄存器中，将`.bss`段的大小放到 x1 寄存器中。然后清空内存。

第二部分代码，将一致内存段的起始地址放到 x0 寄存器中，将一致内存段的大小放到 x1 寄存器中。然后清空之前的一致内存段。

> 注：  
> 使 BL2 映像使用的 RW 内存无效。这包括数据和 NOBITS 部分。这样做是为了防止由于早期引导加载程序阶段使用而导致系统缓存中的脏缓存行损坏此内存。  
> 将 NOBITS 部分清零。其中有 2 个：.bss 部分、一致内存部分。

---

```asm
bl  plat_set_my_stack
```

跳转执行函数`plat_set_my_stack`，这个函数设置了一个指向常规内存下分配的栈的栈指针。

---

```asm
#if STACK_PROTECTOR_ENABLED
  bl  update_stack_protector_canary
#endif
```

如果启用了栈保护器那么就跳转执行函数`update_stack_protector_canary`，这个函数会更改用于堆栈破坏攻击保护的金丝雀的值。

> 注：
> 必须在可以安全调用 C 代码时调用此方法，但不能由 C 代码调用。这样做会导致调用函数返回时检查失败。

---

```asm
mov  x0, x20
mov  x1, x21
mov  x2, x22
mov  x3, x23
bl  bl2_setup
```

恢复第一部分保存的现场，用于 BL2 启动阶段的设置。

---

```asm
#if ENABLE_PAUTH
  bl  pauth_init_enable_el1
#endif /* ENABLE_PAUTH */
```

启用和初始化 EL1 级别的指针认证。

---

```asm
bl  bl2_main

no_ret  plat_panic_handler
```

`bl2_main`：bl2 的唯一作用就是加载之后的图像，并将控制权移交给下一个要执行的 bl。 bl2 占用的内存将在 BL3x 阶段回收。并且 bl2 仅在 EL1 级别执行。

`no_ret`：表示这个函数不用返回。

`plat_panic_handler`：是平台特定的恐慌处理程序，当系统遇到无法恢复的致命错误时，会执行。

## 2-3 plat_set_my_stack 源码解析

这个函数在两个文件中定义分别是`plat/common/aarch64/platform_mp_stack.S`和`plat/common/aarch64/platform_up_stack.S`。  
这两个文件通常是用于初始化和管理嵌入式系统多处理器(MP,Multi-Processor)和单处理器(UP,Uniprocessor)环境下的栈指针和栈内存。  

### 2-3-1 源代码

<details>
  <summary>点击这里展开/收起代码块</summary>

```asm
/*platform_mp_stack.S*/
func plat_set_my_stack
  mov  x9, x30 // lr
  bl   plat_get_my_stack
  mov  sp, x0
  ret  x9
endfunc plat_set_my_stack
```

```asm
/*platform_up_stack.S*/
func plat_set_my_stack
  get_up_stack platform_normal_stacks, PLATFORM_STACK_SIZE
  mov sp, x0
  ret
endfunc plat_set_my_stack
```

</details>

### 2-3-2 源码分析

**对于多处理器环境**：对于当前 CPU，此函数将堆栈指针设置为在普通内存中分配的堆栈。

`mov x9, x30`：把寄存器 x30 中的值传到 x9 中。由于这个函数中要通过`bl`指令调用另一个函数，而`bl`指令会修改寄存器 x30 的值，因此为了可以顺利返回到上层调用函数，需要保存现场。

> 注：  
> 在 ARM64 架构中，寄存器 x30 通常被称为链接寄存器(Link Register, LR)，当一个函数调用另一个函数是，调用者的返回地址会存储在寄存器 x30 中，以便函数执行完毕之后返回执行。**一般`bl`、`blr`、`ret`三个指令会隐式的修改寄存器 x30 的值，需要特别注意。**

`bl   plat_get_my_stack`：跳转执行函数`plat_get_my_stack`，这个函数对于当前 CPU，此函数返回在设备内存中分配的堆栈的堆栈指针。

> 问：这里提到了设备内存(device memory)，它和前面提到的普通内存(normal memory)有什么差别？  
> 答：[不同的内存类型](./Extra_knowledge/Arm_memory_type.md)

`mov  sp, x0`：函数`plat_get_my_stack`的返回值（新栈的指针）被存放在了 x0 寄存器中，这行指令将新栈指针赋值给指针寄存器 sp。

`ret  x9`：使用寄存器 x9 中保存的返回地址返回，返回上层调用函数。

**对于单处理器环境**：对于冷启动 BL 映像，只有主 CPU 需要堆栈。此函数将堆栈指针设置为在普通内存中分配的堆栈。

`get_up_stack platform_normal_stacks, PLATFORM_STACK_SIZE`：`get_up_stack`是一个宏，后面紧跟栈的基地址或者栈区段的标识符(platform_normal_stacks)和栈的大小(PLATFORM_STACK_SIZE)。`get_up_stack`函数的作用是计算出新的栈顶地址，并把结果存在寄存器 x0 中。

`mov  sp, x0`：把新栈指针赋值给指针寄存器 sp。

`ret`：返回上层调用函数，这里默认使用寄存器 x30 中存储的地址

## 2-4 update_stack_protector_canary 源码解析

当启用了栈保护机制，那么就要更新栈保护金丝雀。

### 2-4-1 源代码

<details>
  <summary>点击这里展开/收起代码块</summary>

```asm
func update_stack_protector_canary

  mov  x19, x30
  bl  plat_get_stack_protector_canary

  adrp  x1,  __stack_chk_guard
  str  x0, [x1, #:lo12:__stack_chk_guard]
  ret  x19
endfunc update_stack_protector_canary
```

</details>

### 2-4-2 源码分析

`mov  x19, x30`：下面的`bl`指令会变更链接寄存器 x30 的值，因此需要保护现场。

`bl plat_get_stack_protector_canary`：这个函数会生成一个随机数，并把返回值存放到寄存器 x0 中。

> 注：  
> 栈保护器是一种安全机制，用于检测和防止栈溢出攻击。在编译器生成的代码中，canary 值通常被插入到栈中，并在函数返回前进行检查。如果 canary 值被修改，则表示可能发生了栈溢出攻击，程序会采取相应的安全措施。plat_get_stack_protector_canary 函数通常在程序启动时或在某些关键时刻调用，以生成新的 canary 值。生成的 canary 值会被插入到栈中，并在函数返回前进行检查。通过这种方式，可以检测到栈溢出攻击并采取相应措施。  

`adrp  x1,  __stack_chk_guard`：这个指令获得`__stack_chk_guard`的基地址。

`str  x0, [x1, #:lo12:__stack_chk_guard]`：`[x1,#:lo12:__stack_chk_guard]`表示将 x1 寄存器中的基地址加上`__stack_chk_guard`的低十二位地址。`str`指令将寄存器 x0 中的数据存储到后面获得的地址中。

`ret  x19`：按照 x19 中的地址返回上层调用函数。

## 2-5 bl2_setup 源码解析

### 2-5-1 源代码

<details>
  <summary>点击这里展开/收起代码块</summary>

```C
void bl2_setup(u_register_t arg0, u_register_t arg1, u_register_t arg2,
         u_register_t arg3)
{
  bl2_early_platform_setup2(arg0, arg1, arg2, arg3);

  bl2_plat_arch_setup();

#if CTX_INCLUDE_PAUTH_REGS
  assert(is_armv8_3_pauth_present());
#endif /* CTX_INCLUDE_PAUTH_REGS */
}
```

</details>

### 2-5-2 源码分析

`bl2_early_platform_setup2(arg0, arg1, arg2, arg3);`：这个函数初始化了 BL2 的内存布局、初始化了控制台来提供早期的 debug 支持。

`bl2_plat_arch_setup();`：快速而粗糙的初始化 mmu，以完成早期平台特定架构设置。

`#if CTX_INCLUDE_PAUTH_REGS`：当定义了这个宏，表示需要处理指针认证寄存器。

`assert(is_armv8_3_pauth_present());`：作用是检查当前系统是否支持 ARMv8.3 的指针认证功能。这个函数通常会检查处理器的特性寄存器或其他硬件特性，以确定是否支持指针认证。

## 2-6 bl2_main 源码解析

### 2-6-1 源代码

bl2 的唯一作用就是加载之后的图像，并将控制权移交给下一个要执行的 bl。 bl2 占用的内存将在 BL3x 阶段回收。并且 bl2 仅在 EL1 级别执行。

<details>
    <summary>点击这里展开/收起代码</summary>

```C
void bl2_main(void)
{
  entry_point_info_t *next_bl_ep_info;

  NOTICE("BL2: %s\n", version_string);
  NOTICE("BL2: %s\n", build_message);

  bl2_arch_setup();

#if TRUSTED_BOARD_BOOT
  auth_mod_init();
#endif /* TRUSTED_BOARD_BOOT */

  bl2_plat_preload_setup();

  next_bl_ep_info = bl2_load_images();

#if !BL2_AT_EL3
#ifndef __aarch64__

  disable_mmu_icache_secure();
#endif /* !__aarch64__ */

  console_flush();

#if ENABLE_PAUTH
  pauth_disable_el1();
#endif /* ENABLE_PAUTH */

  smc(BL1_SMC_RUN_IMAGE, (unsigned long)next_bl_ep_info, 0, 0, 0, 0, 0, 0);
#else /* if BL2_AT_EL3 */
  NOTICE("BL2: Booting " NEXT_IMAGE "\n");
  print_entry_point_info(next_bl_ep_info);
  console_flush();

#if ENABLE_PAUTH
  pauth_disable_el3();
#endif /* ENABLE_PAUTH */

  bl2_run_next_image(next_bl_ep_info);
#endif /* BL2_AT_EL3 */
}

```

</details>

### 2-6-2 源码分析

```C
entry_point_info_t *next_bl_ep_info;

NOTICE("BL2: %s\n", version_string);
NOTICE("BL2: %s\n", build_message);

bl2_arch_setup();
```

`entry_point_info_t *next_bl_ep_info;`：定义一个变量用于存储下一阶段引导加载程序的入口点信息。

`NOTICE`：是一个封装了标准输出功能的宏，用于在控制台或者日志文件中打印通知级别的日志信息。

`version_string`和`build_message`：分别代表 BL2 阶段引导程序的版本信息和构建信息。

`bl2_arch_setup();`：这个函数目前只是一个占位符，其实 BL2 对于平台没有什么需要初始化。

---

```C
#if TRUSTED_BOARD_BOOT
  auth_mod_init();
#endif /* TRUSTED_BOARD_BOOT */
```

`#if TRUSTED_BOARD_BOOT`：启用可信引导时编译下面呢代码。

`auth_mod_init();`：用于根据系统的生命周期状态（LCS）和引导模式初始化认证模块。在启用 RCAR 安全引导时，它会读取相关寄存器值，设置引导模式，并初始化相应的引导验证函数指针。在未启用安全引导时，它会默认设置为正常引导模式。通过这种方式，可以确保系统在引导过程中根据不同的安全需求进行相应的初始化和配置。

---

```C
bl2_plat_preload_setup();
next_bl_ep_info = bl2_load_images();
```

`bl2_plat_preload_setup();`：这个函数是为下一阶段的加载过程初始化设置。

`next_bl_ep_info = bl2_load_images();`：将下一个阶段的镜像加载到定义的变量中。

---

```C
#if !BL2_AT_EL3
...
#else /* if BL2_AT_EL3 */
...
#endif /* BL2_AT_EL3 */
```

`BL2_AT_EL3`：这个宏用来控制 BL2 阶段是否在 EL3 级别执行。

---

```C
#ifndef __aarch64__
  disable_mmu_icache_secure();
#endif /* !__aarch64__ */
```

当架构是 ARM64 架构时，调用`disable_mmu_icache_secure();`函数禁用 mmu 和指令缓存。

---

```C
console_flush();
```

`console_flush();`：函数用于刷新控制台输出缓冲区，确保待输出的内容会及时显示在控制台上。

---

```C
#if ENABLE_PAUTH
  pauth_disable_el1();
#endif /* ENABLE_PAUTH */
```

这里表示 BL2 基本执行完毕，如果开启了 EL1 指针认证，那么就关闭。

---

```C
smc(BL1_SMC_RUN_IMAGE, (unsigned long)next_bl_ep_info, 0, 0, 0, 0, 0, 0);
```

`smc`：这个函数表示在不同的执行环境中进行切换，可以是从安全世界到非安全世界，也可以是从一个引导镜像到另一个引导镜像。这里的切换时后者。

`BL1_SMC_RUN_IMAGE`：对应 BL1 异常处理服务执行 BL31 的 SMC 的功能 ID。这里可以这样理解，在 BL1 的最后，代码是通过异常处理的方式进入  BL2 的因此，这里调用 BL1 异常处理服务来执行 BL31

`(unsigned long)next_bl_ep_info`：指向下一个入口信息的结构体指针，结合上下文可知这里存的时 BL31 的入口信息。

`0, 0, 0, 0, 0, 0`：这里表示其他的参数都是0，根据其源代码可知就是不需要其他参数。

---

**从这里开始就是 ELSE 的部分了，表示 BL2 在 EL3 级别运行**  

```C
NOTICE("BL2: Booting " NEXT_IMAGE "\n");
print_entry_point_info(next_bl_ep_info);
console_flush();
```

`NOTICE("BL2: Booting " NEXT_IMAGE "\n");`：打印说明正在运行下一个镜像。

`print_entry_point_info(next_bl_ep_info);`：打印出下一个镜像的入口点信息。

`console_flush();`：函数用于刷新控制台输出缓冲区，确保待输出的内容会及时显示在控制台上。

---

```C
#if ENABLE_PAUTH
  pauth_disable_el3();
#endif /* ENABLE_PAUTH */
```

如果启用了 EL3 指针认证，那么关闭。因为执行到这里 BL2 已经彻底结束了，已经准备开始跑下一个镜像了。

---

```C
bl2_run_next_image(next_bl_ep_info);
```

运行下一个要运行的镜像。

## 2-7 plat_panic_handler 源码解析

这个是系统遇到致命错误时，会执行的程序，不同的平台可以根据需求自行编写，默认实现只是让机器处于低功耗模式，然后无限循环。

下面是默认代码的一些解释。

```plainttext
当通用代码遇到无法继续的错误情况时，将调用此 API。它允许平台执行错误报告或恢复操作（例如，重置系统）。此函数不得返回。

该参数使用来自“errno.h”的标准代码指示错误类型。

通用代码报告的可能错误包括：

-“-EAUTH”：无法验证证书或映像（启用 Trusted Board Boot 时）

-“-ENOENT”：找不到请求的映像或证书或检测到 IO 错误

-“-ENOMEM”：资源耗尽。TF-A 不使用动态内存，因此此错误通常表示数组大小不正确

默认实现只是让机器处于低功耗模式，然后无限循环。
```
