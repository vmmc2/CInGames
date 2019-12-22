#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Basic Driver PCIHello");
MODULE_AUTHOR("Victor Miguel de Morais Costa");

//-- Enum: Serve para facilitar minha vida na hora de identificar com qual tipo de input/output eu
//estou lidando.
enum io{HEXPORT, INPORT, GREEN_LEDS, RED_LEDS, BUTTONS, SWITCHES, DISPLAY_7SEG};

//-- Hardware Handles

static void *hexport;  // handle to 32-bit output PIO
static void *inport;   // handle to 16-bit input PIO
static void *green_leds; //ked verde placa
static void *red_leds; //led vermelho placa
static void *buttons; //butoes da placa
static void *switches; //switches da placa
static void *display_7seg; //display de 7 segmentos da placa

//-- Char Driver Interface
static int   access_count =  0;
static int   MAJOR_NUMBER = 91;

static int     char_device_open    ( struct inode * , struct file *);
static int     char_device_release ( struct inode * , struct file *);
static ssize_t char_device_read    ( struct file * , char *,       size_t , loff_t *);
static ssize_t char_device_write   ( struct file * , const char *, size_t , loff_t *);

//estrutura contendo as funções de callback obrigatorias
static struct file_operations file_opts = {
      .read = char_device_read,
      .open = char_device_open,
     .write = char_device_write,
   .release = char_device_release
};

static int char_device_open(struct inode *inodep, struct file *filep) {
   access_count++;
   printk(KERN_ALERT "altera_driver: opened %d time(s)\n", access_count);
   return 0;
}

static int char_device_release(struct inode *inodep, struct file *filep) {
   printk(KERN_ALERT "altera_driver: device closed.\n");
   return 0;
}

static ssize_t char_device_read(struct file *filep, char *buf, size_t len, loff_t *off) {
  //Essa função vai ser a responsável por ler os bytes do dispositivo, e jogar a informação recolhida para 
  //a aplicação.
  //A informação se encontra em: char *buf
  //Eu vou selecionar o tipo de input de acordo com a variável: size_t len.
  //A funcao copy_to_user, como o proprio nome sugere, é responsável por pegar dados do dispositivo
  //e jogá-los para o usuário/aplicação. Relembrando, tais dados se encontram no buffer(variável char *buf)
  if(len == INPORT){
      if(copy_to_user(buf, inport, sizeof(uint32_t))) return -1;
  }else if(len == BUTTONS){
      if(copy_to_user(buf, buttons, sizeof(uint32_t))) return -1;
  }else if(len == SWITCHES){
      if(copy_to_user(buf, switches, sizeof(uint32_t))) return -1;
  }else{
      return -1; //caso aconteça um erro.
  }
  return sizeof(uint32_t);
}

static ssize_t char_device_write(struct file *filep, const char *buf, size_t len, loff_t *off) {
  //Essa função faz o caminho inverso. Ou seja, ele pega a informação (bytes) da aplicação/user e 
  //joga eles para o dispositivo. Para fazer isso, fazemos uso da função: copy_from_user.
  if(len == DISPLAY_7SEG){
      if(copy_from_user(display_7seg, buf, 2*sizeof(uint32_t))) return -1;
  }else if(len == HEXPORT){
      if(copy_from_user(hexport, buf, 2*sizeof(uint32_t))) return -1;
  }else if(len == RED_LEDS){
      if(copy_from_user(red_leds, buf, 2*sizeof(uint32_t))) return -1;
  }else if(len == GREEN_LEDS){
      if(copy_from_user(green_leds, buf, 2*sizeof(uint32_t))) return -1;
  }else{
      return -1; //caso dê algum erro.
  }
  return sizeof(uint32_t);
}

//-- PCI Device Interface

static struct pci_device_id pci_ids[] = {
  { PCI_DEVICE(0x1172, 0x0004), },
  { 0, }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id);
static void pci_remove(struct pci_dev *dev);

static struct pci_driver pci_driver = {
  .name     = "alterahello",
  .id_table = pci_ids,
  .probe    = pci_probe,
  .remove   = pci_remove,
};

static unsigned char pci_get_revision(struct pci_dev *dev) {
  u8 revision;
  pci_read_config_byte(dev, PCI_REVISION_ID, &revision);
  return revision;
}

static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id) {
  int vendor;
  int retval;
  unsigned long resource;

  retval = pci_enable_device(dev);
  
  if (pci_get_revision(dev) != 0x01) {
    printk(KERN_ALERT "altera_driver: cannot find pci device\n");
    return -ENODEV;
  }

  pci_read_config_dword(dev, 0, &vendor);
  printk(KERN_ALERT "altera_driver: Found Vendor id: %x\n", vendor);

  resource = pci_resource_start(dev, 0);
  printk(KERN_ALERT "altera_driver: Resource start at bar 0: %lx\n", resource);


  //Colocar os enderecos das outras coisas: red led, green led, buttons
  hexport = ioremap_nocache(resource + 0XC000, 0x20);
  inport  = ioremap_nocache(resource + 0XC140, 0x20);
  //Colocar depois os enderecos que estao no Qsys do Quartus 15
  //Como trabalhamos só com 32 bits. Temos que: 0x20.
  green_leds = ioremap_nocache(resource + 0XC080 , 0x20);
  red_leds = ioremap_nocache(resource + 0XC060 , 0x20);
  switches = ioremap_nocache(resource + 0XC200, 0x20);
  buttons = ioremap_nocache(resource + 0XC100, 0x20);
  display_7seg = ioremap_nocache(resource + 0XC040, 0x20);
  return 0;
}

static void pci_remove(struct pci_dev *dev) {
  iounmap(hexport);
  iounmap(inport);
  iounmap(green_leds);
  iounmap(red_leds);
  iounmap(switches);
  iounmap(buttons);
  iounmap(display_7seg);
}


//-- Global module registration
//funciona como a função main()
static int __init altera_driver_init(void) {
   int t = register_chrdev(MAJOR_NUMBER, "de2i150_altera", &file_opts);
   t = t | pci_register_driver(&pci_driver);

   if(t<0)
      printk(KERN_ALERT "altera_driver: error: cannot register char or pci.\n");
   else
     printk(KERN_ALERT "altera_driver: char+pci drivers registered.\n");

   return t;
}

//função chamada quando a gente termina de usar o device. funciona
//quase como um destrutor
static void __exit altera_driver_exit(void) {
  printk(KERN_ALERT "Goodbye from de2i150_altera.\n");

  unregister_chrdev(MAJOR_NUMBER, "de2i150_altera");
  pci_unregister_driver(&pci_driver);
}

module_init(altera_driver_init);
module_exit(altera_driver_exit);
