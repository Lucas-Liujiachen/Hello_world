<!-- markdownlint-disable MD033 -->

# ATF启动流程一：bl1

[back](./ATF.md)

系统上电后首先会运行 SCP boot ROM，之后会跳转到 ATF 的 bl1 中继续执行。bl1 主要初始化 CPU，设定异常向量，将 bl2 的 image 加载到安全 RAM 中，然后跳转到 bl2 中进行执行。
bl1 的主要代码存放在 bl1 目录中，bl1 的链接脚本是 bl1/bl1.ld.S 文件。其中 bl1 的入口函数是`bl1_entrypoint`。
bl1/bl1.ld.S 文件制定了内存中各个段（如.data 、.text 、.bss 等）存放的位置和属性。

## 1.1 bl1_entrypoint 源码分析

这个函数主要进行EL3执行环境的基本初始化、设置向量表，并加载 bl2 的 image 到对应的位置并跳转到 bl2 执行。

### 1-1-1 源代码

<details>
  <summary>点击这里展开/收起代码块</summary>

```asm
   .globl   bl1_entrypoint
   
func bl1_entrypoint
   
   el3_entrypoint_common               \
      _init_sctlr=1               \
      _warm_boot_mailbox=!PROGRAMMABLE_RESET_ADDRESS   \
      _secondary_cold_boot=!COLD_BOOT_SINGLE_CPU   \
      _init_memory=1               \
      _init_c_runtime=1            \
      _exception_vectors=bl1_exceptions
  
   bl   bl1_setup

#if ENABLE_PAUTH
   bl   pauth_init_enable_el3
#endif /* ENABLE_PAUTH */
  
   bl   bl1_main

#if ENABLE_PAUTH
   bl   pauth_disable_el3
#endif /* ENABLE_PAUTH */

   b   el3_exit
endfunc bl1_entrypoint
```

</details>

### 1-1-2 func bl1_entrypoint

```asm
func bl1_entrypoint
...
endfunc bl1_entrypoint
```

`func bl1_entrypoint`是一个声明语句，声明这个函数从此开始。
`endfunc bl1_entrypoint`表示函数到此结束。
`bl1_entrypoint()`函数是当 CPU 从热复位或者冷复位中释放时，进入可信固件代码的入口点。其中热复位为 reset、冷复位为 power on。

### 1-1-3 el3_entrypoint_common 功能

```asm
el3_entrypoint_common               \
      _init_sctlr=1               \
      _warm_boot_mailbox=!PROGRAMMABLE_RESET_ADDRESS   \
      _secondary_cold_boot=!COLD_BOOT_SINGLE_CPU   \
      _init_memory=1               \
      _init_c_runtime=1            \
      _exception_vectors=bl1_exceptions
```

这里使用宏定义了一个名为`el3_entrypoint_common`的函数，主要用于 ARM 架构的 EL3 执行级别上进行系统的初始化和配置，在`include/arch/aarch64/el3_common_macros.S`文件中实现。

### 1-1-4 bl1_setup 功能

`bl bl1_setup`跳转执行`bl1_setup`函数，这个函数在`include/bl1/b1.h`中声明，在`bl1/bl1_main.c`中实现。其主要用于执行对应平台的初始化设置，从未为 bl1_main 的执行初始化环境。

### 1-1-5 pauth_init_enable_el3 功能

```asm
#if ENABLE_PAUTH
bl   pauth_init_enable_el3
#endif
```

这段代码会根据预定义的宏`ENABLE_PAUTH`的值来决定是否执行`pauth_init_enable_el3`功能（若值非零，则代码会被编译到最终程序中）。
`pauth_init_enable_el3`是一个宏定义的函数，在`lib/extensions/pauth/pauth_helper.S`中实现。主要作用是初始化和启用 EL3 级别的指针认证功能。它涉及到初始化平台密钥、设置指令密钥、启用指针认证以及根据条件编译选项启用PAC分支类型兼容性。

`#endif`表示条件编译的结束标志，它指示条件编译的代码块结束。在这个例子中，它表示如果`ENABLE_PAUTH`宏被定义，那么上面的代码块从`#if`到`#endif`之间的内容都将被编译。

### 1-1-6 bl1_main 功能

```asm
bl   bl1_main
```

`bl bl1_main`跳转执行`bl1_main`函数,这个函数在`include/bl1/b1.h`中声明，在`bl1/bl1_main.c`中实现。其主要功能为：执行后期架构和平台特定初始化的函数。它还会查询平台以加载和运行下一个 BL 映像。仅在冷启动后由主 CPU 调用。

### 1-1-7 pauth_disable_el3 功能

```asm
#if ENABLE_PAUTH
bl   pauth_disable_el3
#endif /* ENABLE_PAUTH */
```

这段代码会根据预定义的宏`ENABLE_PAUTH`的值来决定是否执行`pauth_disable_el3`功能（若值非零，则代码会被编译到最终程序中）。
`pauth_disable_el3`是一个宏定义函数，在`lib/extensions/pauth/pauth_helper.S`中实现。主要作用是禁用 EL3 级别的指针认证功能。

`#endif`表示条件编译的结束标志，它指示条件编译的代码块结束。在这个例子中，它表示如果`ENABLE_PAUTH`宏被定义，那么上面的代码块从`#if`到`#endif`之间的内容都将被编译。

### 1-1-8 el3_exit 功能

```asm
b   el3_exit
```

ARM 架构中从 EL3 执行级别中退出到更低的异常级别，并跳转到下一个镜像执行。

## 1.2 el3_entrypoint_common 源码解析

### 1-2-1 源代码

在`include/arch/aarch64/el3_common_macros.S`文件中实现。

这个函数以宏的形式被定义，主要完成 el3 的基本设置和向量表注册。
其中主要的参数为：

1. **_init_stclr**:宏是否需要初始化 SCTLR 寄存器，包括配置数据访问的字节顺序（设置大小端）。
2. **_warm_boot_mailbox**:宏是否需要检测启动类型（冷/热）。检测基于平台入口点地址：如果为零，则为冷启动，否则为热启动。在后一种情况下，此宏会跳转到平台入口点地址。（检测当前属于冷启动还是热启动（power on or reset））
3. **_secondary_cold_boot**:宏是否需要识别调用它的 CPU：主 CPU 还是辅助 CPU。主 CPU 将被允许继续进行平台初始化，而辅助 CPU 在此期间将处于特定于平台的状态。如果调用者知道此宏只会由主 CPU 调用，则可以将此参数定义为 0 以跳过此步骤。
4. **_init_memory**:是否需要初始化内存。
5. **_init_c_runtime**:是否需要初始化 C 语言程序的执行环境。
6. **_exception_vectors**:异常向量表地址。

