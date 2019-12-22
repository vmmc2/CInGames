make
sudo insmod altera_driver.ko
dmesg
sudo mknod /dev/de2i150_altera c 91 1
