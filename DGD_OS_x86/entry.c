#include "console.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "time.h"
#include "pmm.h"
#include "vmm.h"
#include "task.h"
#include "sched.h"
#include "heap.h"
#include "keyboard.h"

void kern_init(void);

uint8_t kern_stack[STACK_SIZE];
uint8_t flag = 0x00;
multiboot_t *glb_mboot_ptr; 

uint32_t kern_stack_top;

uint8_t cursor_x = 0;
uint8_t cursor_y = 0;
uint8_t thread_id[16];

__attribute__((section(".init.data"))) uint32_t stack_address;
__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t*)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low = (pgd_t*)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t*)0x3000;

//__attribute__((section(".init.text"))) 
void kern_entry(){
	//asm volatile ("mov %%ebp, %0" : "=r"(stack_address));

	pgd_tmp[0] = (uint32_t)pte_low |PAGE_PRESENT|PAGE_WRITE;
	pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_high|PAGE_PRESENT|PAGE_WRITE;

	int i;
	for(i = 0; i < 1024; i++){
		pte_low[i] = (i << 12)|PAGE_PRESENT|PAGE_WRITE;
	}

	for(i = 0; i < 1024; i++){
		pte_high[i] = (i<<12)|PAGE_PRESENT|PAGE_WRITE;
	}

	//enable segmentation hardware
	asm volatile ("mov %0, %%cr3" : : "r"(pgd_tmp));

	uint32_t cr0;

	//enable page function hardware
	asm volatile ("mov %%cr0, %0" : "=r"(cr0));

	cr0 = cr0|0x80000000;

	asm volatile ("mov %0, %%cr0" : : "r"(cr0));
	//asm volatile ("mov %%esp, %0" : "=r"(stack_address));
	//stack_address &= 0xFFFFFFFF;
	//stack offset
	kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
	asm volatile("mov %0, %%esp\n"
							 "xor %%ebp, %%ebp" : : "r"(kern_stack_top));

	glb_mboot_ptr = multiboot_ptr_tmp + PAGE_OFFSET;

	kern_init();

}

int thread_1(void);
int thread_2(void);
int thread_3(void);
int thread_4(void);
int thread_5(void);
int thread_6(void);
int thread_7(void);
int thread_8(void);
int thread_9(void);
int thread_10(void);
int thread_11(void);
int thread_12(void);
int thread_13(void);
int thread_14(void);
int thread_15(void);


void kern_init(){
	//extern uint32_t phy_page_count;

	init_debug();

	init_gdt();

	init_idt();

	console_clear();

	
	//printk_color(black, green, "OS kernel running!\n");
	//asm volatile ("int $0x3");
	//asm volatile ("int $0x4");
	//asm volatile ("int $0x20");
	//asm volatile ("int $0x21");

	init_timer(30000);

	init_keyboard();

	uint8_t i;

	for(i = 0; i < 16; i++)
		thread_id[i] = i;

	//uint32_t allc_addr = NULL;

	//printk_color(black, light_blue, "Physcial Memory Allocation Tests\n");

	init_pmm();
	init_vmm();
	init_heap();
	init_sched();
	//init_ide();
	init_keyboard();
	//Syscall_init();

	//print_status();
	printk("\n");


	/*
		face location
		cursor_x = 40;
    	cursor_y = 20;
    */
	kernel_thread((void *)thread_1, NULL);
	kernel_thread((void *)thread_2, NULL);
	kernel_thread((void *)thread_3, NULL);
	kernel_thread((void *)thread_4, NULL);
	kernel_thread((void *)thread_5, NULL);
	kernel_thread((void *)thread_6, NULL);
	kernel_thread((void *)thread_7, NULL);
	kernel_thread((void *)thread_8, NULL);
	kernel_thread((void *)thread_9, NULL);
	kernel_thread((void *)thread_10, NULL);
	kernel_thread((void *)thread_11, NULL);
	kernel_thread((void *)thread_12, NULL);
	kernel_thread((void *)thread_13, NULL);
	kernel_thread((void *)thread_14, NULL);
	kernel_thread((void *)thread_15, NULL);
	//kernel_thread((void *)thread_16, NULL);


	asm volatile("sti");
	//test_ide();

	while(1){
		if((flag & 0xFF) == 0){
			cursor_x = 0;
			cursor_y = 0;
			printk_color(black, light_blue, "        ----------------------   \n");
			printk_color(black, light_blue, "       -------------------------   ");
			printk_color(black, light_red, "     Test Version V_0.1\n");
			printk_color(black, light_green, "     -----------------------------   \n");
			printk_color(black, light_brown, "   ---------------------------------   ");
			printk_color(black, light_red, " Kernel Mode Only for development\n");
			printk_color(black, light_blue, "   ---------------------------------   \n");
			printk_color(black, light_blue, "  -----------------------------------  \n");
			printk_color(black, light_blue, "---------------------------------------\n");
			printk_color(black, light_blue, "---------------------------------------\n");
			printk_color(black, light_blue, "-----     --------------------     ----\n");
			printk_color(black, light_blue, "---------------------------------------");
			printk_color(black, green, " kernel := Kernel for embedded UP Board\n");
			printk_color(black, light_blue, "----------------- ---- ----------------\n");
			printk_color(black, light_blue, "---------------------------------------");
			printk_color(black, green, " Project ECE496 := michaelyuan.feng\n");
			printk_color(black, light_blue, "---------------------------------------\n");
			printk_color(black, light_blue, "---------------------------------------\n");
			printk_color(black, light_blue, " ------------              -----------");
			printk("  kernel in memory start := 0x%08x\n", kern_start);
			printk_color(black, light_blue, "  -----------------------------------");
			printk("   kernel in memory end   := 0x%08x\n", kern_end);
			printk_color(black, light_blue, "   ---------------------------------   ");
			printk(" kernel in memory used  := %d KB\n", (kern_end - kern_start +1023)/1024);
			printk_color(black, light_blue, "     -----------------------------   \n");
			printk_color(black, light_blue, "       -------------------------   \n");
			printk_color(black, light_blue, "         ---------------------   \n");
			flag ++;
			//console_clear();
			//print_cur_status();
		}
	}

}