<details>
  <summary>点击这里展开/收起代码块</summary>

```asm
      .macro el3_entrypoint_common               \
      _init_sctlr, _warm_boot_mailbox, _secondary_cold_boot,   \
      _init_memory, _init_c_runtime, _exception_vectors

   .if \_init_sctlr
      
      mov_imm   x0, (SCTLR_RESET_VAL & ~(SCTLR_EE_BIT | SCTLR_WXN_BIT \
            | SCTLR_SA_BIT | SCTLR_A_BIT | SCTLR_DSSBS_BIT))
      msr   sctlr_el3, x0
      isb
   .endif /* _init_sctlr */

   .if \_warm_boot_mailbox
     
      bl   plat_get_my_entrypoint
      cbz   x0, do_cold_boot
      br   x0

   do_cold_boot:
   .endif /* _warm_boot_mailbox */

   adr   x0, \_exception_vectors
   msr   vbar_el3, x0
   isb

   bl   reset_handler

   el3_arch_init_common

   .if \_secondary_cold_boot

      bl   plat_is_my_cpu_primary
      cbnz   w0, do_primary_cold_boot

      bl   plat_secondary_cold_boot_setup

      bl   el3_panic

   do_primary_cold_boot:
   .endif /* _secondary_cold_boot */

   .if \_init_memory
      bl   platform_mem_init
   .endif /* _init_memory */

   .if \_init_c_runtime
#if defined(IMAGE_BL31) || (defined(IMAGE_BL2) && BL2_AT_EL3 && BL2_INV_DCACHE)
      adrp   x0, __RW_START__
      add   x0, x0, :lo12:__RW_START__
      adrp   x1, __RW_END__
      add   x1, x1, :lo12:__RW_END__
      sub   x1, x1, x0
      bl   inv_dcache_range
#endif
      adrp   x0, __BSS_START__
      add   x0, x0, :lo12:__BSS_START__
      adrp   x1, __BSS_END__
      add   x1, x1, :lo12:__BSS_END__
      sub   x1, x1, x0
      bl   zeromem

#if USE_COHERENT_MEM
      adrp   x0, __COHERENT_RAM_START__
      add   x0, x0, :lo12:__COHERENT_RAM_START__
      adrp   x1, __COHERENT_RAM_END_UNALIGNED__
      add   x1, x1, :lo12: __COHERENT_RAM_END_UNALIGNED__
      sub   x1, x1, x0
      bl   zeromem
#endif

#if defined(IMAGE_BL1) || (defined(IMAGE_BL2) && BL2_AT_EL3 && BL2_IN_XIP_MEM)
      adrp   x0, __DATA_RAM_START__
      add   x0, x0, :lo12:__DATA_RAM_START__
      adrp   x1, __DATA_ROM_START__
      add   x1, x1, :lo12:__DATA_ROM_START__
      adrp   x2, __DATA_RAM_END__
      add   x2, x2, :lo12:__DATA_RAM_END__
      sub   x2, x2, x0
      bl   memcpy16
#endif
   .endif /* _init_c_runtime */

   msr   spsel, #0
   bl   plat_set_my_stack

#if STACK_PROTECTOR_ENABLED
   .if \_init_c_runtime
   bl   update_stack_protector_canary
   .endif /* _init_c_runtime */
#endif
   .endm
```

</details>

### 1-2-2 源码分析

```asm
.macro el3_entrypoint_common                    \
        _init_sctlr, _warm_boot_mailbox, _secondary_cold_boot,  \
        _init_memory, _init_c_runtime, _exception_vectors
.endm
```

`.macro`表示这是一个宏定义，`el3_entrypoint_common`表示函数名，后面的为宏的参数列表，包含了六个参数。参数含义见1-2-1。
`.endm`表示这个宏定义结束。
>注：代码里的 '\\' ，由于存在于宏定义中，其作用时将宏定义延续到下一行。这种技术被称为换行符续行，允许一个较长的宏定义分成多行，提高代码的可读性和可维护性。

---

```asm
.if \_init_sctlr
      
      mov_imm   x0, (SCTLR_RESET_VAL & ~(SCTLR_EE_BIT | SCTLR_WXN_BIT \
            | SCTLR_SA_BIT | SCTLR_A_BIT | SCTLR_DSSBS_BIT))
      msr   sctlr_el3, x0
      isb
   .endif /* _init_sctlr */
```

>注：在 Linux 的 ARM 汇编中规定：使用 .if 伪指令时，后面的参数前需要加上 '\\'。

这是 SCTLR_EL3 的初始化，因此必须确保所有字段都明确设置，而不是依赖于硬件。
一些字段重置为实现定义的值，而其他字段在重置时在架构上是未知的。
SCTLR .EE：在执行任何可能涉及内存读取或写入的操作之前，设置 CPU 字节序。设置为零以选择 Little Endian（小端序）。
SCTLR_EL3.WXN：对于 EL3 转换机制，此字段可以强制将所有可写入的内存区域视为 XN（从不执行）。设置为零，以便此控制对内存访问权限没有影响。
SCTLR_EL3.SA：设置为零以禁用堆栈对齐检查。
SCTLR_EL3.A：设置为零以禁用对齐错误检查。
SCTLR.DSSBS：设置为零以在异常进入 EL3 时禁用推测存储绕过安全行为。

`mov_imm x0,...`：将一个立即数(imm)移动到寄存器 x0 中。

`SCTLR_RESET_VAL`：一个常量的值，表示 SCTLR 系统控制寄存器的复位值。`SCTLR`控制处理器的各种系统及功能，它的位字段的设置影响系统行为。

`& ~()`：将括号内的值按位取反，并与前面的值进行按位与操作。

`SCTLR_EE_BIT`：用于控制节序（大小端）。

`SCTLR_WXN_BIT`：用于控制写、执行的权限。Write、eXecute

`SCTLR_SA_BIT`：用于控制堆栈的对齐检查。

`SCTLR_A_BIT`：用于控制数据访问的对齐检查。

