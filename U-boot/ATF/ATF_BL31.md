<!-- markdownlint-disable MD033 -->

# ATF启动流程三：bl31

[back](./ATF.md)

在 bl2 中通过调用 smc 指令后会跳转到 bl31 中进行执行，bl31 最终主要的作用是建立 EL3 runtime software，在该阶段会建立各种类型的 smc 调用注册并完成对应的 cortex 状态切换。该阶段主要执行在monitor中。
通过查看`bl31.ld.S`文件可知，bl31 的入口点为`bl31_entrypoint`。

BL31 的主要职责是：

**1. 上下文切换和异常处理**  
BL31 负责在安全世界和非安全世界之间进行上下文切换。这包括处理来自 EL1 和 EL2 的异常，并根据需要将控制权交给相应的世界。

* **异常向量表**：BL31 配置和管理异常向量表，以确保能够正确处理异常。
* **上下文切换**：保存和恢复安全世界和非安全世界的上下文信息，以确保系统状态的一致性。

**2. 运行时服务**  
BL31 提供各种运行时服务，这些服务可以通过 SMC（Secure Monitor Call）进行调用。这些服务包括但不限于：

* **电源管理**：管理系统电源状态，如进入休眠和唤醒等。
* **中断管理**：处理和分发中断信号，确保中断能够被正确传递到目标处理器或核心。
* **安全服务**：管理安全相关的操作，如密钥管理、加密操作等。

**3. 初始化任务**  
在系统启动过程中，BL31 负责初始化各种硬件和软件组件。这些初始化任务确保系统在进入操作系统之前处于正确的状态。

* **硬件初始化**：初始化关键硬件组件，如中断控制器（GIC）、定时器等。
* **内存初始化**：配置和初始化内存子系统，确保内存能够被正确访问和使用。

---

详细工作流程

**1. 启动和初始化**
BL31 的启动和初始化通常包括以下步骤：

1. 启动入口：BL31 从前一个引导阶段（如 BL2 或 BL1）接管控制权，进入其启动入口点。
2. 基本初始化：进行基本的硬件初始化，如设置堆栈指针、初始化异常向量表等。
3. 服务初始化：初始化各种运行时服务，确保它们能够正常工作。
4. 转移控制权：将控制权转移给非安全世界的引导阶段，通常是 BL33（如 UEFI 或操作系统引导程序）。

**2. 运行时管理**
在系统运行期间，BL31 处理各种运行时事件和服务请求：

1. 异常处理：捕获和处理来自 EL1 和 EL2 的异常，根据需要进行上下文切换。
2. SMC 调用处理：处理来自非安全世界的 SMC 调用，提供运行时服务。
3. 电源管理：处理电源管理事件，如进入和退出低功耗状态。

## 3.1 bl31_entrypoint 源码解析

### 3-1-1 源代码

> 注：bl31_entrypoint()函数只有在冷启动情况下由主 CPU 执行。

<details>
    <summary>点击这里展开/收起代码块</summary>

```asm
func bl31_entrypoint
#ifdef PLAT_imx8qm
    ldr    x21, stm
    ldr    w20, =0x80000000
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21] /* 16 */
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21]
    str    w20, [x21] /* 32 */
#endif

    mov    x20, x0
    mov    x21, x1
    mov    x22, x2
    mov    x23, x3

#if ENABLE_PIE
    mov_imm    x0, BL31_BASE
    mov_imm    x1, BL31_LIMIT
    bl    fixup_gdt_reloc
#endif /* ENABLE_PIE */

#if !RESET_TO_BL31
    el3_entrypoint_common                    \
        _init_sctlr=0                    \
        _warm_boot_mailbox=0                \
        _secondary_cold_boot=0                \
        _init_memory=0                    \
        _init_c_runtime=1                \
        _exception_vectors=runtime_exceptions
#else

    el3_entrypoint_common                    \
        _init_sctlr=1                    \
        _warm_boot_mailbox=!PROGRAMMABLE_RESET_ADDRESS    \
        _secondary_cold_boot=!COLD_BOOT_SINGLE_CPU    \
        _init_memory=1                    \
        _init_c_runtime=1                \
        _exception_vectors=runtime_exceptions

    mov    x20, 0
    mov    x21, 0
    mov    x22, 0
    mov    x23, 0
#endif /* RESET_TO_BL31 */

    mov    x0, x20
    mov    x1, x21
    mov    x2, x22
    mov    x3, x23
    bl    bl31_setup

#if ENABLE_PAUTH
    bl    pauth_init_enable_el3
#endif /* ENABLE_PAUTH */

    bl    bl31_main

    adr    x0, __DATA_START__
    adr    x1, __DATA_END__
    sub    x1, x1, x0
    bl    clean_dcache_range

    adr    x0, __BSS_START__
    adr    x1, __BSS_END__
    sub    x1, x1, x0
    bl    clean_dcache_range

    b    el3_exit
endfunc bl31_entrypoint

.ltorg
stm:
    .quad 0x5D1B0020
```

