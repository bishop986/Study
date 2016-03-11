#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	if(argc == 1) {
		printf("Please enter game -h ask for help.\n");
		return 0;
	}

	if(argv[1][1] == 'h') {
		printf("--------------------Game list-------------------------\n");
		printf("1.-dota will start Dota2\n");
		printf("2.-civil will start Clivaliztion V\n");
		printf("3.-aoe will start Age of Empire III\n");
		printf("4.-ra will start Red Alert\n");
		printf("5.-skyrim will start Elder Scoer V\n");
		printf("6.-steam will start steam\n");
		printf("-----------------------end-----------------------------\n");
		return 0;
	}

	if ( !strcmp( argv[1], "-dota"))
	{
		system("start D:\\Progra~1\\Dota2\\dota2launcher.exe");
		return 0;

	} else if ( !strcmp( argv[1], "-civil")) {

		system("start F:\\DoyoGames\\Games\\10102949\\10102949\\Launcher.exe");
		return 0;

	} else if ( !strcmp( argv[1], "-aoe")) {

		system("start D:\\Progra~1\\AOE3\\age3y");
		return 0;

	} else if ( !strcmp( argv[1], "-ra")) {

		system("start D:\\Progra~1\\ElectronicArts\\RedAlert3\\RA3.exe ");
		return 0;

	} else if ( !strcmp( argv[1], "-skyrim")) {

		system("start F:\\DoyoGames\\Games\\10450484\\10450484\\SkyrimLauncher.exe");
		return 0;

	} else if ( !strcmp( argv[1], "-steam")) {

		system("start D:\\Progra~1\\Steam\\Steam.exe");
		return 0;

	} else {

		printf("No such commad! Please enter game -h ask for help.\n");
		return 0;
	}

	return 0;
}
