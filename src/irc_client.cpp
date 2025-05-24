
#include "handler.cpp"

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"        
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"    
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/util/ref.hpp"
#include "ftxui/component/loop.hpp"
#include <string>


int main() {
	using namespace ftxui;

	IRCat::handler handler = IRCat::handler();

	auto screen = ScreenInteractive::Fullscreen();
	std::string message_contents;
	std::string button_send_label = "Send";

	float scroll_percent = 0.0f;
	auto notifColour = color(Color::Default);

	std::vector<std::string> channel_entries = handler.server.channels;
	int selected_channel = 0;

	Component channel_selector = Dropdown(&channel_entries, &selected_channel);
	Component input_message = Input(&message_contents, "send message");
	Component button_send = Button(&button_send_label, screen.ExitLoopClosure(), ButtonOption::Ascii());

	auto message_box = Container::Horizontal ({
			channel_selector,
			input_message,
			button_send,
			});

	auto components = Container::Vertical ({
			message_box,
			});

	auto renderer = Renderer(components, [&] {
			return vbox ({
					//        menu->Render(),
					channel_selector->Render(),
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
				//std::string pr = ":testing PRIVMSG Guest36 :";
				//pr.append(message_contents);
				int bytes_sent = handler.send_user_msg(message_contents, channel_entries[selected_channel]);
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
