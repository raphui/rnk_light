#ifndef IRQ_H
#define IRQ_H

struct irq {
	unsigned int num_line;
};


int irq_init(void);
int irq_request(unsigned int irq, void (*handler)(void *), void *arg);

#endif /* IRQ_H */