int thread_1(){
	while(1){
		if((flag & 0xFF) == 1){
			cursor_x = 0;
			cursor_y = 0;
			/*printk_color(black, light_blue, "        ----------------------   \n");
			printk_color(black, light_blue, "       -------------------------   ");
			printk_color(black, light_red, "     Test Version V_0.1\n");
			printk_color(black, light_blue, "     -----------------------------   \n");
			printk_color(black, light_blue, "   ---------------------------------   ");
			printk_color(black, light_red, " Kernel Mode Only for development\n");
			printk_color(black, light_blue, "   ---------------------------------   \n");
			printk_color(black, light_blue, "  -----------------------------------  \n");
			printk_color(black, light_blue, "---------------------------------------\n");
			printk_color(black, light_blue, "---------------------------------------\n");
			printk_color(black, light_blue, "---------------------------------------\n");
			printk_color(black, light_blue, "---------------------------------------");
			printk_color(black, green, " kernel := Kernel for embedded UP Board\n");
			printk_color(black, light_blue, "----------------- ---- ----------------\n");
			printk_color(black, light_blue, "---------------------------------------");
			printk_color(black, green, " Project ECE496 := michaelyuan.feng\n");
			printk_color(black, light_blue, "---------------------------------------\n");
			printk_color(black, light_blue, "---------------------------------------\n");
			printk_color(black, light_blue, " -------------------------------------");
			printk("  kernel in memory start := 0x%08x\n", kern_start);
			printk_color(black, light_blue, "  -----------------------------------");
			printk("   kernel in memory end   := 0x%08x\n", kern_end);
			printk_color(black, light_blue, "   ---------------------------------   ");
			printk(" kernel in memory used  := %d KB\n", (kern_end - kern_start +1023)/1024);
			printk_color(black, light_blue, "     -----------------------------   \n");
			printk_color(black, light_blue, "       -------------------------   \n");
			printk_color(black, light_blue, "         ---------------------   \n");*/
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}

int thread_2(){
	while(1){
		if((flag & 0xFF) == 2){
			cursor_x = 0;
    		cursor_y = 22;
			//printk_color(black, light_brown, "This is A\r");
			flag ++;
			//console_clear();
		}
	}

	return 0;
}

int thread_3(){
	while(1){
		if((flag & 0xFF) == 3){
			cursor_x = 10;
    		cursor_y = 22;
			//printk_color(black, light_green, "This is B\r");
			flag ++;
			//console_clear();
		}
	}

	return 0;
}

int thread_4(){
	while(1){
		if((flag & 0xFF) == 4){
			cursor_x = 20;
    		cursor_y = 22;
			//printk_color(black, light_magenta, "This is C\r");
			flag ++;
			//console_clear();
		}
	}

	return 0;
}

int thread_5(){
	while(1){
		if((flag & 0xFF) == 5){
			cursor_x = 30;
    		cursor_y = 22;
			//printk_color(black, light_green, "This is F\r");
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}

int thread_6(){
	while(1){
		if((flag & 0xFF) == 6){
			cursor_x = 40;
    		cursor_y = 22;
			//printk_color(black, light_brown, "This is G\r");
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}

int thread_7(){
	while(1){
		if((flag & 0xFF) == 7){
			cursor_x = 50;
    		cursor_y = 22;
			//printk_color(black, light_blue, "This is H\r");
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}


int thread_8(){
	while(1){
		if((flag & 0xFF) == 8){
			cursor_x = 60;
    		cursor_y = 22;
			//printk_color(black, light_cyan, "This is I\r");
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}


int thread_9(){
	while(1){
		if((flag & 0xFF) == 9){
			cursor_x = 70;
    		cursor_y = 22;
			//printk_color(black, light_magenta, "This is J\r");
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}


int thread_10(){
	while(1){
		if((flag & 0xFF) == 0xA){
			cursor_x = 0;
    		cursor_y = 22;
			//printk_color(black, light_red, "This is K\r");
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}


int thread_11(){
	while(1){
		if((flag & 0xFF) == 0xB){
			cursor_x = 10;
    		cursor_y = 22;
			//printk_color(black, light_brown, "This is L\r");
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}


int thread_12(){
	while(1){
		if((flag & 0xFF) == 0xC){
			cursor_x = 20;
    		cursor_y = 22;
			//printk_color(black, light_green, "This is M\r");
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}


int thread_13(){
	while(1){
		if((flag & 0xFF) == 0xD){
			cursor_x = 30;
    		cursor_y = 22;
			//printk_color(black, light_blue, "This is N\r");
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}


int thread_14(){
	while(1){
		if((flag & 0xFF) == 0xE){
			cursor_x = 40;
    		cursor_y = 22;
			//printk_color(black, light_cyan, "This is O\r");
			flag ++;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}

int thread_15(){
	while(1){
		if((flag & 0xFF) == 0xF){
			cursor_x = 50;
    		cursor_y = 22;
			//printk_color(black, light_red, "This is P\r");
			flag = 0;
			//console_clear();
				//print_cur_status();
		}
	}

	return 0;
}


