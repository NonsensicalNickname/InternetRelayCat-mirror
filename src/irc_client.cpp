
#include "handler.cpp"

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"        
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"    
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/util/ref.hpp"
#include "ftxui/component/loop.hpp"


int main() {
	using namespace ftxui;

	IRCat::handler::login_info connect_with;
	connect_with.nick = "testing152";
	connect_with.real_name = "a";
	connect_with.password = "";
	IRCat::handler handler = IRCat::handler(&connect_with);

	auto screen = ScreenInteractive::Fullscreen();
	std::string message_contents;
	std::string button_send_label = "Send";

	float scroll_percent = 0.0f;
	auto notifColour = color(Color::Default);

	std::vector<std::string> options = {"one", "two", "three"};
	int selectedOpt = 0;
	MenuOption option;
	option.on_enter = screen.ExitLoopClosure();

	Component menu = Menu(&options, &selectedOpt, option);
	Component input_message = Input(&message_contents, "send message");
	Component button_send = Button(&button_send_label, screen.ExitLoopClosure(), ButtonOption::Ascii());

	auto message_box = Container::Horizontal ({
			input_message,
			button_send,
			});

	auto components = Container::Vertical ({
			message_box,
			menu,
			});

	auto renderer = Renderer(components, [&] {
			return vbox ({
					//        menu->Render(),
					filler(),
					handler.render_messages() | focusPositionRelative(0, scroll_percent) | frame | vscroll_indicator 
					| size(HEIGHT, LESS_THAN, Terminal::Size().dimy * 0.8),
					//separator() | notifColour,
					separator(),// | color(configuration.theme[IRCat::fg]),
					hbox(
							input_message->Render(),
							separator(), 
							button_send->Render()
						) | borderLight | size(HEIGHT, LESS_THAN, Terminal::Size().dimy * 0.1),
					}) | borderHeavy;// | bgcolor(configuration.theme[IRCat::bg]);
			});

	renderer |= CatchEvent([&](Event event) {
			if (event == Event::Character('\n')) {
				handler.send_user_msg(message_contents);
				message_contents = "";
				return true;
			}
			if (event.mouse().button == Mouse::WheelUp && scroll_percent>=0.01) {
			scroll_percent-=0.01;
			return true;
			}
			if (event.mouse().button == Mouse::WheelDown && scroll_percent<=0.99) {
			scroll_percent+=0.01;
			return true;
			}
			if (event == Event::Character('\e')) {
				screen.ExitLoopClosure()();
				return true;
			}
			return false;
	});

	Loop loop(&screen, renderer);

	while (!loop.HasQuitted()) {
		if (handler.poll_msgs() == true) {
			if (scroll_percent < 0.8) {
				notifColour = color(Color::Red);
			}
			screen.Post(Event::Custom);
		}
		if (scroll_percent > 0.8) {
			notifColour = color(Color::Default);
		}
		loop.RunOnce();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