</details>

### 3-1-2 源码解析

<details>
    <summary>点击这里展开/收起代码块</summary>

```asm
#ifdef PLAT_imx8qm
    ldr    x21, stm
    ldr    w20, =0x80000000
    str    w20, [x21]
    ...
    str    w20, [x21] /* 16 */
    ...
    str    w20, [x21] /* 32 */
#endif

...

.ltorg
stm:
    .quad 0x5D1B0020
```

</details>

`#ifdef PLAT_imx8qm`：条件编译选项，表示如果`PLAT_imx8qm`这个宏被定义，也就是使用的`imx8qm`这个平台是，进行如下操作。

`ldr x21, stm`：这行指令将标签`stm`所指向的地址加载到寄存器`x21`中。

`ldr w20,=0x80000000`：将立即数`0x80000000`，加载到 32 位宽的寄存器 w20 中。

`str w20,[x21]`：这条指令将寄存器 w20 中的值存储到 x21 寄存器所指向的地址（x21 寄存器中存了一个地址，就是存到这个地址）。这条指令重复了 32 次，应该是初始化过程中确保正确写入到硬件寄存器或者内存的某个区域，以此来正确设置。

`.ltorg`：是一个 GNU 汇编器的伪指令，用于将常量池放置在当前指令流的末尾。常量池是用来存储大于指令立即数范围的常量数据。在这行指令之后的常量数据可以通过偏移来访问。

`stm`：这是一个标签，用于标记接下来的一行数据的内存位置。标签在汇编代码中用作符号，指代特定的内存地址或位置。这个标签使得我们可以引用该位置的数据。

`.quad 0x5D1B0020`：是一个伪指令，用于声明一个 64 位的常量数据。这里声明的常量值是`0x5D1B0020`。这个值会被存储在由 stm 标签标记的内存位置。

---

```asm
mov    x20, x0
mov    x21, x1
mov    x22, x2
mov    x23, x3
```

这几行指令的作用是保存现场，将之前的设置先保存起来，后面要用。

---

```asm
#if ENABLE_PIE
    mov_imm    x0, BL31_BASE
    mov_imm    x1, BL31_LIMIT
    bl    fixup_gdt_reloc
#endif /* ENABLE_PIE */
```

这段代码涉及条件编译，用于在启用了 Position Independent Executable（PIE）时执行一系列操作。具体来说，它通过设置两个寄存器并调用一个函数来修正全局描述符表（GDT）的重定位。

`#if ENABLE_PIE`：当定义了宏`ENABLE_PIE`时，即启用位置无关可执行文件时，将下列指令进行编译。

> 注：  
> PIE 使得可执行文件在加载时可以被随机化放置在内存中的任何位置，以防止某些类型的攻击（例如，缓冲区溢出攻击）。当启用 PIE 时，必须调整代码和数据的地址以确保它们在新位置正确运行。

`mov_imm    x0, BL31_BASE`：将立即数`BL31_BASE`中的值加载到寄存器 x0 中。`BL31_BASE`表示 BL31 镜像的基地址。

`mov_imm    x1, BL31_LIMIT`：将立即数`BL31_LIMIT`中的值加载到寄存器 x1 中。`BL31_LIMIT`表示 BL31 镜像的末地址。

`bl    fixup_gdt_reloc`：跳转执行函数`fixup_gdt_reloc`，这个函数将修正全局描述符表(GDT)的重定位。在启用 PIE 时，内存中的代码和数据可能被重定位。fixup_gdt_reloc 函数用于更新全局描述符表，使其指向新的地址范围，以确保系统的正常运行。

---

```asm
#if !RESET_TO_BL31
...
else
...
#endif /* RESET_TO_BL31 */
```

