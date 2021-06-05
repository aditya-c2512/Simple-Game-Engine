#include "AppWindow.h"

int main()
{
	AppWindow app;
	if (app.init())//IF APP WINDOW INITIALIZATION IS SUCCESSFUL
	{
		while (app.isRun())//IF APP WINDOW IS CURRENTLY RUNNING
		{
			app.broadcast();
		}
	}
	return 0;
}