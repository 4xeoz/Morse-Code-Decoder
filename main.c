#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include "pico/stdlib.h"

#include "includes/seven_segment.h"

#define BUTTON_PIN 16	


// an array called"morse" to  represent the morse code and corresponding alphabet.

char* MORSE[] = {
".-",// A
"-...",// B
"-.-.", // C
"-..", // D
".", // E
"..-.", // F
"--.", // G
"....", // H
"..", // I
".---", // J
"-.-", // K
".-..", // L
"--", // M
"-.", // N
"---", // O
".--.", // P
"--.-", // Q
".-.", // R
"...", // S
"-", // T
"..-", // U
"...-", // V
".--", // W
"-..-", // X
"-.--", // Y
"--.." // Z
};

// an array called ALPHABET that store a list of Alphabet
// the two arrays here allow us to map Morse code inputs to their respective characters
char* ALPHABET[] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X", "Y","Z"};

//the beginning part of the program
int main() {
	
	stdio_init_all();// Initialise all standard input/output interfaces.
	gpio_init(BUTTON_PIN); // Initialise gpio pin for the buttons
	gpio_set_dir(BUTTON_PIN, GPIO_IN);// set the pin as an input
	gpio_pull_down(BUTTON_PIN);// ennable pulldown resistor for the button pin.
	seven_segment_init();// initialse the display segments
	seven_segment_off();// make sure segments are all off at the start
	int number = 0;
	bool cur_pressed = false;

	// Variable declarations for time tracking and Morse code processing.
	struct timeval start_time;
	struct timeval end_time;
	gettimeofday(&start_time, NULL);// Get the current time.
	gettimeofday(&end_time, NULL);//initialise the ende time with the current time 
	long int elapsed_time = 0;// store the elapsed time in between the button being pressed.
	char letter[10] = "";
	char word[10] = ""; 
	bool working = false; // we use this indicate we input a valid mouse code or not
	bool bufferWorking = true;// this is to track if the button is currently pressed.
	
	// the welcome statement and show the "-" at the beginning of the program
	printf("Welcome to the morse code program");
	seven_segment_show(27);


	while (true){

		//using the flags we created
		//to check if we are pressing the button to create input signal or not
		if (elapsed_time > 1000000 && cur_pressed == false && bufferWorking == true){
			working = false;

			//looping throught the morse array to find the matching input(the dot and dashes)
			// in here , we are using strcmp method the map the values
			for (int i = 0; i < 26; i++){
				if ( strcmp(letter, MORSE[i]) == 0){
					printf(ALPHABET[i]);
					strcpy(letter, "");// this should clear the letter buffer for new input
					seven_segment_show(i);// calling the method to display the letters on the 7segments
					strcat(word, ALPHABET[i]);// append the letter to the word.
					working = true; // Set the flag to indicate the successful match
				}
			}
			printf(word);// it print the decoded word
		}

		// this part is for "error handling"
		if (working == false && elapsed_time > 1000000 && bufferWorking == true && cur_pressed == false){
				printf("Error: Morse code does not correspond to any chracter\n");
				strcpy(letter, ""); // clear the letter buffer
				seven_segment_show(26);//display the error indication on the 7 segment(26 refer to "8")
			}
			       
		
		 // reset the flag if the button has not been pressed for a certain time
		if (elapsed_time > 1000000 && cur_pressed == false){
			bufferWorking = false;
		}
		
		 // this calculate the time since the last button press or release
		elapsed_time= (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
		
		// Detect if the button being pressed.
		if(gpio_get(BUTTON_PIN) && cur_pressed == false){			
			gettimeofday(&start_time, NULL);// Update start_time to the current time.
			cur_pressed = true;// set it true to indicate it is being pressed
		}

		// it detect if the button release and process the morse code signal
		else if(!(gpio_get(BUTTON_PIN)) && cur_pressed == true){
			if (elapsed_time < 250000){
				strcat(letter, ".");// Append a dot for a short press.
			}
			else if (elapsed_time < 1000000){
				strcat(letter, "-");// Append a dash for a long press.
			}
			else{
				printf("Error: Button held for too long");// Error message if we preess the button for too long
				seven_segment_show(26);// 26 refer to "8", which is the error indication on 7segment
			}
			gettimeofday(&start_time, NULL);  // reset time for the next press
			cur_pressed = false;//reset the flag
			printf(letter);// print the current Morse code letter.
			printf("\n");// start a new line
			bufferWorking = true;// reset the flag
		}
		gettimeofday(&end_time, NULL);//simply update the end time
		sleep_ms(10);
	}
	return 0;
}

//time template functionality from https://stackoverflow.com/questions/75304951/why-the-following-c-code-is-not-outputting-the-execution-time-in-raspberry-pi-pi