`SCTLR_DSSBS_BIT`：控制数据推测同步屏障。

>这条指令的目的是在`SCTLR`的复位值基础上，清除（设为 0）一些特定的控制位。这些控制位包括字节序、写执行权限、堆栈对齐检查、数据访问对齐检查以及数据推测同步屏障。通过这条指令，`x0`寄存器将被设置为一个新的控制值，用于配置系统控制寄存器`SCTLR`。

`msr (Move to System Register)`：该指令将一个通用寄存器的值写到一个特定的系统寄存器中。

`sctlr_el3`:表示第三异常级别(EL3)的系统控制寄存器(SCTLR)。用于配置处理器在 EL3 级别上的系统行为。它包含多个控制位，这些控制位可以启用或禁用处理器的某些功能，如缓存、对齐检查、指令和数据访问权限等。

`x0`：ARM 架构中的一个通用寄存器，里面存放着将要写入 sctlr_el3 寄存器的值。

`msr sctlr_el3, x0`：根据前面分析可知 x0 中存放了清楚了某些特定控制位的 SCTLR_RESET_VAL。这条指令就是将设置好的值存放到控制 SCTLR 的 sctlr_el3 系统寄存器中，以配置 EL3 级别的系统控制寄存器。

`isb`:该指令在 ARM 架构中用于同步指令流，确保在它之前执行的所有指令的效果在它之后的指令开始执行之前都已经生效了。它在系统寄存器更新和指令流同步中起关键作用，尤其是在设置控制寄存器（如scrlt_el3）之后，以确保新的设置立即生效。

---

```asm
.if \_warm_boot_mailbox
  
   bl   plat_get_my_entrypoint
   cbz   x0, do_cold_boot
   br   x0

do_cold_boot:
.endif /* _warm_boot_mailbox */
```

`bl plat_get_my_entrypoint`：执行函数 plat_get_my_entrypoint。在`plat/自己的平台对应的文件夹(如imx)/imx8_helpers.S`中实现。其功能为根据平台特定的寄存器标志来判断启动类型（冷启动或热启动）。如果是冷启动，函数返回 0。如果是热启动，函数从 mailbox_base 获取入口点地址并返回。这样做可以确保启动代码根据启动类型执行相应的初始化逻辑。函数的返回值放到 x0寄存器。

`cbz x0, do_cold_boot`：检查 x0 寄存器是否为零，如果 x0 的值为零，那么跳转到 do_cold_boot 标签对应的地方。结合上一步来看，上一步函数的返回值存放在了 x0 寄存器中，如果判断出来时冷启动，那么跳转到 do_cold_boot 标签的地方，进行冷启动，否则根据返回的热启动入口地址进行热启动。

`br x0`：无条件跳转到 x0 寄存器中存储的地址处继续执行。这里 x0 中存放的是热启动对应的入口地址。

---

```asm
adr   x0, \_exception_vectors
msr   vbar_el3, x0
isb

bl   reset_handler

el3_arch_init_common
```

`adr x0, \_exception_vectors`：指令`adr`是根据 PC 的偏移地址计算目标地址。这里可以理解成计算出 _exception_vectors 的地址，然后存到寄存器 x0 中。

`msr`：该指令将一个通用寄存器的值写到一个特定的系统寄存器中。

`vbar_el3`：ARM 架构下的系统寄存器，它是用来存放 EL3 的异常向量表的地址。其中应该包含了处理器遇到异常时应该跳转程序的地址。

`msr   vbar_el3, x0`:这行指令将通用寄存器 x0 中的值，放到了 vbar_el3 系统寄存器中。从上面可以知道，计算出的异常向量表的地址存放到了 x0 中，因此这一步把异常向量表的地址放到系统寄存器 vbar_el3 下，从而指定 EL3 级别异常时应跳转的程序的地址。

`isb`：这条指令用于同步指令流，确保它之前的指令的执行效果在它之后的指令执行之前就已经生效了。这里是为了确保上面指令更改存储异常向量表地址的系统寄存器 vbar_el3 后，设置生效。

`bl   reset_handler`：执行函数 reset_handler。这个函数在`lib/cpus/aarch64/cpu_helpers.S`中实现。调用平台特定复位函数的`plat_reset_handler`函数、获取并验证`cpu_ops`结构体指针、根据`cpu_ops`中的复位处理函数执行相应的操作、保护和恢复必要的寄存器状态，以确保后续流程的正常进行。

`el3_arch_init_common`：一个宏定义函数用于初始化 EL3 异常级别的系统控制状态和寄存器。根据上文可知，这是一个冷启动情况，因此在初始化一些寄存器。

---

```asm
.if \_secondary_cold_boot

bl   plat_is_my_cpu_primary
cbnz   w0, do_primary_cold_boot

bl   plat_secondary_cold_boot_setup

bl   el3_panic
do_primary_cold_boot:
.endif /* _secondary_cold_boot */
```

`bl plat_is_my_cpu_primary`：判断当前 CPU 是不是主 CPU，并把结果放到 w0 中返回。

`cbnz w0, do_primary_cold_boot`：如果 w0 不是零，那么就跳转到 do_primary_cold_boot 标签那里执行。

`bl plat_secondary_cold_boot_setup`：如果执行到这里，说明当前 CPU 不是主 CPU，而是从属 CPU，那么应该执行从属 CPU 的冷启动流程。

`bl el3_panic`：函数在`common/aarch64/debug.S`中实现，通过阅读源码可知，当执行到上面一条语句`bl plat_secondary_cold_boot_setup`后，`plat_secondary_cold_boot_setup()`函数并不会返回，如果返回了，那么说明程序运行出问题了，应该报错并停止。

---

```asm
.if \_init_memory
    bl   platform_mem_init
.endif /* _init_memory */
```

`bl platform_mem_init`：函数在`plat/imx/common/imx8_helpers.S`中实现。一般可以直接ret，不用进行任何内存初始化操作，因为 Secure RAM 是可以直接访问的。

---
**接下来是C语言执行环境的初始化**  

```asm
#if defined(IMAGE_BL31) || (defined(IMAGE_BL2) && BL2_AT_EL3 && BL2_INV_DCACHE)
   adrp   x0, __RW_START__
   add   x0, x0, :lo12:__RW_START__
   adrp   x1, __RW_END__
   add   x1, x1, :lo12:__RW_END__
   sub   x1, x1, x0
   bl   inv_dcache_range
#endif
```