`RESET_TO_BL31`：表示系统复位时会进入 BL31，跳过其他引导阶段的初始化。这通常用于简化引导流程或者在特定系统配置中提高引导效率。

`!RESET_TO_BL31`：表示系统复位时不会进入 BL31 状态，而是通过 BL1 或者 BL2 进行进一步初始化。

---

如果设置了`!RESET_TO_BL31`复位不进入 BL31，那么执行下面语句：

```asm
el3_entrypoint_common                    \
    _init_sctlr=0                    \
    _warm_boot_mailbox=0                \
    _secondary_cold_boot=0                \
    _init_memory=0                    \
    _init_c_runtime=1                \
    _exception_vectors=runtime_exceptions
```

调用`el3_entrypoint_common`宏函数，后面紧跟需要传入的参数。这是 EL3 中冷启动或热启动期间需要执行的操作的超集。此代码由 BL1 和 BL31 共享。此宏将始终执行重置处理、架构初始化和堆栈设置。其余操作是可选的，因为它们可能不需要，具体取决于调用此宏的上下文。这就是此宏被参数化的原因；每个参数都允许启用/禁用某些操作。

---

如果设置了`RESET_TO_BL31`复位进入 BL31 状态，那么执行下面语句：

```asm
el3_entrypoint_common                    \
    _init_sctlr=1                    \
    _warm_boot_mailbox=!PROGRAMMABLE_RESET_ADDRESS    \
    _secondary_cold_boot=!COLD_BOOT_SINGLE_CPU    \
    _init_memory=1                    \
    _init_c_runtime=1                \
    _exception_vectors=runtime_exceptions

mov    x20, 0
mov    x21, 0
mov    x22, 0
mov    x23, 0
```

第一部分进行了当设置了复位进入 BL31 状态的宏定义的时候，调用宏函数`el3_entrypoint_common`需要传入的参数。

第二部分对寄存器进行初始化。

> 问：已知之前 x20 ~ x23 寄存器中存储了 x0 ~ x3 寄存器中关于初始化相关的信息，这里问什么要重新设置为 0 ？  
> 答：对于 RESET_TO_BL31 系统，BL31 是第一个运行的引导加载程序，因此没有从之前的引导加载程序传递的参数。将传递给平台层的参数清零以反映这一点。

---

```asm
mov    x0, x20
mov    x1, x21
mov    x2, x22
mov    x3, x23
bl    bl31_setup
```

`bl31_setup`函数会使用 x0 ~ x3 寄存器，并且这四个寄存器中应该是引导加载程序传入的设置相关的信息，因此还原现场，方便函数调用。

---

```asm
#if ENABLE_PAUTH
    bl    pauth_init_enable_el3
#endif /* ENABLE_PAUTH */
```

如果启用了 EL3 级别指针认证，那么初始化并启用 EL3 级别指针认证。

---

```asm
bl    bl31_main
```

跳转执行函数 bl31_main。

BL31 负责在将控制权传递给引导加载程序或操作系统之前设置主 CPU 的运行时服务。此函数调用 Runtime_svc_init()，它初始化所有已注册的运行时服务。运行时服务将设置足够的上下文，以便核心切换到下一个异常级别。当此函数返回时，核心将通过 ERET 切换到编程的异常级别。

---

```asm

adr    x0, __DATA_START__
adr    x1, __DATA_END__
sub    x1, x1, x0
bl    clean_dcache_range

adr    x0, __BSS_START__
adr    x1, __BSS_END__
sub    x1, x1, x0
bl    clean_dcache_range
```

这两部分代码通过调用函数清除的`.data`段和`.bss`段的数据，为接下来的使用提供空间。

---

```asm
b    el3_exit
```

退出 EL3 级别执行。

## 3.2 bl31_main 源码解析

### 3-2-1 源代码

<details>
    <summary>点击这里展开/收起代码块<summary>

```C
void bl31_main(void)
{
    NOTICE("BL31: %s\n", version_string);
    NOTICE("BL31: %s\n", build_message);

    bl31_platform_setup();

    bl31_lib_init();

#if EL3_EXCEPTION_HANDLING
    INFO("BL31: Initialising Exception Handling Framework\n");
    ehf_init();
#endif

    INFO("BL31: Initializing runtime services\n");
    runtime_svc_init();

    if (bl32_init != NULL) {
        INFO("BL31: Initializing BL32\n");
        int32_t rc = (*bl32_init)();
        if (rc == 0)
            WARN("BL31: BL32 initialization failed\n");
    }

    bl31_prepare_next_image_entry();

    console_flush();

    bl31_plat_runtime_setup();
}
```

