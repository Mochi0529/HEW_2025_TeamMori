#include "KadaiGameObject.h"
#include "Scene.h"
#include <iostream>

int MochiFramework::GameObjects::KadaiGameObject::mObjectCounter = 0;

void MochiFramework::GameObjects::KadaiGameObject::DisplayCounter(void)
{
	system("cls");
	std::cout << mObjectCounter;
}
