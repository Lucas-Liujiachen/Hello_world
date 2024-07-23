# 遇到的问题

[返回](../Download_Guidance.md)

1. 请确保虚拟机被分配了至少600G的存储空间。
   [VMware更改虚拟机存储空间](./More_disk_space.md)
2. 由于bitbake涉及下载内容，请确保虚拟机已经打开代理，从而加快bitbake。
   (加了代理都要下载将近一天，不加下到下辈子吧！)
   [如何为虚拟机的bash设置代理?](./Bash_Proxy.md)
3. 更换VMware到VirtualBox
   [更换虚拟机软件](./Change_VM_to_VirtualBox.md)
4. 通过U-boot挂载虚拟机上的rootfs来验证生成的rootfs是否可行
   [验证rootfs](./Mount_rootfs_from_VM.md)
5. [使用WSL 2，创建 Yocto 项目开发环境](./Use_WSL2_to_create_Yocto.md)
6. U-boot的编译与烧录（非Yocto环境，iMX8M NANO）
   [烧录U-boot](./Compile&burn_uboot.md)
7. TBD
