#pragma once
#ifndef _SCREENMANAGER_H_
#define _SCREENMANAGER_H_

#include <Graphics/Screens/Screen.h>
#include <Graphics/Screens/ScreenStack.h>
#include <boost/unordered_map.hpp>
#include <system/Input/InputModule.h>
class Engine;

namespace Graphics
{
	namespace Screens
	{
		class ScreenManager : public ::Input::InputInterface, public AnimatedDraw
		{
		public:
			void AddScreen(screen_ptr screen);
			void AddScreenToStack(screen_ptr screen, const std::string& stackName);
			void AddScreenStack(const std::string& name, const ScreenStack& stack, bool registerIndividualScreens = true);
			ScreenStack& GetScreenStack(const std::string& name);

			bool RegisterStackCreationFunction(const std::string& name);
			ScreenStack CreateStack(const std::string stackname, const std::string creationFunction, bool registerStack = true, bool registerIndividualScreens = true);

			//Input Interface
			virtual bool HandleKeyPressed(const sf::Uint32 time, const ::Input::InputModule* inputModule, ::Input::InputActionResult& action) override;
			virtual bool HandleKeyReleased(const sf::Uint32 time, const ::Input::InputModule* inputModule, ::Input::InputActionResult& action) override;

			//Animate Draw
			virtual void Update(const sf::Uint32 time, const float TimeScale = 1.0) override;
			virtual void Update(const float time, const float TimeScale = 1.0) override;

			virtual void Draw(sf::RenderWindow &window) override;
			virtual void Draw(sf::RenderWindow &window, sf::Shader &shader) override;
		private:
			Engine* engine;
			boost::unordered_map<std::string, Screen*> AllScreensByName;
			boost::unordered_map<int, Screen*> AllScreensByID;

			boost::unordered_map<std::string, ScreenStack> StackByName;
			boost::unordered_map<int, ScreenStack*> StackByID;
			std::vector<ScreenStack*> StacksInDrawOrder;
		};
	}
}

#endif