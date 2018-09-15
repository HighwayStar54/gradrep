/*

Written by Ulisse "HighwayStar54" Benedetti

*/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdio>
#include <chrono>
#include <Windows.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_native_dialog.h>

constexpr int MINOR_ERROR = 0xe;
constexpr int FATAL_ERROR = 0xc;

int main(int argc, char * argv[]) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	std::string timestamp = __TIMESTAMP__;
	std::string help =
		"\nGRADREP v1.0\t Made by Ulisse Benedetti using Allegro 5, compiled: " + timestamp +
		"\n\nSyntax: GRADREP\t[PathToFile]\t[Flags]"
		"\n\n\t/F\tOpens the window in fullscreen mode, not compatible with /FW."
		"\n\t/FW\tOpens the window in fullscreen-windowed mode, not compatible with /F."
		"\n\t/S\tSkips space characters."
		"\n\t/N\tSimulates newlines."
		"\n\t/NC\tHides console until the window is closed."
		"\n\t/new\tRuns the program from another cmd instance."
		"\n\t/newc\tRuns the program from another cmd instance and closes the old one."
		"\n\n\t/R\tDraws the pixels in a red gradient."
		"\n\t/G\tDraws the pixels in a green gradient."
		"\n\t/B\tDraws the pixels in a blue gradient."
		"\n\n\tFlags can be typed in any order, color flags can be combined.\n";

