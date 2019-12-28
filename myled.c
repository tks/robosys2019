#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Kanta  Takasu");
MODULE_DESCRIPTION("driver for Mini-Traffic-Light");
MODULE_LICENSE( "GPL" );
MODULE_VERSION( "0.1" );

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;
static const u32 h_green  = 0x01;
static const u32 h_red    = 0x02;
static const u32 c_green  = 0x04;
static const u32 c_yellow = 0x08;
static const u32 c_red    = 0x10;
static const u32 all_led  = 0x7f;
//0x01      1 gpio20  green   for human
//0x02     10 gpio21  red     for human
//0x04    100 gpio22  green
//0x08   1000 gpio23  yellow
//0x010 10000 gpio24  red

void change_led(u32 car, u32 human, int lighting_span){
    gpio_base[10] = all_led << 20;
    gpio_base[7]  = (car + human) << 20;
    msleep(lighting_span);
}

void change_led(u32 car, int lighting_span){
    gpio_base[10] = all_led << 20;
    gpio_base[7]  = car << 20;
    msleep(lighting_span);
}

void change_green(void){
    // car red and human blink 
    int i;
    for ( i = 0; i < 8 ; i++ ){
        change_led(c_red, h_green , 300);
        change_led(c_red,           300);
    }
    change_led(c_red, h_red ,1200);
    change_led(c_green, h_red, 100);
}

void change_red(void){
    change_led(c_yellow, h_red, 900);
    change_led(c_red, h_red, 900);
    change_led(c_red, h_green, 100);
}
static ssize_t led_write( struct file* filp, const char* buf, size_t count, loff_t* pos ) {
    char c;
    if( copy_from_user(&c, buf, sizeof(char)) ) return -EFAULT;

    switch( c ){
        case '0':
            change_green();
            break;
        case '1':
            change_red();
            break;

        default:
            break;
    }

    return 1;
}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .write = led_write,
};

static int __init init_mod( void ){
    int retval;

    gpio_base = ioremap_nocache( 0x3f200000, 0xA0 );
    gpio_base[2]=0xffe49249;
    retval = alloc_chrdev_region( &dev, 0, 1, "myled" );
    printk( KERN_INFO "%s is loaded. major:%d\n", __FILE__, MAJOR(dev) );

    cdev_init( &cdv, &led_fops );
    retval = cdev_add( &cdv, dev, 1 );
    if( retval < 0 ) {
        printk( KERN_ERR "cdev_add failed. major:%d, minor:%d", MAJOR(dev), MINOR(dev) );
        return retval;
    }
    cls = class_create( THIS_MODULE, "myled" );
    if( IS_ERR(cls) ) {
        printk( KERN_ERR "class_create failed." );
        return PTR_ERR( cls );
    }
    device_create( cls, NULL, dev, NULL, "myled%d", MINOR(dev) );
    return 0;
}

static void __exit cleanup_mod( void ){
    cdev_del( &cdv );
    device_destroy( cls, dev );
    class_destroy( cls );
    unregister_chrdev_region( dev, 1 );
    printk( KERN_INFO "%s is unloaded. major:%d\n", __FILE__, MAJOR(dev) );
}

module_init( init_mod );
module_exit( cleanup_mod );
