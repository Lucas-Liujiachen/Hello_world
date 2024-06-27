<!-- markdownlint-disable MD033 -->

# ATF启动流程四：bl32

[back](./ATF.md)

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