`#if defined(IMAGE_BL31) || (defined(IMAGE_BL2) && BL2_AT_EL3 && BL2_INV_DCACHE)`：条件编译选项。语句结构：当宏定义 IMAGE_BL31 被定义或者 defined(IMAGE_BL2) && BL2_AT_EL3 && BL2_INV_DCACHE 为真，则下面的代码编译。语句含义：当启动阶段为 bl31 时可以编译下方的代码；或者当处在 bl2 启动阶段时，并且 BL2_AT_EL3 和 BL2_INV_DCACHE 都被定义为真时，下列代码编译。

`BL2_AT_EL3`:BL2_AT_EL3 用于表示 bl2 引导加载程序是否将在 EL3 级别执行。这可能影响代码的行为，因为EL3有更高的特权，可以访问所有系统资源，并且在系统启动时常常用于初始化关键硬件和软件组件。

`BL2_INV_DCACHE`：BL2_INV_DCACHE 通常用于指示是否需要在BL2阶段执行数据缓存（D-Cache）失效操作。在嵌入式系统和低级引导代码中，确保数据一致性和系统稳定性是非常重要的。数据缓存失效操作是为了确保在缓存中的数据和主存中的数据一致，以避免数据不一致导致的错误。定义 BL2_INV_DCACHE 表示在BL2阶段需要进行这样的缓存失效操作。

`adrp`：adrp 指令用于计算目标地址的页基地址（忽略页内偏移量），并将其加载到目标寄存器中。页基地址是将目标地址对齐到 4KB 边界得到的地址。

`adrp   x0, __RW_START__`：`__RW_START__` 代表一个可读写段的起始地址。这里这个地址的页基地址存储到 x0 寄存器中。

`add   x0, x0, :lo12:__RW_START__`：这条指令和上面一条指令是一个常见的组合指令，用于获得完整地址。`:lo12:__RW_START__`这是一个特殊的操作数，表示符号`__RW_START__`的低12位偏移量。上面一条指令获得了页基地址，这条指令获得页内偏移量。这样寄存器 x0 中就存储了`__RW_START__`所代表的完整地址。
> 问：为什么要用两条指令完整地址的赋值，而不是直接赋值？
> 答：
> 1.在 ARM64 汇编中，mov 指令能处理的立即数位数为符合特定模式的16位数和她的移位版本，不能加载任意的64位数。
> 2.在 ARM64 指令集设计中，指令的长度固定是32位，因此一条指令无法直接包含和处理64位的立即数。
> 3.这种组合方式是一种优化，让长地址加载可以在两条指令内完成。
> 其他方式：
> 1.常量池：将常量池中的64位地址存到内存中，然后就可以通过加载指令（如`ldr`）就可以将其直接加载到寄存器中。
> 2.分段加载：将64位地址分成4个16位地址然后进行加载。但是这样就要用到4条指令，不如这种组合方式效率高。

`adrp   x1, __RW_END__`：类似上面两条指令的组合，这条指令将`__RW_END__`的页基地址加载到 x1 寄存器中。

`add   x1, x1, :lo12:__RW_END__`：这条指令将`__RW_END__`的低12位偏移量加载到 x1 寄存器中。

`sub   x1, x1, x0`：计算`__RW_START__`和`__RW_END__`两个符号之间的地址偏移量，也就是读写段的大小，并存在寄存器 x1 中。方便之后进行有效的内存管理、数据访问和计算数据结构大小等操作。

`bl   inv_dcache_range`：跳转到 inv_dcache_range 函数中执行。inv_dcache_range 函数将使得从基地址到指定大小的地址失效，其中 x0 为基地址，x1 为地址大小。
> 问：为什么这里要将这段地址的内容失效？
> 答：最后执行 inv_dcache_range 的目的是为了确保在处理器多核系统中，所有核心对于`__RW_START__`和`__RW_END__`之间的数据访问都是一致和正确的。通过失效数据缓存，可以强制处理器重新加载这段内存区域的最新数据，避免了可能因为缓存中旧数据而引发的问题。这种操作在操作系统内核初始化或者关键数据更新时特别重要，能够确保系统的稳定性和正确性。特别是在操作系统内核初始化阶段，或者在某些硬件操作（例如初始化设备寄存器）后，确保所有核心看到的内存数据都是最新的非常重要。这样可以避免因为旧数据造成的问题，比如设备驱动的不正确行为或者数据的一致性问题。

```asm
adrp   x0, __BSS_START__
add   x0, x0, :lo12:__BSS_START__
adrp   x1, __BSS_END__
add   x1, x1, :lo12:__BSS_END__
sub   x1, x1, x0
bl   zeromem
```

与上方类似，通过两对指令集合获得`__BSS_START__`和`__BSS_END__`对应的完整的地址。并将其基地址和地址大小分别存放在 x0 寄存器和 x1 寄存器中。BSS 段通常用于存放程序中未初始化或者显式初始化为零的全局变量。这些变量在程序运行之前被分配了固定大小的空间，但是它们的初始值是未定义的。这种段在程序加载时会被初始化为零或者空值，因此通常称为 "Block Started by Symbol" 段。  
最后跳转到 zeromem 函数中。zeromem 函数用于将 x0 作为基地址，x1 大小的地址内对应的内存清空。
> 问：为什么要清空内存？
> 答：在程序启动或初始化过程中，通过将 BSS 段的内存区域清零，确保了所有未初始化的全局变量和静态分配的空间的初始状态是一致的，避免了未知的数据内容对程序运行造成影响。

```asm
#if USE_COHERENT_MEM
   adrp   x0, __COHERENT_RAM_START__
   add   x0, x0, :lo12:__COHERENT_RAM_START__
   adrp   x1, __COHERENT_RAM_END_UNALIGNED__
   add   x1, x1, :lo12: __COHERENT_RAM_END_UNALIGNED__
   sub   x1, x1, x0
   bl   zeromem
#endif
```

`#if USE_COHERENT_MEM`：启用一直内存(Coherent Memory)，是为了在不同核心中共享内存，确保数据的一致性和可靠性。

后面的代码表示清空一致性内存的那部分地址对应的内存的内容。