#pragma region
	if(argc < 2) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MINOR_ERROR);
		std::cout << "\nNot enough arguments. USAGE: GRADREP  [PathToFile] [Flags]\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
		return -1;
	}
	std::string fargbuff = argv[1];
	if(fargbuff == "/?") {
		std::cout << help;
		return 0;
	}

	int
		width,
		height;

	bool
		fullscreen = false,
		fullwndwed = false,
		spaces = false,
		newlines = false,
		red = false,
		green = false,
		blue = false;

	bool invalid_flag = false;

	char ch;
	long int iter = 0; //This is gonna be our string iterator.

	if(argc > 2) { //Dynamic flag detection.
		for(int i = 2; i < argc; i++) {
			std::string buff = argv[i];
			if(buff == "/F") {
				fullscreen = true;
			}
			else if(buff == "/FW") {
				fullwndwed = true;
			}
			else if(buff == "/S") {
				spaces = true;
			}
			else if(buff == "/N") {
				newlines = true;
			}
			else if(buff == "/R") {
				red = true;
			}
			else if(buff == "/G") {
				green = true;
			}
			else if(buff == "/B") {
				blue = true;
			}
			else if(buff == "/NC") {
				ShowWindow(GetConsoleWindow(), SW_HIDE);
			}
			else if (buff == "/new") {
				std::string argset;
				for (int i = 0; i < argc; i++) {
					if (std::string(argv[i]) != "/new") //Collects the command-line arguments that were used, but opts out /new.
						argset.append(std::string(argv[i]) + char(32)); //Copies the new set of arguments to a new string.
					else {}
				}
				system(("start" + argset).c_str()); //Uses the generated string to run gradrep in a new cmd window.
			}
			else if (buff == "/newc") {
				std::string argset;
				for (int i = 0; i < argc; i++) {
					if (std::string(argv[i]) != "/newc")
						argset.append(std::string(argv[i]) + char(32));
					else {}
				}
				system(("start" + argset).c_str());
				return EXIT_SUCCESS; //This conditional is the same as the one before, but closes the original cmd window too.
			}
			else if(buff == "/?"){
				std::cout << "ERROR: Invalid syntax.\nFor help, type GRADREP /?";
				return MINOR_ERROR;
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MINOR_ERROR);
				std::cout << "\nInvalid flag: " << buff << '\n';
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
				invalid_flag = true;
			}
		}
		if(fullscreen && fullwndwed) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MINOR_ERROR);
			std::cout << "\nInvalid set of flags: /F and /FW\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
			return -1;
		}
	}
	if(invalid_flag) {
		return -6;
	}
	if(!red & !green & !blue) {
		red = true;
		green = true;
		blue = true;
	}

	if(!al_init()) { //If something went wrong when initializing allegro, an error is printed and a message box is shown.
		al_show_native_message_box(al_get_current_display(), "FATAL ERROR!", "!al_init() is TRUE.", "Allegro failed to initialize.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FATAL_ERROR);
		std::cout << "\nal_init error: " << stderr << '\n';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
		return -2;
	}

	if(!al_init_primitives_addon()) { //If something went wrong when initializing the primitives addon, an error is printed and a message box is shown.
		al_show_native_message_box(al_get_current_display(), "FATAL ERROR!", "Allegro primitives addon failed to initialize.",NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FATAL_ERROR);
		std::cout << "\nal_init_primitives_addon error: " << stderr << '\n';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
		return -3;
	}

	if(!al_init_image_addon()) { //If something went wrong when initializing the image addon, an error is printed and a message box is shown.
		al_show_native_message_box(al_get_current_display(), "FATAL ERROR!", "Allegro image addon failed to initialize.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FATAL_ERROR);
		std::cout << "\nal_init_image_addon error: " << stderr << '\n';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
		return -4;
	}

	ALLEGRO_EVENT Ev;
	ALLEGRO_EVENT_QUEUE * EvQ = al_create_event_queue();
	if(!EvQ) { //If something went wrong when creating an event queue, an error is printed and a message box is shown.
		al_show_native_message_box(al_get_current_display(), "FATAL ERROR!", "Failed to create event queue!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FATAL_ERROR);
		std::cout << "\nal_create_event_queue error: " << stderr << '\n';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
		return -5;
	}

	if(!al_install_keyboard()) { //If keyboard installation failed, an error is printed and a message box is show.
		al_show_native_message_box(al_get_current_display(), "FATAL ERROR!", "Failed to install keyboard!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FATAL_ERROR);
		std::cout << "\nal_install_keyboard error: " << stderr << '\n';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
		return -8;
	}

// #pragma region
	std::string * str = new std::string; //Allocating this string into the heap, as a file can easily be too large for the stack.
	std::ifstream rd(argv[1], std::ios::binary); //Without the ios::binary flag every byte belonging to a non-narrow character would be skipped when reading.
	if(!rd.is_open()) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MINOR_ERROR);
		std::cout << "\nInvalid file or file is already open: " << argv[1] << '\n';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
		return 128; //If file failed to open, the standard windows error is thrown.
	}
	str->assign((std::istreambuf_iterator<char>(rd)), (std::istreambuf_iterator<char>())); //Reads from the specified file and stores its contents in str.
	if(str->length() < 1) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), MINOR_ERROR);
		std::cout << "\nFile can't be empty!\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
		return 128;
	}

	ALLEGRO_DISPLAY * Disp = NULL;
	ALLEGRO_DISPLAY_MODE Mode; //This is only useful if the fullscreen options is specified.

	if(fullscreen || fullwndwed) { //Creates a display(window) using fullscreen settings, given fullscreen or fullwndwed is true.
		al_get_display_mode(al_get_num_display_modes() - 1, &Mode); //Gets the highest resolution possible for the screen.
		width = Mode.width;
		height = Mode.height;
		if(fullwndwed) {
			al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
		}
		if(fullscreen) {
			al_set_new_display_flags(ALLEGRO_FULLSCREEN);
		}
		Disp = al_create_display(width, height);
	}
	else { //If !fullscreen, it proceeds to create a display large enough to show all of the characters.
		width = sqrt(str->length());
		height = sqrt(str->length());
		Disp = al_create_display(width, height);
	}
	if(!Disp) { //If something went wrong with the display creation, an error is printed and a message box is show.
		al_show_native_message_box(al_get_current_display(), "FATAL ERROR!", "Failed to create a window!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FATAL_ERROR);
		std::cout << "\nal_create_display error: " << stderr << '\n';
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes);
		return -7;
	}

	auto start = std::chrono::high_resolution_clock::now(); //The drawing operation is timed starting from here.

	for(int Y = 1; Y <= height; Y++) {
		for(int X = 1; X <= width && iter < str->length(); X++) {
			ch = str->at(iter);
			if(ch == '\n' && newlines) {
				Y++;
				X = 1;
			}
			else if(ch == 32 && spaces) {}
			else {
					al_draw_pixel(X, Y, al_map_rgb(ch * red, ch * green, ch * blue));
			}
			iter++;
		}
	}

	auto end = std::chrono::high_resolution_clock::now(); //Timer ends here.

	al_flip_display(); //The pixels drawn in the backbuffer are shown on the display.

	std::cout //Operation report is printed.
		<< "\nWindow width: " << width
		<< "\nWindow heigh: " << height;
	std::cout
		<< "\nFile size: " << str->length()
		<< "\nTime taken: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f << "ms\n";

	delete str; //Deallocates str, as it's no longer needed. If the file was big, it's good to get rid of the used memory ASAP.

	al_register_event_source(EvQ, al_get_display_event_source(Disp)); //Registers all the event sources for the display window to be properly closed.
	al_register_event_source(EvQ, al_get_keyboard_event_source());

	do {
		al_wait_for_event(EvQ, &Ev);
		if(Ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			break;
		}
	} while(Ev.type != ALLEGRO_EVENT_DISPLAY_CLOSE || Ev.type == ALLEGRO_EVENT_KEY_DOWN);

	al_destroy_display(Disp);
	al_destroy_event_queue(EvQ);
	ShowWindow(GetConsoleWindow(), SW_SHOW); //When everything is done, shows the CMD window again if it was hidden before.
	return EXIT_SUCCESS; //Happy ending.
}
