/* Name: AntZerg
   Author: Tarik Sturm-Dahal

 Copyright: Copyright (c) <2010> <Tarik Sturm-Dahal>
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 Description: AntZerg is a small C++/Lua simulation using swarm intelligence & behavior trees to model
 an ant colony.
*/

#include <memory>

#include <irrlicht.h>

#include "AppManager.h"
#include "Ants/AntFactory.h"

using namespace AntZerg;
using namespace irr;
using namespace std;


int main()
{
	AppManager *app = new AppManager(800, 800);

	std::unique_ptr<AntFactory> factory(new AntFactory);
	int q = factory->CreateAnt("queen", 0.f, 0.f);
	factory->RunAll();
	
	while (app->device->run())
	{
		if (app->device->isWindowActive())
		{
			app->driver->beginScene(true, true, video::SColor(255, 100, 101, 140));

			app->smgr->drawAll();
			app->guienv->drawAll();

			app->driver->endScene();
		}
		else
		{
			app->device->yield();
		}
	}

	delete app;
	return 0;
}