```asm
#if defined(IMAGE_BL1) || (defined(IMAGE_BL2) && BL2_AT_EL3 && BL2_IN_XIP_MEM)
   adrp   x0, __DATA_RAM_START__
   add   x0, x0, :lo12:__DATA_RAM_START__
   adrp   x1, __DATA_ROM_START__
   add   x1, x1, :lo12:__DATA_ROM_START__
   adrp   x2, __DATA_RAM_END__
   add   x2, x2, :lo12:__DATA_RAM_END__
   sub   x2, x2, x0
   bl   memcpy16
#endif
```

`#if defined(IMAGE_BL1) || (defined(IMAGE_BL2) && BL2_AT_EL3 && BL2_IN_XIP_MEM)`：表示当启动阶段为 bl1时，或者如果启动阶段为 bl2 。并且 bl2 启动阶段以 EL3 级别执行，并且程序处在XIP存储器中时，下列代码将编译。

> * XIP 是一种特殊的存储器配置，允许处理器直接从存储器中执行代码，而无需将其加载到内部 RAM 或缓存中。这种方式可以节省内存空间，提高系统的性能和效率，特别是对于嵌入式系统和资源有限的设备而言。
> * 如果 BL2_IN_XIP_MEM 被定义，那么代码（如 Boot Loader 第二阶段）被设计为直接从 XIP 存储器执行。这意味着代码本身不会被复制到 RAM 或者缓存中执行，而是直接在存储器中执行。

接下来代码将`__DATA_RAM_START__`的地址放到 x0 寄存器中，将`__DATA_ROM_START__`的地址放到 x1 寄存器中，将`__DATA_RAM_END__`的地址放到 x2 寄存器中。

最后跳转到 memcpy16 函数，这个函数将把 ROM 的数据复制到 RAM 中。这种操作通常在引导加载过程或者程序初始化阶段用于将预定义的数据（存在 ROM 中）加载到可执行的内存区域（一般为 RAM）中。

```asm
msr   spsel, #0
bl   plat_set_my_stack
```

`spsel`寄存器：用于选择当前模式下的栈指针(Stack Pointer)。在 ARMv8 架构中，定义如下：

* Bit 0 控制当前模式的栈指针选择：
  * 0：使用主栈指针（Main Stack Pointer，MSP）
  * 1：使用进程栈指针（Process Stack Pointer，PSP）

`msr spsel, #0`：指令将值 0（即二进制的 0）写入到 spsel 寄存器的第 0 位。这意味着，该指令设置当前模式使用主栈指针（MSP）

`bl plat_set_my_stack`：跳转执行 plat_set_my_stack 函数。这个函数将为当前任务设置合适的栈空间。

```asm
#if STACK_PROTECTOR_ENABLED
   .if \_init_c_runtime
   bl   update_stack_protector_canary
   .endif /* _init_c_runtime */
#endif
```

`#if STACK_PROTECTOR_ENABLED`：判断是否开启栈保护器。栈保护器（Stack Protector）是一种用于检测和防止栈溢出攻击的技术。当 STACK_PROTECTOR_ENABLED 被定义时，编译器会插入额外的代码来保护函数的返回地址和栈帧信息，以防止恶意程序修改或篡改这些信息。

`update_stack_protector_canary`：作用是更新或者设置栈保护器的标志值（canary），用于检测栈溢出攻击。

## 1.3 bl1_setup 解析

函数实现在`/bl1/bl1_main.c`中，主要执行两个函数`bl1_early_platform_setup()`和`bl1_plat_arch_setup()`。
`bl1_setup`函数主要是为了后面`bl1_main`的执行初始化。

`bl1_early_platform_setup()`：进行平台特定的初始化设置。初始化控制台以提供早期调试支持，并设置允许 bl1访问所有的可信任 RAM。

`bl1_plat_arch_setup()`：执行 ARM 平台之间共享的早期平台特定设置。

## 1.4 pauth_init_enable_el3 源码解析

`pauth_init_enable_el3`是启用 EL3 指针认证。

<details>
  <summary>点击这里展开/收起代码块</summary>

```asm
func pauth_init_enable_el3
   stp   x29, x30, [sp], #-16!

   /* Initialize platform key */
   bl   plat_init_apkey

   /* Program instruction key A used by the Trusted Firmware */
   msr   APIAKeyLo_EL1, x0
   msr   APIAKeyHi_EL1, x1

   /* Enable pointer authentication */
   mrs   x0, sctlr_el3
   orr   x0, x0, #SCTLR_EnIA_BIT

#if ENABLE_BTI
    /* Enable PAC branch type compatibility */
   bic   x0, x0, #SCTLR_BT_BIT
#endif
   msr   sctlr_el3, x0
   isb

   ldp   x29, x30, [sp], #16
   ret
endfunc pauth_init_enable_el3
```

</details>

`stp   x29, x30, [sp], #-16!`：将 x29 和 x30 两个寄存器的值存到内存中，存储的目的地是栈顶（SP 寄存器当前指向的位置），存储后，栈指针 SP 向下偏移16个字节，指向新的栈顶。

`bl   plat_init_apkey`：跳转执行 plat_init_apkey 函数。这个函数返回可用于编程 ARMv8.3 指针认证密钥的 128 位值。该值应从可靠的随机源获取。仅当在受信任固件中使用 ARMv8.3 指针认证且将 BRANCH_PROTECTION 选项设置为非零时，才需要此函数。

`msr   APIAKeyLo_EL1, x0`：`APIAKeyLo_EL1`是指针认证密钥A的低位部分，把 x0 中的值存到这个目标系统寄存器。

`msr   APIAKeyHi_EL1, x1`：`APIAKeyHi_EL1`是指针认证密钥A的高位部分，把 x1 中的值存到这个目标系统寄存器。

`mrs   x0, sctlr_el3`：把表示第三异常级别(EL3)的系统控制寄存器(SCTLR)的信息存储到 x0 寄存器中，方便后续进行修改。

`orr   x0, x0, #SCTLR_EnIA_BIT`：#SCTLR_EnIA_BIT表示SCTLR寄存器中表示开启指针认证的开关位，将它与存放控制器信息的 x0 寄存器进行按位操作，就是设置指针认真开关对应位的值。

`msr   sctlr_el3, x0`：把新得到的系统控制寄存器的值放回生效的寄存器。

`isb`：确保上面对系统的更改能够生效。