</details>

### 3-2-2 源码解析

```C
NOTICE("BL31: %s\n", version_string);
NOTICE("BL31: %s\n", build_message);
```

这两行代码通过日志输出打印了两条消息，分别是映像的版本信息和构建信息。

---

```C
bl31_platform_setup();
```

`bl31_platform_setup`：函数用于在 BL31 阶段初始化平台特定的硬件组件，特别是中继控制器(GIC)，这个函数初始化了 GIC 的 CPU 和分发器接口，并启用了 CPU 接口。

这个函数调用了四个函数分别做了：初始化 GICv2 驱动、初始化 GICv2 分发器接口、初始化 GICv2 处理器分发接口、启用 GICv2 CPU 接口。

---

```C
bl31_lib_init();
```

初始化 bl31 启动阶段的一下辅助库。

---

```C
#if EL3_EXCEPTION_HANDLING
    INFO("BL31: Initialising Exception Handling Framework\n");
    ehf_init();
#endif
```

如果开启了 EL3 阶段的异常处理，那么首先打印一条消息说“BL31 正在初始化异常处理程序”。然后调用函数初始化异常处理程序。

---

```C
INFO("BL31: Initializing runtime services\n");
runtime_svc_init();
```

首先打印消息，显示正在初始化运行时服务。调用函数进行运行时服务的初始化，如中断处理、电源管理、安全服务等。

---

```C
if (bl32_init != NULL) {
    INFO("BL31: Initializing BL32\n");
    int32_t rc = (*bl32_init)();
    if (rc == 0)
        WARN("BL31: BL32 initialization failed\n");
}
```

判断是否需要进行 bl32 的的初始化，如果需要那么首先打印信息表明开始进行 bl31 的初始化，然后调用初始化函数，并通过其返回值来判断初始化是否成功。

---

```C
bl31_prepare_next_image_entry();
```

获得下一个需要加载的的影响的入口信息。

---

```C
console_flush();
```

用于刷新控制台输出缓冲区，确保待输出的内容会及时显示在控制台上。

---

```C
bl31_plat_runtime_setup();
```

退出 bl31 之前需要执行特定平台的运行时设置，主要是特定硬件的初始化，如 DRAM 信息的获取和设置。

## 3.3 runtime_svc_init 源码解析

### 3-3-1 源代码

<details>
    <summary>点击这里展开/收起代码块</summary>

```C
void __init runtime_svc_init(void)
{
    int rc = 0;
    uint8_t index, start_idx, end_idx;
    rt_svc_desc_t *rt_svc_descs;

    assert((RT_SVC_DESCS_END >= RT_SVC_DESCS_START) &&
            (RT_SVC_DECS_NUM < MAX_RT_SVCS));

    if (RT_SVC_DECS_NUM == 0U)
        return;
   
    (void)memset(rt_svc_descs_indices, -1, sizeof(rt_svc_descs_indices));

    rt_svc_descs = (rt_svc_desc_t *) RT_SVC_DESCS_START;
    for (index = 0U; index < RT_SVC_DECS_NUM; index++) {
        rt_svc_desc_t *service = &rt_svc_descs[index];

        rc = validate_rt_svc_desc(service);
        if (rc != 0) {
            ERROR("Invalid runtime service descriptor %p\n",
                (void *) service);
            panic();
        }

        if (service->init != NULL) {
            rc = service->init();
            if (rc != 0) {
                ERROR("Error initializing runtime service %s\n",
                        service->name);
                continue;
            }
        }

        start_idx = (uint8_t)get_unique_oen(service->start_oen,
                            service->call_type);
        end_idx = (uint8_t)get_unique_oen(service->end_oen,
                          service->call_type);
        assert(start_idx <= end_idx);
        assert(end_idx < MAX_RT_SVCS);
        for (; start_idx <= end_idx; start_idx++)
            rt_svc_descs_indices[start_idx] = index;
    }
}
```

</details>

### 3-3-2 源码解析

`assert((RT_SVC_DESCS_END >= RT_SVC_DESCS_START) && (RT_SVC_DECS_NUM < MAX_RT_SVCS));`：断言判断描述符的数量小于最大索引，

`if (RT_SVC_DECS_NUM == 0U) return;`：如果没有运行时服务，那么直接返回就行了，不需要初始化。

