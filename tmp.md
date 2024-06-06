# 记事本

UUU烧录命令

`sudo uuu ~/mfgtools/uuu.auto-imx8mnddr4evk`

## 复制必要的文件

     # uboot-imx
 cp uboot-imx-lf_v2022.04/tools/mkimage                                     ./imx-mkimage/iMX8M/mkimage_uboot
 cp uboot-imx-lf_v2022.04/arch/arm/dts/imx8mn-ddr4-evk.dtb                  ./imx-mkimage/iMX8M/imx8mn-ddr4-evk.dtb
 cp uboot-imx-lf_v2022.04/spl/u-boot-spl.bin                                ./imx-mkimage/iMX8M/
 cp uboot-imx-lf_v2022.04/u-boot-nodtb.bin                                  ./imx-mkimage/iMX8M/
 ​
     # firmware-imx  在这里区分使用 LPDDR4 还是 DDR4，详见 "分析 imx-mkimage" 小节
 cp firmware-imx-8.10/firmware/ddr/synopsys/ddr4_dmem_1d.bin    ./imx-mkimage/iMX8M/
 cp firmware-imx-8.10/firmware/ddr/synopsys/ddr4_dmem_2d.bin    ./imx-mkimage/iMX8M/
 cp firmware-imx-8.10/firmware/ddr/synopsys/ddr4_imem_1d.bin    ./imx-mkimage/iMX8M/
 cp firmware-imx-8.10/firmware/ddr/synopsys/ddr4_imem_2d.bin    ./imx-mkimage/iMX8M/
 cp firmware-imx-8.10/firmware/ddr/synopsys/ddr4_imem_1d_201810.bin    ./imx-mkimage/iMX8M/
 cp firmware-imx-8.10/firmware/ddr/synopsys/ddr4_imem_2d_201810.bin    ./imx-mkimage/iMX8M/
 cp firmware-imx-8.10/firmware/ddr/synopsys/ddr4_dmem_1d_201810.bin    ./imx-mkimage/iMX8M/
 cp firmware-imx-8.10/firmware/ddr/synopsys/ddr4_dmem_2d_201810.bin    ./imx-mkimage/iMX8M/
 ​
     # imx-atf
 cp imx-atf/build/imx8mn/release/bl31.bin                       ./imx-mkimage/iMX8M/
