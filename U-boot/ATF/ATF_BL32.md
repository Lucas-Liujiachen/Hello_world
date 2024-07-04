<!-- markdownlint-disable MD033 -->

# ATF启动流程四：bl32

[back](./ATF.md)

## 4.1 opteed-setup 源码分析

### 4-1-1 源代码

<details>
    <summary>点击这里展开/收起代码块</summary>

```C
static int32_t opteed_setup(void)
{
    entry_point_info_t *optee_ep_info;
    uint32_t linear_id;
    uint64_t opteed_pageable_part;
    uint64_t opteed_mem_limit;
    uint64_t dt_addr;

    linear_id = plat_my_core_pos();

    optee_ep_info = bl31_plat_get_next_image_ep_info(SECURE);
    if (!optee_ep_info) {
        WARN("No OPTEE provided by BL2 boot loader, Booting device"
            " without OPTEE initialization. SMC`s destined for OPTEE"
            " will return SMC_UNK\n");
        return 1;
    }

    if (!optee_ep_info->pc)
        return 1;

    opteed_rw = optee_ep_info->args.arg0;
    opteed_pageable_part = optee_ep_info->args.arg1;
    opteed_mem_limit = optee_ep_info->args.arg2;
    dt_addr = optee_ep_info->args.arg3;

    opteed_init_optee_ep_state(optee_ep_info,
                opteed_rw,
                optee_ep_info->pc,
                opteed_pageable_part,
                opteed_mem_limit,
                dt_addr,
                &opteed_sp_context[linear_id]);

    bl31_register_bl32_init(&opteed_init);

    return 0;
}
```

</details>

### 4-1-2 源码分析

```C
entry_point_info_t *optee_ep_info;
uint32_t            linear_id;
uint64_t            opteed_pageable_part;
uint64_t            opteed_mem_limit;
uint64_t            dt_addr;
```

这段代码是变量声明，声明后面要使用的临时变量。

`optee_ep_info`：这是定义了一个指向`entry_point_info_t`的指针，这个结构体类型用于描述入口点的信息

`linear_id`：线性 ID，用于表示当前 CPU 或 核心的标识符，每个核心有唯一的一个标识符，这个标识符用于多核的调度和管理。

`opteed_pageable_part`：存储 optee 分页部分的地址和大小。

`opteed_mem_limit`：由于指示 optee 可以使用的内存的上限，用于确保不会超出预先分配内存区域。

`dt_addr`：用于指示设备树的物理地址。设备树是一个数据结构，用于描述硬件布局，用于引导程序和操作系统在启动时了解系统的硬件配置。

---

```C
linear_id = plat_my_core_pos();

optee_ep_info = bl31_plat_get_next_image_ep_info(SECURE);
if (!optee_ep_info) {
    WARN("No OPTEE provided by BL2 boot loader, Booting device"
        " without OPTEE initialization. SMC`s destined for OPTEE"
        " will return SMC_UNK\n");
    return 1;
}
```

`linear_id = plat_my_core_pos();`：获取当前 CPU 的标识符。

`optee_ep_info = bl31_plat_get_next_image_ep_info(SECURE);`：获得下一个 image 的入口点信息，这里就是指 op-tee 的入口信息。返回值保存到 `optee_ep_info`中。获取成功则返回一个指向存有下一个 image 的入口信息的结构体地址的指针。

当获取失败时，打印一条信息之后结束程序。

---

```C
if (!optee_ep_info->pc)
    return 1;
```

判断成功获得的结构体的 PC 指针（这个应该指向入口点地址）是否有效，无效则返回错误代码。

---

```C
opteed_rw = optee_ep_info->args.arg0;
opteed_pageable_part = optee_ep_info->args.arg1;
opteed_mem_limit = optee_ep_info->args.arg2;
dt_addr = optee_ep_info->args.arg3;
```

从`optee_ep_info`中提取必要参数，以便后续函数调用时使用。

---

```C
opteed_init_optee_ep_state(optee_ep_info,
        opteed_rw,
        optee_ep_info->pc,
        opteed_pageable_part,
        opteed_mem_limit,
        dt_addr,
        &opteed_sp_context[linear_id]);
```

`opteed_init_optee_ep_state`函数的目的是初始化 OP-TEE 的入口点状态。这通常包括设置必要的寄存器、内存区域和其他上下文信息，以便在系统启动时正确地跳转到 OP-TEE 的入口点并开始执行。

> 问：既然这里的临时变量并没有进行操作就传入了函数，那为什么直接使用 结构体->内部变量 的方式呢？  
> 答：要是为了提高代码的可读性、简化表达式、方便调试和在某些情况下提高性能。这些好处使得代码更易于理解和维护，减少了出错的可能性。  

---

```C
bl31_register_bl32_init(&opteed_init);
```

注册一个初始化函数，该函数将在 BL31 中调用，用于初始化 BL32（通常是 OP-TEE）。

`&opteed_init`：指向`opteed_init`函数的指针。`opteed_init`是一个函数，用于初始化 BL32（OP-TEE）。

---

## 4.2 opteed_init 源码分析

### 4-2-1 源代码

<details>
    <summary>点击这里展开/收起代码块</summary>

```C