`ldp   x29, x30, [sp], #16`：这一步对应着`stp   x29, x30, [sp], #-16!`这个指令。这一步将前面存在栈顶的 x29 和 x30 两个寄存器的值读回寄存器，恢复现场。

## 1.5 bl1_main 源码解析

bl1_main函数的作用主要是：执行后期架构和平台的初始化函数，并查询平台要加载和运行的下一个 BL 映像。
> 注：只在冷启动后，由主 CPU 调用。

<details>
  <summary>点击这里展开/收起代码块</summary>

```C
void bl1_main(void)
{
   unsigned int image_id;

   /* Announce our arrival */
   NOTICE(FIRMWARE_WELCOME_STR);
   NOTICE("BL1: %s\n", version_string);
   NOTICE("BL1: %s\n", build_message);

   INFO("BL1: RAM %p - %p\n", (void *)BL1_RAM_BASE,
               (void *)BL1_RAM_LIMIT);

   print_errata_status();

#if ENABLE_ASSERTIONS
   u_register_t val;

#ifdef __aarch64__
   val = read_sctlr_el3();
#else
   val = read_sctlr();
#endif
   assert(val & SCTLR_M_BIT);
   assert(val & SCTLR_C_BIT);
   assert(val & SCTLR_I_BIT);
   
   val = (read_ctr_el0() >> CTR_CWG_SHIFT) & CTR_CWG_MASK;
  
   if (val != 0)
      assert(CACHE_WRITEBACK_GRANULE == SIZE_FROM_LOG2_WORDS(val));
   else
      assert(CACHE_WRITEBACK_GRANULE <= MAX_CACHE_LINE_SIZE);
#endif /* ENABLE_ASSERTIONS */

   bl1_arch_setup();

#if TRUSTED_BOARD_BOOT
   
   auth_mod_init();
#endif /* TRUSTED_BOARD_BOOT */

   bl1_platform_setup();

#if ENABLE_PAUTH

   bl1_apiakey[0] = read_apiakeylo_el1();
   bl1_apiakey[1] = read_apiakeyhi_el1();
#endif /* ENABLE_PAUTH */

   image_id = bl1_plat_get_next_image_id();

   if (image_id == BL2_IMAGE_ID)
      bl1_load_bl2();
   else
      NOTICE("BL1-FWU: *******FWU Process Started*******\n");

   bl1_prepare_next_image(image_id);

   console_flush();
}
```

</details>

---

`unsigned int image_id;`：`image_id`用来存接下来要传入那个image

---

```C
NOTICE(FIRMWARE_WELCOME_STR);
NOTICE("BL1: %s\n", version_string);
NOTICE("BL1: %s\n", build_message);
```

这三句就是打印一下，表明进入了`bl1_main()`。

`INFO("BL1: RAM %p - %p\n", (void *)BL1_RAM_BASE,(void *)BL1_RAM_LIMIT);`：这个函数打印一个提示信息，通过打印BL1在 RAM 中的起始地址和结束地址来记录 BL1 使用的 RAM 的内存范围。

`print_errata_status();`：这个函数是一个获取并打印 CPU 已知错误状态的函数。

---

```C
#if ENABLE_ASSERTIONS
   u_register_t val;
   
#ifdef __aarch64__
   val = read_sctlr_el3();
#else
   val = read_sctlr();
#endif
   assert(val & SCTLR_M_BIT);
   assert(val & SCTLR_C_BIT);
   assert(val & SCTLR_I_BIT);
   
   val = (read_ctr_el0() >> CTR_CWG_SHIFT) & CTR_CWG_MASK;
  
   if (val != 0)
      assert(CACHE_WRITEBACK_GRANULE == SIZE_FROM_LOG2_WORDS(val));
   else
      assert(CACHE_WRITEBACK_GRANULE <= MAX_CACHE_LINE_SIZE);
#endif /* ENABLE_ASSERTIONS */
```

这段代码只有在 ENABLE_ASSERTIONS 为真，也就是启用断言的时候编译器才会编译这段代码。

```C
#ifdef __aarch64__
   val = read_sctlr_el3();
#else
   val = read_sctlr();
#endif
```

这段代码含义是当定义了`__aarch64__`这个宏，即当前平台为 ARMv8 架构，那么就获得 EL3 级别权限。当不是这个架构时，那么调用`read_sctlr()`获取系统控制寄存器。
> 注：只有 ARMv8 架构中才设置了 EL3 这个高级别权限，在非 ARMv8 架构中，不同寄存器对应不同权限，没有设置 EL3，因此需要使用不同函数。

```C
assert(val & SCTLR_M_BIT);
assert(val & SCTLR_C_BIT);
assert(val & SCTLR_I_BIT);
```

这段代码的作用时确保得到的系统控制寄存器的值（权限）是否包含以下三个：
`SCTLR_M_BIT`：表示控制寄存器中的管理模式（Management Mode）位，用于确定处理器当前的操作模式或权限级别。
`SCTLR_C_BIT`：表示控制寄存器中的缓存（Cache）使能位，用于控制处理器的缓存功能是否开启或关闭。
`SCTLR_I_BIT`：表示控制寄存器中的指令缓存（Instruction Cache）使能位，用于控制处理器是否启用指令缓存。

```C
val = (read_ctr_el0() >> CTR_CWG_SHIFT) & CTR_CWG_MASK;
```

这行代码是在获得 CTR_EL0 中的缓存写回粒度 (CWG)。  
`read_ctr_el0()`函数是用来读取当前处理器的控制寄存器 EL0 的值。在 ARM 架构中，EL0 是最低特权级别，通常是用户空间的执行级别。  
`CTR_CWG_SHIFT`是一个宏，用于表示控制寄存器中描述写回缓存的粒度字段的起始位偏移量。它告诉我们要将从 read_ctr_el0() 返回的值向右移动多少位，以便获取我们该位字段。  
`TR_CWG_MASK`是一个宏，它是一个掩码，用于过滤出描述写回缓存的粒度位字段。通常，它是一个二进制掩码，将不相关的位设置为 0，保留相关的位。  

```C
if (val != 0)
   assert(CACHE_WRITEBACK_GRANULE == SIZE_FROM_LOG2_WORDS(val));
else
   assert(CACHE_WRITEBACK_GRANULE <= MAX_CACHE_LINE_SIZE);
```

