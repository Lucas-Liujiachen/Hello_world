
U-Boot SPL 2022.04 (Jun 28 2024 - 09:46:12 +0800)
DDRINFO: start DRAM init
DDRINFO: DRAM rate 2400MTS
DDRINFO:ddrphy calibration done
DDRINFO: ddrmix config done
SEC0:  RNG instantiated
Normal Boot
Trying to boot from BOOTROM
Boot Stage: USB boot
Find img info 0x&48025fa0, size 864
Need continue download 1024
Download 1998288, Total size 1999408
NOTICE:  BL31: v2.2(release):rel_imx_5.4.70_2.3.6-0-g15e8ff164
NOTICE:  BL31: Built : 17:33:36, Jun 21 2024


U-Boot 2022.04 (Jun 28 2024 - 09:46:12 +0800)

CPU:   i.MX8MNano Quad rev1.0 1500 MHz (running at 1200 MHz)
CPU:   Commercial temperature grade (0C to 95C) at 38C
Reset cause: POR
Model: NXP i.MX8MNano DDR4 EVK board
DRAM:  2 GiB
TCPC:  Vendor ID [0x1fc9], Product ID [0x5110], Addr [I2C1 0x52]
Power supply on USB2
TCPC:  Vendor ID [0x1fc9], Product ID [0x5110], Addr [I2C1 0x50]
Core:  159 devices, 27 uclasses, devicetree: separate
MMC:   FSL_SDHC: 1, FSL_SDHC: 2
Loading Environment from nowhere... OK
[*]-Video Link 0adv7535_mipi2hdmi adv7535@3d: Can't find cec device id=0x3c
fail to probe panel device adv7535@3d
mxs_video lcd-controller@32e00000: failed to get any video link display timings
probe video device failed, ret -22

        [0] lcd-controller@32e00000, video
        [1] dsi_controller@32e10000, video_bridge
        [2] adv7535@3d, panel
adv7535_mipi2hdmi adv7535@3d: Can't find cec device id=0x3c
fail to probe panel device adv7535@3d
mxs_video lcd-controller@32e00000: failed to get any video link display timings
probe video device failed, ret -22
In:    serial
Out:   serial
Err:   serial
SEC0:  RNG instantiated

 BuildInfo:
  - ATF 15e8ff1

MMC: no card present
Detect USB boot. Will enter fastboot mode!
Net:   eth0: ethernet@30be0000
Fastboot: Normal
Boot from USB for mfgtools
*** Warning - Use default environment for                                mfgtools
, using default environment

Run bootcmd_mfg: run mfgtool_args;if iminfo ${initrd_addr}; then if test ${tee} = yes; then bootm ${tee_addr} ${initrd_addr} ${fdt_addr}; else booti ${loadaddr} ${initrd_addr} ${fdt_addr}; fi; else echo "Run fastboot ..."; fastboot 0; fi;
Hit any key to stop autoboot:  0

## Checking Image at 43800000 ...
Unknown image format!
Run fastboot ...
Detect USB boot. Will enter fastboot mode!
flash target is MMC:1
MMC: no card present
MMC card init failed!
MMC: no card present
** Block device MMC 1 not supported
Detect USB boot. Will enter fastboot mode!
flash target is MMC:2
switch to partitions #0, OK
mmc2(part 0) is current device
Detect USB boot. Will enter fastboot mode!
Starting download of 16776232 bytes
..........................................................................
.....................................................
downloading of 16776232 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776232
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
.....................................................
downloading of 16776244 bytes finished
writing to partition 'all'
sparse flash target is mmc:2
writing to partition 'all' for sparse, buffer size 16776244
Flashing sparse image at offset 0
Flashing Sparse Image
........ wrote 16776192 bytes to 'all'
Starting download of 16776244 bytes
..........................................................................
