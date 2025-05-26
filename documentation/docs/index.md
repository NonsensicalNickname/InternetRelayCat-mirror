# InternetRelayCat

## About

InternetRelayCat (IRCat) is an IRC client written in C++ that provides basic functionality and can look passable.

## Features

### Working

+ Connecting to a server as a user, and sending and receiving messages.
+ Configuring multiple users and servers which may be used.
+ Configuring a list of channels and users which may be selected as targets for messsage.
+ Configuring colour schemes for the UI (technically not fully implemented, but only in that not every UI element has a colour applied to it).
+ *Technically* works in very minimal terminal environments (see: Vim-mode in features).

### Incomplete or planned

#### Vim

This is the highest priority (aside from minor but useful changes), as it is the most important thing to me in terms of actually using this client. This is also important when it comes to using IRCat in basic terminals, as at the moment a mouse is required.

+ Keyboard based navigation.
+ Provide a command line for changing config options at runtime (such as adding channels).
+ Add keybind mappings to the config file.


#### More UI Configurability

+ Date and time format (implementing this would also require showing date and time).
+ Perhaps layout options.
+ Select a different user during runtime.
+ (maaaaybe) Provide a UI for editing and loading colour schemes in the program.

#### Integrations (Only if I actually end up doing all of this and use IRCat regularly)

+ Automatically handle uploading files to hosts and getting a link.
+ Rich-presence
