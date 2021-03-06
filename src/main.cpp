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

#ifdef _DEBUG
#ifdef _WIN32
#include <vld.h>
#endif
#endif

#include <memory>

#include <irrlicht.h>

#include "AppManager.h"
#include "Ants/AntFactory.h"
#include "Renderer/Renderer.h"

using namespace AntZerg;
using namespace irr;
using namespace std;


int main()
{
	std::shared_ptr<AppManager> app(std::make_shared<AppManager>(800, 800));
	std::shared_ptr<LuaManager> luaMngr(std::make_shared<LuaManager>());
	std::unique_ptr<AntFactory> factory((new AntFactory(luaMngr)));
	std::unique_ptr<Renderer> renderer((new Renderer(app, luaMngr)));
	app->device->setWindowCaption(L"AntZerg");

	luaMngr->LoadScript("scripts/actions/behaviorTree.lua");
	luaMngr->LoadScript("scripts/antUtilities.lua");
	luaMngr->LoadScript("scripts/conf/startup.lua");

	auto prevTime = app->device->getTimer()->getTime();
	
#ifdef _DEBUG
	int lastFPS = -1;
#endif
	while(app->device->run())
	{
			auto currentTime = app->device->getTimer()->getTime();
			double dt = currentTime - prevTime;
			prevTime = currentTime;
			dt /= 1000.;
			
			factory->RunAll(dt);
			luaMngr->CallFunction("RenderUpdateAllAnts");

			app->driver->beginScene(true, true, video::SColor(255, 100, 101, 140));

			renderer->DrawAll();

			app->smgr->drawAll();
			app->guienv->drawAll();

			app->driver->endScene();
			
#ifdef _DEBUG
			int fps = app->driver->getFPS();

			if (lastFPS != fps)
			{
				core::stringw str = L"AntZerg ";
				str += "FPS: ";
				str += fps;

				app->device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
#endif
	}

	return 0;
}