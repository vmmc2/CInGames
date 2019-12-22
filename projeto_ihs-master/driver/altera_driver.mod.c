#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xe00b4984, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x610edd5f, __VMLINUX_SYMBOL_STR(pci_unregister_driver) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0x319ca38e, __VMLINUX_SYMBOL_STR(__pci_register_driver) },
	{ 0x62f4400f, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0x42c8de35, __VMLINUX_SYMBOL_STR(ioremap_nocache) },
	{ 0x879038fc, __VMLINUX_SYMBOL_STR(pci_bus_read_config_dword) },
	{ 0x426b55c9, __VMLINUX_SYMBOL_STR(pci_bus_read_config_byte) },
	{ 0x5c146c0a, __VMLINUX_SYMBOL_STR(pci_enable_device) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xedc03953, __VMLINUX_SYMBOL_STR(iounmap) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v00001172d00000004sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "F14EFF635C67C4625F593FC");