`(void)memset(rt_svc_descs_indices, -1, sizeof(rt_svc_descs_indices));`：这里将内部变量设为-1，表示无效。

`rt_svc_descs = (rt_svc_desc_t *) RT_SVC_DESCS_START;`：获得运行时服务描述符表的表头地址。

`for (index = 0U; index < RT_SVC_DECS_NUM; index++) {...}`：遍历一遍运行时服务描述表。

`rt_svc_desc_t *service = &rt_svc_descs[index];`：声明一个变量用来存储本次循环需要用到的运行时服务的描述符的地址。

---

```C
rc = validate_rt_svc_desc(service);
if (rc != 0) {
    ERROR("Invalid runtime service descriptor %p\n",
    (void *) service);
    panic();
}
```

这段代码的作用是验证这个运行时服务是否是一个错误，如果是那么将很难预测系统的表现。

如果 rc 不是 0 ，说明这是一个不合法的文件描述符，应该停止运行。

`validate_rt_svc_desc(service)`：这个函数会对传入的文件描述符进行简单的判断，当文件描述符不是合法的文件描述符时返回确定的错误(`-EINVAL` = -22)。

---

```C
if (service->init != NULL) {
    rc = service->init();
    if (rc != 0) {
        ERROR("Error initializing runtime service %s\n",service->name);
    continue;
    }
}
```

运行时服务可能为其快速 SMC 和延迟 SMC 分别设置独立的 rt_svc_desc_t。由于服务本身只需初始化一次，因此只有其中一个会定义初始化例程。如果定义了该运行时服务的初始化例程，则调用这段代码，当服务并没有初始化时，对服务进行初始化。

---

```C
start_idx = (uint8_t)get_unique_oen(service->start_oen,service->call_type);
end_idx = (uint8_t)get_unique_oen(service->end_oen,service->call_type);
assert(start_idx <= end_idx);
assert(end_idx < MAX_RT_SVCS);
for (; start_idx <= end_idx; start_idx++)
    rt_svc_descs_indices[start_idx] = index;
```

将对应起始和结束 oen 的索引填充为描述符的索引，该描述符将处理属于此 oen 范围内的 SMC 调用。

## 3.4 bl31_prepare_next_image_entry 源码解析

### 3-4-1 源代码

<details>
    <summary>点击这里展开/收起代码块<summary>

```C
void __init bl31_prepare_next_image_entry(void)
{
    entry_point_info_t *next_image_info;
    uint32_t image_type;

#if CTX_INCLUDE_AARCH32_REGS
    if (el_implemented(1) == EL_IMPL_A64ONLY) {
        ERROR("EL1 supports AArch64-only. Please set build flag "
                "CTX_INCLUDE_AARCH32_REGS = 0\n");
        panic();
    }
#endif

    image_type = bl31_get_next_image_type();

    next_image_info = bl31_plat_get_next_image_ep_info(image_type);
    assert(next_image_info != NULL);
    assert(image_type == GET_SECURITY_STATE(next_image_info->h.attr));

    INFO("BL31: Preparing for EL3 exit to %s world\n",
        (image_type == SECURE) ? "secure" : "normal");
    print_entry_point_info(next_image_info);
    cm_init_my_context(next_image_info);
    cm_prepare_el3_exit(image_type);
}
```

</details>

### 3-4-2 源码分析

```C
#if CTX_INCLUDE_AARCH32_REGS
    if (el_implemented(1) == EL_IMPL_A64ONLY) {
        ERROR("EL1 supports AArch64-only. Please set build flag "
                "CTX_INCLUDE_AARCH32_REGS = 0\n");
        panic();
    }
#endif
```

`#if CTX_INCLUDE_AARCH32_REGS`：这个宏定义是判断是否包含了 AARCH32 寄存器上下文。

`if (el_implemented(1) == EL_IMPL_A64ONLY)`：这个 if 语句是在检查 EL1 是否从 AA64PFR0 寄存器字段实现。

---

```C
image_type = bl31_get_next_image_type();
```

`bl31_get_next_image_type()`：这个函数返回一个变量的值，这个变量用于指示在 BL31 之后执行的下一个映像是 BL33（非安全和默认）还是 BL32（安全），是在文件中写好的。

---

```C
next_image_info = bl31_plat_get_next_image_ep_info(image_type);
assert(next_image_info != NULL);
assert(image_type == GET_SECURITY_STATE(next_image_info->h.attr));
```

