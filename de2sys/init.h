
#ifndef included_init
#define included_init

/****************************************************************************************
* check initialized
****************************************************************************************/
void init_check(alt_up_parallel_port_dev *dev, char *str)
{
	if (dev == NULL) {
		alt_printf("Error: could not open %s device\n",str);
	}
	else
	{
		alt_printf("Opened %s device \n",str);
	}
}

void init_device(alt_up_pixel_buffer_dma_dev *pixel_dev, alt_up_parallel_port_dev *char_buf_dev)
{
	alt_up_char_buffer_clear(char_buf_dev);
	alt_up_pixel_buffer_dma_clear_screen(pixel_dev, 0);
}

// for red and green LEDs test
void LEDrot(alt_up_parallel_port_dev *LEDG_dev)
{
	unsigned int i;
	for (i = 0; i < 16; i++) {
		alt_up_parallel_port_write_data(LEDG_dev, GREEN_LEDs[i % 8]);
		delay(WAIT_SEC / 2);
	}
	alt_up_parallel_port_write_data(LEDG_dev, LEDOFF);
}

#define redLEDrot LEDrot
#define greenLEDrot LEDrot


#endif