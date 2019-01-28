

#ifndef included_mod
#define included_mod

// 1ms delay
void delay(int count)
{
	usleep(1000*count);
}

/*
 *
 *  print seven segment LEDs
 *	print_Hex0to3(Hex3to2,Hex1to0)
 *		+	Hex3to2 :: nHex :: unsigned char
 *		+	Hex1to0 :: nHex	:: unsigned char
 *
 */

void print7seg(unsigned char b1, unsigned char b2)
{
	/* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
	* a single 7-seg display in the DE2 Media Computer, for the hex digits 0 - F */
	unsigned char	seven_seg_decode_table[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07,
		0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };
	unsigned char	hex_segs[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned int shift_buffer, nibble;
	unsigned char code;
	int i;

	shift_buffer = (b1 << 8) | b2;
	for (i = 0; i < 6; ++i)
	{
		nibble = shift_buffer & 0x0000000F;		// character is in rightmost nibble
		code = seven_seg_decode_table[nibble];
		hex_segs[i] = code;
		shift_buffer = shift_buffer >> 4;
	}

	/* drive the hex displays */
	HEX3_HEX0_DATA_REG = *(int *)(hex_segs);
	//*(HEX7_HEX4_ptr) = *(int *)(hex_segs + 4);
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


/*
 *
 *  print LCD
 *	printLCD("top str","bottom str",LCD_dev)
 *		+	top_str[LCD_STR_MAX]
 *		+	bot_str[LCD_STR_MAX]
 *		+	LCD_STR_MAX == 16
 *
 */

void printLCD(char *str_top, char *str_bot, alt_up_character_lcd_dev *LCD)
{
	// set LCD cursor location to top row
	alt_up_character_lcd_set_cursor_pos(LCD, 0, 0);
	alt_up_character_lcd_string(LCD, str_top);
	alt_up_character_lcd_set_cursor_pos(LCD, 0, 1);	// set LCD cursor location to bottom row
	alt_up_character_lcd_string(LCD, str_bot);
	alt_up_character_lcd_cursor_off(LCD);				// turn off the LCD cursor 
}



#endif