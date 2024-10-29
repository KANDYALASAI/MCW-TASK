
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

#define IRQ_NO 11  // Define the IRQ number

// Interrupt handler for IRQ 11.
static irqreturn_t irq_handler(int irq, void *dev_id) {
    printk(KERN_INFO "Interrupt Occurred on IRQ %d\n", irq);
    return IRQ_HANDLED;
}

static int __init simple_irq_init(void) {
    // Request the interrupt line
    if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "simple_irq_handler", (void *)(irq_handler))) {
        printk(KERN_ERR "Failed to register IRQ %d\n", IRQ_NO);
        return -1;
    }
    printk(KERN_INFO "Interrupt Handler Registered for IRQ %d\n", IRQ_NO);
    return 0;
}

static void __exit simple_irq_exit(void) {
    free_irq(IRQ_NO, (void *)(irq_handler));  // Free the interrupt line
    printk(KERN_INFO "Interrupt Handler Unregistered for IRQ %d\n", IRQ_NO);
}

module_init(simple_irq_init);
module_exit(simple_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX");
MODULE_DESCRIPTION("Simple Interrupt Handler Example");
MODULE_VERSION("1.0");

