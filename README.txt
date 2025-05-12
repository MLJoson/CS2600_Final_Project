CS2600 Final Project

Group Members: Marco Joson, Chelsea Ocampo, Lucia Maturino Iniguez, Brandon Romero, Ethan Nhan

This project is to create an application/game based off of the mobile game "Doodle Jump". Using what we have learned about C and
SDL, we were able to replicate the basic mechanics of the "Doodle Jump" and create a functioning menu and end screen. 

"CMake REQUIRED"

Librarires required:
	SDL3.lib
	SDL3_ttf.lib

Installation process:
	Run the 'install_SDL3_SDL3_ttf.sh' script to install all necessary libraries to run the application.

Running the application:
	1) Run the 'temp_var.sh' with '. temp_var.sh' script to set environment variables to run the game. Must set these variables each terminal session. 
	2) Then, run the makefile using 'make' and start the 'main' executable.
	3) When the application starts, the main menu screen shows up displaying the title, start button, and quit button.
		a. Press the start button to begin playing the game.
		b. Or, press the quit button to close the application.
	4) After play is pressed, the application goes to the game screen. The player moves the black box with left/right arrows to jump on top of the
	platforms. The higher the player goes up, the more points they earn. Continue getting as high of a score as possible until the player somehow hits
	the bottom of the screen, ending the game.
		a. From here, the final score is displayed, along with a replay and quit button, which will start the game again or close the application resepectively.

Uninstalling libraries:
	To uninstall the libraries, run the 'uninstall.sh' script to remove SDL3 libraries.
