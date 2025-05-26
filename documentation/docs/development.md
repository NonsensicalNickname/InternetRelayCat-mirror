# Requirements and Development

## Requirements

The use case of the project is for communication for group coordination in the context of programming and computer science education.
Thus, a lightweight solution prioritising core functionality and maintaining usability on old or low-end systems is ideal. Furthermore, support for a degree of customisation including colour schemes is important in that getting a number of people to use the same thing is hard since people have different opinions. To that end, using a reliable and established protocol as opposed to implementing a new one is ideal, as this means that users may use other clients (even if it makes me sad). 
Hence, the project should consist of a terminal based Internet Relay Chat (IRC) client, as this meets all of the aforementioned needs. (The largest drawback to this approach is that, as a result of the passing of time, many students are not familiar with IRC).

## Specification

| Decision | Justification |
| -------- | ------------- |
| The project shall be written in C++ | The task says "high level, object oriented". This means no Rust, and I don't enjoy using Python for purposes beyond scripting. I should also learn and practice more C++. In regards to the requirements, C++ is conducive to relatively fast and light programs, and the language has widespread support. Furthermore, C++ allows direct use of syscalls written in C, which, while present in other languages, is very convenient. This helps notably with socket programming without a library. |
| The project shall only work on Linux. | <ul><li>Socket programming in Windows is notably different.</li><li>When it comes to platforms with less differences (i.e. openBSD or other BSD-based OSes), I do not think many highschool students are daily-driving openBSD.</li><li>While I did have a functioning and sensibly set up VM for Windows at one point, that point was a fair while ago, and troubleshooting issues with virtual machines is not the most joyous experience.</li><li>Whilst this may be at odds with the goal of compatability, within the context of programming and education, I think it is actually a net positive. This is to say, make the children learn Linux, it will be good for their souls.</li></ul>
| The UI library shall be FTXUI. | The requirements for a UI library (since writing my own is beyond the scope of this project) are simple: C++, terminal-based, and with good documentation. FTXUI has all of these attributes to varying extents (the gaps in the documentation are filled by the fact that the header files for the different modules are very easy to read). |
| The config file shall be written in toml. | It is very readable but is still actually used, which is a good balance for software which students who may be new to programming might use. |
| The parsing of the config file shall be done using tomlplusplus. | It works, and I already knew it existed. |


## Outline

IRCat is written in C++, and relies on FTXUI and tomlplusplus for handling the terminal UI rendering and parsing toml files resepectively.
The dependencies are managed using CMake. 
The project is structured into an abstract sockets class with implementations for connecting and binding sockets, a config class which parses the config file and stores the resultant data, and a handler class which is responsible for the 'backend' - interacting with the server and using the config and socket classes. The UI is currently handled in the main program file, which instances a handler object to send and receive messages.  
For more information, see [the source code](https://codeberg.org/ceridwen15/InternetRelayCat).

## Debugging

The use of debugging tools has been mostly limited to:

+ Using perror and checking the return values at each step in the sockets class. For example, if the program encounters an error because the user is not connected to the Internet, the following will be output: `an error occured while establishing the connection: Network is unreachable`. This significantly reduced the shenanigans endured, and remains applicable when using the program (though further processing errors and still displaying the UI would be a good idea).
+ Using dummy message data when writing the code to handle rendering messages. Whilst this approach is more focused on convenience than debugging, it does mean that strange behaviour as a result of receiving bad data is not misconstrued as a bug in the UI, reducing the time spent identifying the bug.
+ std::cout. Using various print statements to narrow down the cause of a bug is, while sometimes inelegant, usually quite effective.
+ The GNU C compiler (but for C++). Admittedly this stops being useful as soon as `Segmentation fault`, and has made me appreciate rustc even more. This (alongside the LSP and linting, which I only setup towards the very of the project, as with full syntax highlighting and completions) is useful for identifying trivial bugs such as syntax errors, mismatched types, and poor practice - such as the handling of C-strings, which is mentioned in warnings.
+ Finally, while it is not specifically a debugging tool, reasonably encapsulated code made it quite easy to fix issues in writing the code for IRCat. For example, if there is an issue with sending a message but the initial connection is established successfully, the handler.cpp file can reasonably be assumed to be responsible. This also limits needing to create *numerous* 'test.cpp' programs and wasting time trying to isolate code.

## Errors

The main sources of errors were pointer shenanigans with tomlplusplus, and lambda shenanigans with FTXUI (largely due to initial confusion from the shorthand that the library uses). The former errrors were generally able to be resolved either from the compiler output, or by consulting the documentation. 
The latter sources of errors were more infuriating, primarily due to the documentation not always existing, however the numerous examples and convenient module headers were mostly sufficient to resolve things in a timely manner. The aforementioned errors were primarily syntax errors, however at times they simply led to undefined or unexpected behaviour.   

The most common logic errors were caused by how the messages were rendered.  
Initially, rendering messages blocked text input, and messages would only be received when the user moved the cursor. The first issue was solved by piping parsed messages into a function to return components that could be rendered within a container. The second issue was resolved alongside a logic error in which messages would be received multiple times. 
The solution was to poll the socket for events once each frame, and if the call to poll returned a number greater than 0, receive new messages, parse them, add them to the container, and update the rendering loop.   

Runtime errors mostly arose when connecting to the server, and were easily resolved thanks to checking the return values of each syscall and using perror to output error information.