这里根据前面代码得到的`image_type`的值，通过函数`bl31_plat_get_next_image_ep_info`获得下一个要执行的镜像。

---

```C
INFO("BL31: Preparing for EL3 exit to %s world\n", (image_type == SECURE) ? "secure" : "normal");
print_entry_point_info(next_image_info);
cm_init_my_context(next_image_info);
cm_prepare_el3_exit(image_type);
```

第一行代码是打印消息：在 BL31 执行完毕之后去安全世界还是非安全世界，这里由前面获得的`image_type`值来控制，其中 SECURE 代表 BL32 是安全世界， NON_SEUCRE 代表 BL33 是非安全世界。

`print_entry_point_info(next_image_info);`：打印下一个镜像的入口点信息。

`cm_init_my_context(next_image_info);`：函数初始化当前 CPU 的 cpu_context 以供首次使用，并设置由 entry_point_info 结构指定的初始入口点状态。

`cm_prepare_el3_exit(image_type);`：为首次进入安全或正常世界准备 CPU 系统寄存器 如果请求执行 hyp 模式，则初始化 HSCTLR。如果请求执行非安全 PL1，并且 CPU 支持HYP 模式，则通过配置所有必要的 HYP 模式寄存器来禁用 HYP 模式。

## 3.5 bl31_plat_runtime_setup 源码解析

### 3-5-1 源代码

<details>
    <summary>点击这里展开/执行代码块</summary>

```C
void bl31_plat_runtime_setup(void)
{
    struct draminfo *di = (struct draminfo *)(unsigned long)DRAMINFO_BASE;

    sq_scp_get_draminfo(di);
}

uint32_t sq_scp_get_draminfo(struct draminfo *info)
{
#if SQ_USE_SCMI_DRIVER
    sq_scmi_get_draminfo(info);
#else
    scpi_get_draminfo(info);
#endif
    return 0;
}
```

</details>

### 3-5-2 源码解析

`struct draminfo *di = (struct draminfo *)(unsigned long) DRAMINFO_BASE;`：这行代码将 DRAMINFO_BASE 强制类型转换成结构体指针 draminfo* 并赋值给变量 di，以方便访问结构体内部的数据。

> 问：为什么需要两次类型转换？  
> 答：这是为了防止类型转换时发生地址信息丢失或者截断。在 32 位系统和 64 位系统中，地址和指针的大小是不同的，为了适配两种型号，先将地址转换成 unsigned long 来确保地址转换成 64 位，然后转换成结构体指针，就能把这个地址变成结构体的头指针使用。  
> 如果只进行一次类型转换直接强制转换成结构体指针，那么可能会引起如下情况：
>
> * 地址信息可能丢失或截断，特别是在 64 位系统上。
> * 编译器可能会发出警告或错误。
> * 潜在的内存对齐问题。
>
> 因此为了保证代码的可移植性和安全性，应当使用两次类型转换。

---

`sq_scp_get_draminfo(di);`：这个函数的目的是根据系统配置动态选择合适的方式来获取 DRAM 信息，并将其存储在 info 指向的结构体中。

根据条件编译选项可知：如果使用 SCMI 驱动程序，调用 sq_scmi_get_draminfo(info)。如果不使用 SCMI 驱动程序，调用 scpi_get_draminfo(info)。

> SCMI (System Control and Management Interface):SCMI 是一种标准接口，用于系统控制和管理操作，常用于多核处理器和嵌入式系统。
> SCPI (System Control and Power Interface):SCPI 是另一种接口，用于系统控制和电源管理。

## 3.6 小结

为什么使用 uuu 烧录 emmc 时，我们只需要放置 bl31.bin 文件到指定的脚本文件即可？

首先，bl31.bin文件是启动过程中重要的一环，它是安全监控阶段，负责初始化安全世界，并将控制权转交给 bl33 ，而 bl33(u-boot) 则属于非安全世界引导阶段，负责程序从安全世界到正常世界的转换，因此为了引导出操作系统内核，bl31 阶段必不可少。  
其次，对于 bl1 和 bl2 启动阶段而言，虽然这两者也是重要的启动阶段，但是其任务相对简单，很多平台的 bl1 和 bl2 固件通常已经预配置在了 ROM 中。因此用户只需要提供 bl31.bin 来完成后续操作。  