`CACHE_WRITEBACK_GRANULE`：平台的缓存写回粒度。
`MAX_CACHE_LINE_SIZE`：架构下的最大缓存写回粒度。
`SIZE_FROM_LOG2_WORDS()`：用于计算以字为单位的大小，其参数是一个以对数（log2）形式表示的字数。

如果获取的缓存写回粒度不是0，那么说明程序定义了缓存写回粒度，那么我们可以通过计算其大小来判断是不是等于平台的默认规定的缓存写回粒度。
如果获取的缓存写回粒度为0，那么说明没有可用的 CWG 信息，这时候我们可以检查一下平台的缓存写回粒度是否小于架构的最大值，如果小于，那么说明没有问题，可以通过默认条件进行缓存写回。

---

```C
bl1_platform_setup();
```

执行剩余的 EL3 级别的通用架构设置。
其中只有`write_scr_el3(read_scr_el3() | SCR_RW_BIT);`一行语句。这行语句的作用是读取现在的 EL3 系统控制器的值，并将读写权限(SCR_RW_BIT)设置为打开，并写回到系统控制器，从而实现打开读写权限。

---

```C
#if ENABLE_PAUTH
   bl1_apiakey[0] = read_apiakeylo_el1();
   bl1_apiakey[1] = read_apiakeyhi_el1();
#endif /* ENABLE_PAUTH */
```

这段代码只有在 ENABLE_PAUTH 为真，也就是平台认证启用之后才会编译。
当平台认证启用之后，使用read_apiakeylo_el1()，read_apiakeyhi_el1()获取处理器 APIAKey_EL1 密钥的低位和高位部分。
bl1_apiakey 这个数组里面存储了 APIAKey_EL1 密钥的完整部分。

---

```C
   image_id = bl1_plat_get_next_image_id();
   if (image_id == BL2_IMAGE_ID)
      bl1_load_bl2();
   else
      NOTICE("BL1-FWU: *******FWU Process Started*******\n");
   bl1_prepare_next_image(image_id);
   console_flush();
```

`image_id = bl1_plat_get_next_image_id();`：获得下一步平台要加载和执行的 image。  
接下里如果发现需要加载 bl2 ，那么调用函数`bl1_load_bl2()`将 bl2 加载到内存中。  
如果不是，那么调用`NOTICE`函数打印一条提示信息，表示固件更新程序启动。  

`bl1_prepare_next_image(image_id);`：调用这个函数，说明 bl1 阶段基本执行完毕，开始对下一个要执行的image进行一些准备。包含了：获取镜像描述符、获取入口点信息、设置处理器状态寄存器、平台的特定操作、初始化处理器上下文、非安全镜像处理、刷新上下文并标记镜像执行状态、打印入口点信息。这里会将 bl2 的入口地址写道 elr_el3 寄存器。方便 bl1 执行完毕后，直接跳转到 bl2 的入口。  

`console_flush();`：函数用于刷新控制台输出缓冲区，确保待输出的内容会及时显示在控制台上。

## 1.6 pauth_disable_el3 源码解析

`pauth_disable_el3`是关闭 EL3 指针认证。

```asm
func pauth_disable_el3
   mrs   x0, sctlr_el3
   bic   x0, x0, #SCTLR_EnIA_BIT
   msr   sctlr_el3, x0
   isb
   ret
endfunc pauth_disable_el3
```

`mrs   x0, sctlr_el3`：把表示第三异常级别(EL3)的系统控制寄存器(SCTLR)的信息存储到 x0 寄存器中，方便后续进行修改。

`bic   x0, x0, #SCTLR_EnIA_BIT`：`bic`是位清除指令，清除指定的位，这里相当于要关闭 EL3 指针认证，因此关闭系统控制寄存器对应的位。

`msr   sctlr_el3, x0`：将修改后的系统控制寄存器的值放回可以生效的寄存器中。

`isb`：等待系统控制寄存器的修改生效。

## 1.7 el3_exit 源码解析

<details>
  <summary>点击这里展开/收起代码块</summary>

```asm
func el3_exit

   mov   x17, sp
   msr   spsel, #MODE_SP_ELX
   str   x17, [sp, #CTX_EL3STATE_OFFSET + CTX_RUNTIME_SP]

   ldr   x18, [sp, #CTX_EL3STATE_OFFSET + CTX_SCR_EL3]
   ldp   x16, x17, [sp, #CTX_EL3STATE_OFFSET + CTX_SPSR_EL3]
   msr   scr_el3, x18
   msr   spsr_el3, x16
   msr   elr_el3, x17

#if IMAGE_BL31 && DYNAMIC_WORKAROUND_CVE_2018_3639
   ldr   x17, [sp, #CTX_CVE_2018_3639_OFFSET + CTX_CVE_2018_3639_DISABLE]
   cbz   x17, 1f
   blr   x17
1:
#endif

   bl   restore_gp_pmcr_pauth_regs
   ldr   x30, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_LR]

#if IMAGE_BL31 && RAS_EXTENSION
   esb
#endif
   eret

endfunc el3_exit
```

</details>

---

```asm
mov   x17, sp
msr   spsel, #MODE_SP_ELX
str   x17, [sp, #CTX_EL3STATE_OFFSET + CTX_RUNTIME_SP]
```

这段代码的作用是保存当前的 SP_EL0，即 EL3 运行时堆栈，它将用于处理下一个 SMC。然后切换到 SP_EL3。

`mov x17,sp`：将栈顶指针的值存放到 x17 寄存器中。这一步是保存栈指针的现场，方便后续恢复现场。

`spsel`：目标系统寄存器，代表堆栈指针选择（Stack Pointer Select）。它决定当前堆栈指针选择的是 SP_EL0 还是 SP_ELx。当 spsel 寄存器设置为 0 时，系统使用 SP_EL0 作为当前堆栈指针。当 spsel 寄存器设置为 1 时，系统使用 SP_ELx 作为当前堆栈指针。

`msr spsel,#MODE_SP_ELX`：把堆栈指针系统寄存器 spsel 的值设置为 MODE_SP_ELX。

`CTX_EL3STATE_OFFSET`：用于表示存储与EL3（Exception Level 3）相关的上下文状态的基址偏移量。

`CTX_RUNTIME_SP`：用于表示特定上下文中的运行时堆栈指针（Stack Pointer）的偏移量。在保存上下文时，通常不仅要保存通用寄存器的值，还要保存堆栈指针的值，以便恢复时能够准确地恢复执行状态。在上下文切换时，保存和恢复堆栈指针的值是非常重要的。CTX_RUNTIME_SP 表示这个堆栈指针在上下文结构中的具体位置。

