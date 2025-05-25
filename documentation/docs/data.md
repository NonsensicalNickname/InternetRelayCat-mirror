## Data Types

### Char and String

Chars (characters) hold a single character, while strings may hold several. In a number of languages, either directly or under the hood, strings are essentially char arrays, and either have a fixed buffer size or are accessed using a pointer such that they can be easily stored on the heap. Chars may be used to store characters representing keystrokes, i.e. to allow the user to remap key bindings in a program, or to denote modes in function parameters - though this may be more elegantly done using const ints or enum types. Strings may be used to store data such as input from the user, raw data received from another process, a buffer created from reading a file, or for constructing largely repetitive output. In IRCat, C-strings (as in, an explicit array of characters) are used relatively commonly due to the usage of syscalls, which, due to being written in C, take C-strings as parameters. Examples of this include using send and recv over a TCP socket, and a buffer of a fixed size is used when calling recv. Literal chars are used when processing events in the UI, such as ‘\e’ and ‘\n’ for <esc> and <CR> respectively. Strings (as in the variety from the C++ standard library) are used internally for everything that does not require a C-string, such as the contents of the message box, or the different fields in irc-message structs.

### Boolean

Booleans store (in theory) a single bit, and represent true or false (1 or 0). Booleans are generally used as flags for control flow. In IRCat, booleans are used in the configuration options for settings such as vim mode and 24h time (although neither of the features corresponding to such settings are fully implemented and do not yet exist in the main branch).

### Real

Reals store real numbers, or, more accurately, approximations of real numbers. A number of specific data types are ‘subsets’ of reals, such as floating point integers. Reals are useful for storing numbers when a significant degree of precision is required, such as an approximation of pi.

### Single-precision float point

Single-precision float point is a way of storing numbers, and typically has a size of 32 bits. The purpose of single-precision float point is to achieve a balance between range and precision, allowing signed values with a range to the 38th order of magnitude. The double-precision floating point format uses a size of 64 bits, allowing for greater range at the expense of memory usage. Due to the reasonable balance achieved by single and double precision floating point formats, numerous languages use them as the primary data type for precise numbers. 

### Integer

Integers store whole numbers of various sizes. In languages such as Rust, the size must be explicitly specified - i.e. i8, u32 which represent a signed 8 bit integer and an unsigned 32 bit integer respectively. In C++, integers can be specified similarly (i.e. long or short), but if no size is specified, the size is at least 16 bits, and on 32 and 64 bit systems, the size will generally default to 32 bit. Integers are used for all manner of purposes, including storing whole numbers, defining options using const ints (to make the use of options more programmer-friendly), and indexing structures such as arrays. In IRCat, an explicit, unsigned, long integer is used to store the IP address of the server after it is converted from a string to the specified type in network-byte-order. Other uses of integers in IRCat include, but are not limited to, the socket file descriptor and the number of events returned after polling the socket. 

### Date and Time

Data types utilised for storing dates and times are primarily used in database querying languages or similar use cases, due to the otherwise narrow reasons for having a dedicated type for handling date and time. Fun fact: Unix time is a representation of date and time that measures the number of seconds since the 1st of January, 1970 at midnight in UTC. At the time of writing plus however many seconds it took me to check, Unix time was 1748166220. Fun fact the 2nd: Unix time hit 1000000000 seconds on the 9th of September, 2001 at 01:46:40 UTC. 

## Data Structures

### Arrays

Arrays contain a number of elements that may be indexed. The elements in an array must all be of the same size and type, however similar concepts such as lists are often used when this is too restrictive - such as when storing several strings. The cheapest way to alter an array is to pop elements off of the end, or push new elements to the back. Arrays and lists are highly useful to contain a number of related values, especially as iterating through an array is quite efficient. Arrays may also have more than one dimension, and two-dimensional arrays are useful for representing 2D grids or matrices. In IRCat, lists are mostly used instead due to the requirement of holding strings. For example, the messages to be displayed in the UI are stored as a list of structs representing the components of an IRC command and its fields.

### Records

A record is broadly a data structure that contains a number of fields of potentially different types which have corresponding values. Various languages have struct data types which represent records. Records are useful for storing data that is related but of different types. In IRCat, the user and server are represented by structs which hold key information for setting up the connection. Messages are also represented with structs after being parsed. 

### Trees

A tree consists of a root node which branches into child nodes. Each child node can have its own child nodes and so on, however each node must have only one parent. Trees are useful in representing decision-making, or for edits in a text document in order to implement undo and redo.

### Sequential File

 A sequential file contains entries that can be accessed in the same order they were made, and cannot be edited. The primary purpose of sequential files is for logging. 

## Data Dictionary

| Name | Type | Use | Example | Scope | (where applicable) Member values |
| ---- | ---- | --- | ------- | ----- | ------------------------ |
| user | struct User | Holding the nick, name, password, and status of the user for login. The nick is also used later to display above messages sent by the user. | `{"john from accounting", "john", "accounting-sucks-actually", 8}` | Public member of an object within main. | nick (string), real name (string), password (string), status (int) |
| server | struct Server | Holding the name, ip address, and channels for the server that the user is connecting to. | `{"Libera", "94.125.182.252", ["#esolangs", "#linux", "Guest36"]}`| Public member of an object within main | name (string), ip address (string), channels (vector of strings). |
| msg-data | Vector of struct irc-msg | Holding all of the messages that have been parsed in a format that enables easily accessing parts of the message for rendering messages. | `[{"lead.libera.chat", "NOTICE", ["john from accounting", "*** Looking up your hostname..."]}, {"john from accounting", "PRIVMSG", ["Gene", "I don't much like this accounting gig anymore"]}]` | Private member of object within main. | Member values of struct irc-msg: prefix (string, default = ""), command (string), params (vector of strings). |
| sock-fd | int | Stores the file descriptor of the socket that the program is using. | `3` | Private member of object within main. | |
| scroll-percent | float | Stores how far up or down the user has scrolled through the UI. | `0.5` | Main. | |
| channel-entries | vector of strings | Stores the channels dictated by the server settings in the config file. Used for selecting the user or channel to message with a dropdown menu. | `["#esolangs", "#linux", #Guest36]"` | Main. | |
| ip-bytes | long unsigned int | Stores the IP address of the server in a useful format. Converted from: string -> C-string -> long unsigned int -> long unsigned int in network-byte-order. | `1585297148` | Constructor of class Handler. | |
| server-socket | class CSocket | Used for intitialising the socket and connecting to the server, as well as getting the socket file descriptor. | `CSocket(AF_INET, SOCK_STREAM, 0, 6667, 1585297148)` | Constructor of class Handler. | Address (struct sockaddr_in), sock-fd (int), connection (int). |
