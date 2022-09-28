INSTALLATION
------------

	On Linux
	--------
		Installing GCC on Ubuntu
		------------------------
			Start by updating the packages list:

			$ sudo apt update

			Install the build-essential package by typing:

			$ sudo apt install build-essential

			The command installs a bunch of new packages including gcc, g++ and make.

			You may also want to install the manual pages about using GNU/Linux for development:

			$ sudo apt-get install manpages-dev

			To validate that the GCC compiler is successfully installed,
			use the gcc --version command which prints the GCC version:

			$ gcc --version

			The default version of GCC available in the Ubuntu 20.04 repositories is 9.4.0:

		Installing OpenCV on Ubuntu
		---------------------------
			To install OpenCV on ubuntu you can follow this tutorial:
			https://www.geeksforgeeks.org/how-to-install-opencv-in-c-on-linux/

		Install libcanberra-gtk-module and libcanberra-gtk3-module packages by typing
		-----------------------------------------------------------------------------
			sudo apt install libcanberra-gtk-module libcanberra-gtk3-module


	On Windows
	----------
		You can follow this tutorial for installations:
		https://gist.github.com/vc-30/faeeb3f0790a9781d12233dece71119e


Perfect now we can install the game
-----------------------------------
	First install Puzzle\_Game\_with\_Cpp\_and_OpenCV folder to your computer
	Then go to directory on command line and run commands below
	-----------------------------------------------------------
		$ cd Puzzle_Game_with_Cpp_and_OpenCV/
		$ cmake .
		$ make
		Everything is set, you can launch game by typing:
		$ ./puzzle


How to play
-----------
	Main goal of the game is move blue square to top middle.
	You can move every rectangle by clicking on them.
	If one move is available for that rectangle then it will be played, if two moves are available then game gives you a choice.
	A solution is given below for hint.


https://user-images.githubusercontent.com/75525649/192752844-e6e051ca-fbac-4c10-b04d-c0a11f478ee2.mp4

![solution](https://user-images.githubusercontent.com/75525649/192594709-6afcffb9-45ba-4cbe-bf3a-ddd205ff5b08.png)