`str x17, [sp, #CTX_EL3STATE_OFFSET + CTX_RUNTIME_SP]`：表示将寄存器 x17 中的值存到内存中，位置为栈顶指针 sp 偏移CTX_EL3STATE_OFFSET + CTX_RUNTIME_SP 后对应的位置。

---

```asm
ldr   x18, [sp, #CTX_EL3STATE_OFFSET + CTX_SCR_EL3]
ldp   x16, x17, [sp, #CTX_EL3STATE_OFFSET + CTX_SPSR_EL3]
msr   scr_el3, x18
msr   spsr_el3, x16
msr   elr_el3, x17
```

`ldr   x18, [sp, #CTX_EL3STATE_OFFSET + CTX_SCR_EL3]`：从堆栈中加载保存的 scr_el3 值到 x18。

`ldp   x16, x17, [sp, #CTX_EL3STATE_OFFSET + CTX_SPSR_EL3]`：从堆栈中加载保存的 spsr_el3 和 elr_el3 到 x16 和 x17。

```asm
msr   scr_el3, x18
msr   spsr_el3, x16
msr   elr_el3, x17
```

这三行代码，将三个寄存器中的值分别存到相应的系统寄存器中。
`scr_el3`：控制 EL3 的安全配置和行为，如安全状态的转换和异常处理的配置。

`spsr_el3`：用于保存 EL3 异常处理程序处理异常返回时的程序状态。内容包括条件标志、处理器模式（如用户模式、系统模式等）、中断屏蔽状态等。

`elr_el3`：保存导致 EL3 异常的指令的地址，即异常返回后将继续执行的下一条指令的地址。

---

```asm
#if IMAGE_BL31 && DYNAMIC_WORKAROUND_CVE_2018_3639
   ldr x17, [sp, #CTX_CVE_2018_3639_OFFSET + CTX_CVE_2018_3639_DISABLE]
   cbz x17, 1f
   blr x17
1:
#endif
```

`#if IMAGE_BL31 && DYNAMIC_WORKAROUND_CVE_2018_3639`：检查是否启用了动态修复。

`CTX_CVE_2018_3639_OFFSET`：这个常量通常用于表示存储了特定上下文数据结构中，用于存储有关"Speculative Store Bypass (SSB)"漏洞的修复信息的偏移量。

`CTX_CVE_2018_3639_DISABLE`：这个常量通常用于指示是否禁用或启用有关"Speculative Store Bypass (SSB)"漏洞的修复措施。
> 注："Speculative Store Bypass (SSB)"，也称为"Variant 4"漏洞，是一种影响现代处理器的侧信道攻击漏洞。它与其他类似的漏洞（如Meltdown和Spectre）一样，利用了处理器中的 speculative execution（预测执行）机制来实现攻击。

`ldr x17, [sp, #CTX_CVE_2018_3639_OFFSET + CTX_CVE_2018_3639_DISABLE]`：从堆栈中加载 CVE-2018-3639 漏洞修复的禁用标志。。

`cbz x17, 1f`：检查寄存器 x17 的值是否为零，如果是零，那么跳转到1处。这里的f是一个后缀，通常用来表示这是一个正向（forward）的标号，也就是距离当前指令位置最近的标号。

`blr x17`：跳转到 x17 中的地址，执行动态修复代码。
>注：
>在 ARM64 架构中，blr 指令用于调用寄存器中保存的函数地址，并将下一条指令的地址保存到链接寄存器 lr 中，以便后续可以通过 ret 或 ret x30 指令返回调用点。这条指令执行后，程序控制流会转移到 x17 寄存器中存储的函数的入口地址，并且保存当前指令的地址到链接寄存器 lr 中，以便函数执行完毕后返回到调用点。

---

```asm
bl   restore_gp_pmcr_pauth_regs
ldr   x30, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_LR]
```

`bl restore_gp_pmcr_pauth_regs`：跳转到 restore_gp_pmcr_pauth_regs 函数，这个函数从 CPU 上下文中恢复 ARMv8.3-PAuth（如果已启用）和除 x30 之外的所有通用寄存器。x30 寄存器必须由调用者明确恢复。

`CTX_GPREGS_OFFSET`：用于表示通用寄存器在上下文数据结构中的偏移量。这个偏移量可以用于在代码中访问和修改通用寄存器的值。通常，这种偏移量的使用是为了在上下文切换或者异常处理时，能够正确地保存和恢复这些寄存器的状态。

`CTX_GPREG_LR`：表示上下文数据结构中链接寄存器（Link Register，通常是用来保存函数调用的返回地址）的偏移量。在 ARM 架构中，通常用 LR 寄存器来保存 bl 指令后的返回地址。

`ldr   x30, [sp, #CTX_GPREGS_OFFSET + CTX_GPREG_LR]`：将从堆栈中加载链接寄存器的值到 x30。

---

```asm
#if IMAGE_BL31 && RAS_EXTENSION
   esb
#endif
   eret
```

`#if IMAGE_BL31 && RAS_EXTENSION`：检查是否启用了 RAS 扩展。

`esb`：执行错误同步屏障，确保所有错误状态已被处理（仅当启用了 RAS 扩展时执行）。

`eret`：从异常处理返回到正常执行流。

eret 指令会从当前的异常级别返回到较低的异常级别，并跳转到 ELR_EL3 寄存器中存储的地址。具体执行流程如下：

1. 恢复状态：eret 会恢复 SPSR_EL3 中保存的程序状态寄存器内容，包括中断状态和处理器模式。
2. 跳转到入口点：eret 会将控制权转移到 ELR_EL3 中存储的地址，该地址是在BL1中设置的BL2入口点地址。

> 注：
> 通过分析 bl1_main 函数及其调用的关键函数 bl1_load_bl2 和 bl1_prepare_next_image，可以看到：
> BL2 的入口地址是在 bl1_load_bl2 函数中确定并保存的。
> 在 bl1_prepare_next_image 函数中，这个入口地址被写入 elr_el3 寄存器。
> 当 BL1 完成其任务并执行 eret 指令时，处理器会跳转到 elr_el3 中存储的地址，即 BL2 的入口地址，从而开始执行 BL2 的代码
