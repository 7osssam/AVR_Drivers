#include "calculator.h"

uint8 exitKey = 1; /* to exit the program */

/*
 * Description: This private function is responsible for displaying the reset menu on most right side of the LCD
 * Inputs: void
 * Output: uint8 (exitKey)
 * if it returns 0, it means that the user pressed 0 and wants to return to the main menu
 * if it returns 1, it means that the user pressed C and wants to reset the calculator
 */
static uint8 ResetMenu(void)
{
	LCD_displayStringRowColumn(0, 11, "C-RES");
	LCD_displayStringRowColumn(1, 11, "0-DEL");

	LCD_sendCommand(LCD_CURSOR_OFF); // turn off the cursor
	LCD_sendCommand(LCD_GO_TO_HOME); // reset the cursor to the home position

	while (1)
	{
		exitKey = KEYPAD_getPressedKey(); // wait for the user to press 0 or hold C

		if (exitKey == 'C')
		{
			LCD_clearScreen();
			exitKey = 1;
			break;
		}
		else if (exitKey == 0)
		{
			LCD_clearScreen();
			exitKey = 0;
			break;
		}
		_delay_ms(300);
	}
	return exitKey;
}

/*
 * Description: This function is responsible for displaying the main menu on the LCD
 * and wait for the user to press 1 or 2 to choose the calculator mode (Normal or Programmer)
 * Inputs: void
 * Output: void
 */
void menu(void)
{
	uint8 option; /* to store the pressed key number */

	LCD_displayStringRowColumn(0, 0, "1-Normal");
	LCD_displayStringRowColumn(1, 0, "2-Programmer");

	option = KEYPAD_getPressedKey(); /* get the pressed key number */

	_delay_ms(500); /* Press time */

	LCD_clearScreen(); /* clear LCD at the beginning */
	// LCD_sendCommand(LCD_GO_TO_HOME); /* Go to home position */

	if (option == 1)
	{
		NormalMode();
	}
	else if (option == 2)
	{
		ProgrammerMode();
	}
	exitKey = 1; /* to reset the exitKey */
}

/*
 * Description: This function is responsible for the normal mode of the calculator (+ - * /)
 * Inputs: void
 * Output: void
 */
void NormalMode(void)
{
	// NormalMode function
	uint8 key = -1;

	uint16 num1 = 0;
	uint16 num2 = 0;
	uint16 result = 0;

	/*
	 * 0 means we didn not enter any operation yet (first number)
	 * 1 means we have entered the first number and we are waiting for the second number
	 */
	uint8 operationFlag = 0;
	uint8 operation = 0;

	while (exitKey != 0)
	{
		// trun on cursor
		LCD_sendCommand(LCD_CURSOR_ON);

		key = KEYPAD_getPressedKey();

		if (key >= 0 && key <= 9)
		{
			if (operationFlag == 0)
			{
				num1 = num1 * 10 + key;
			}
			else if (operationFlag == 1)
			{
				num2 = num2 * 10 + key;
			}
			LCD_displayNumber(key);
		}
		else if (key == '/' || key == '*' || key == '-' || key == '+')
		{
			operationFlag = 1;
			operation = key;
			LCD_displayCharacter(key);
		}
		else if (key == '=')
		{
			key = -1;
			switch (operation)
			{
			case '+':
				result = num1 + num2;
				break;
			case '-':
				result = num1 - num2;
				break;
			case '*':
				result = num1 * num2;
				break;
			case '/':
				result = num1 / num2;
				break;
			}

			LCD_Goto_XY(1, 0);
			LCD_displayCharacter('=');
			LCD_displayNumber(result);
			operationFlag = 0;
			num1 = 0;
			num2 = 0;
			result = 0;

			exitKey = ResetMenu();
		}

		_delay_ms(300);
	}
}

/*
 * Description: This function is responsible for the programmer mode of the calculator (Binary and Hex)
 * Inputs: void
 * Output: void
 */
void ProgrammerMode(void)
{
	// NormalMode function
	uint8 key = -1;
	uint8 num = 0;
	LCD_sendCommand(LCD_GO_TO_HOME);

	while (exitKey != 0)
	{
		LCD_sendCommand(LCD_CURSOR_ON);
		key = KEYPAD_getPressedKey();

		if (key >= 0 && key <= 9)
		{
			num = num * 10 + key;
			LCD_displayNumber(key);
		}
		else if (key == '=')
		{

			LCD_clearScreen();

			LCD_Goto_XY(0, 0);
			LCD_displayString("B:");
			LCD_displayBinary(num);

			LCD_Goto_XY(1, 0);
			LCD_displayString("H:");
			LCD_displayHex(num);

			ResetMenu();
			key = -1;
			num = 0;
		}
		_delay_ms(300);
	}
}