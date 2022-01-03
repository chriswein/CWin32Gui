# Boilerplate C Win32 Api Application 

This is a example application using the Win32 Api with C. 

The application examplifies creating a window with multiple buttons and a textfield. It handles the button events and accesses the text field and it's content.

## Compiling with MinGW and GCC 

To compile the application with gcc use the flags -mwindows and -municode

Example:

	gcc win.c -o win.exe -mwindows -municode