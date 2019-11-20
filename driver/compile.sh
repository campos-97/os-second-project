#!/bin/bash
make -C /usr/src/linux-headers-$(uname -r) M=$(pwd modules)
rm /lib/modules/4.15.0-70-generic/kernel/drivers/usbdriver/usbdriver.ko
cp usbdriver.ko /lib/modules/4.15.0-70-generic/kernel/drivers/usbdriver/