static int32_t opteed_init(void)
{
    uint32_t linear_id = plat_my_core_pos();
    optee_context_t *optee_ctx = &opteed_sp_context[linear_id];
    entry_point_info_t *optee_entry_point;
    uint64_t rc;

    optee_entry_point = bl31_plat_get_next_image_ep_info(SECURE);
    assert(optee_entry_point);

    cm_init_my_context(optee_entry_point);

    rc = opteed_synchronous_sp_entry(optee_ctx);
    assert(rc != 0);

    return rc;
}
```

</details>

### 4-2-2 源码分析

```C
uint32_t linear_id = plat_my_core_pos();
optee_context_t *optee_ctx = &opteed_sp_context[linear_id];
entry_point_info_t *optee_entry_point;
uint64_t rc;
```

`uint32_t linear_id = plat_my_core_pos();`：获取当前 CPU 的标识符，并存在`linear_id`中。

`optee_context_t *optee_ctx = &opteed_sp_context[linear_id];`：获取当前 CPU 的上下文信息，通常包括寄存器状态、内存状态等，以便在不同的执行状态之间进行切换。

---

```C
optee_entry_point = bl31_plat_get_next_image_ep_info(SECURE);
assert(optee_entry_point);
```

`optee_entry_point = bl31_plat_get_next_image_ep_info(SECURE);`获取下一个 image 的入口地址，其中参数 SECURE 表示这个 image 属于安全世界，即 OP-TEE 应当属于安全世界。

---

```C
cm_init_my_context(optee_entry_point);
```

初始化当前 CPU 的上下文，并根据传入的入口点信息设置入口点的状态。

---

```C
rc = opteed_synchronous_sp_entry(optee_ctx);
assert(rc != 0);
```

此函数采用 OPTEE 上下文指针并：

1. 应用来自 optee_ctx->cpu_ctx 的 S-EL1 系统寄存器上下文。
2. 在堆栈框架上保存当前 C 运行时状态（被调用方保存的寄存器）并保存对此状态的引用。
3. 调用 el3_exit()，以便使用来自 optee_ctx->cpu_ctx 的 EL3 系统和通用寄存器进入 OPTEE 映像。

---

## 4.3 opteed_enter_sp 源码解析

### 4-3-1 源代码

<details>
    <summary>点击这里展开/收起代码块</summary>

```asm
func opteed_enter_sp

    mov    x3, sp
    str    x3, [x0, #0]
    sub    sp, sp, #OPTEED_C_RT_CTX_SIZE

    stp    x19, x20, [sp, #OPTEED_C_RT_CTX_X19]
    stp    x21, x22, [sp, #OPTEED_C_RT_CTX_X21]
    stp    x23, x24, [sp, #OPTEED_C_RT_CTX_X23]
    stp    x25, x26, [sp, #OPTEED_C_RT_CTX_X25]
    stp    x27, x28, [sp, #OPTEED_C_RT_CTX_X27]
    stp    x29, x30, [sp, #OPTEED_C_RT_CTX_X29]

    b    el3_exit
endfunc opteed_enter_sp
```

</details>

### 4-3-2 源码分析

```asm
    mov    x3, sp
    str    x3, [x0, #0]
    sub    sp, sp, #OPTEED_C_RT_CTX_SIZE
```

`mov x3, sp`：保存现场，将当前堆栈指针的栈顶指针存放到通用寄存器 x3 中。

`str x3, [x0, #0]`：`str`指令是把寄存器的值存到指定的内存地址处，这里是把 x3 寄存器中的值存到 x0 寄存器包含的内存地址+偏移量为 0 的地址处。

`sub sp, sp, #OPTEED_C_RT_CTX_SIZE`：将栈顶指针向下移动`OPTEED_C_RT_CTX_SIZE`个字节，这是在为 OP-TEE 的 C 函数调用分配堆栈空间。

---

```asm
    stp    x19, x20, [sp, #OPTEED_C_RT_CTX_X19]
    stp    x21, x22, [sp, #OPTEED_C_RT_CTX_X21]
    stp    x23, x24, [sp, #OPTEED_C_RT_CTX_X23]
    stp    x25, x26, [sp, #OPTEED_C_RT_CTX_X25]
    stp    x27, x28, [sp, #OPTEED_C_RT_CTX_X27]
    stp    x29, x30, [sp, #OPTEED_C_RT_CTX_X29]
```

`stp`指令的作用是将两个寄存器的值存储到指定的内存位置中，通过这些指令可以将当前寄存器的状态保存到堆栈上，以便切换上下文时恢复。

---

```asm
    b    el3_exit
```

跳转执行函数 el3_exit，退出 el3 执行级别。
