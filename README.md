# Anotation #
My project's goal was to design and implement basic HTTP server that would be able to server static
html website to user.
It should use multithreading,
cache, be able to serve different domain names and
be configurable over config file.
From this project I expected to learn practical info about HTTP protocol and by implementing HTTP
server deeply understand how this protocol works. Furthermore, I wanted to develop multithreaded
application which would take advantage of using more threads
to improve my skills in that area, since
it takes
takes programming to whole new level because of needed synchronizations, mutexes
etc.
# Introduction #
## Technologies ##
Choosing the right technologies for a project is one of the most important things that influence the
whole project for its lifetime. It has high impact on every part of the project, including time demands
and the outcome. Thus, choosing technologies should not be underestimated and all factors including
developers skills and preferences must be evaluated.
## Programming language ##
I would consider myself to be advanced in several programming languages, including PHP, Java or
C++ with several successfull job projects developed in the past, so I had wide range to choose from.
After some time it came down to Java or C++, which were both suitable for this project. Finally, I
picked C++, since this language is more powerful for this kind of stuff, not to mention that most of
all servers are developed in C++, if not assembler.
## Operating system ##
Other technology aspect is the platform (operating system/enviroment). For a while, I thought of
developing this server for Windows systems, however, I did not wanted to encounter some of the
Window's tricky problems so I decided to go with the open source platform – Linux – namely Ubuntu
12.04.
As multithreading library I used POSIX Threads, which is most likely the best for Linux platform.
## Development enviroment ##
Last but not least was the selection of IDE (
Integrated development environment
).
I could have just
used text editor with syntax highlighting (like Sublime) and compile program over console, but it's not
much convinient especially when it comes to debugging and tuning. Therefore, I used Netbeans editor,
which has
reasonable
debugger and other features.
For advanced debugging of program failures
(segmentation faults) or memory leaks, I used Valgrind.
Features and functions
In the following sections I will describe each server feature separately together with small snippets
of
source code and potential developing problems.
## Multithreading ##
Multithreading allows application to have more than one executing sequences of commands, which
increases performance of application by parallel executing of commands. In addition, for my project, it
allows the server to serv and send content to two or more visitors at the same time.
## Producer – consumer model ##
In my application, I used producer – consumers model. The model is fairly simple and powerful. One
thread (producer) is running in infinite loop only accepting connection comming to our server and only
inserting them to a visitors que. The producer thread does nothing with the connections and does not
important and actually are not needed for the most basic functionality of http server.
They just improve
and make the communication between visitor and server more reliable and effective.

## Responding to visitor ##
In case there
was
error while parsin
g
visitor's request, error page is sen
t
back. That can happen for
example when he wants to connect to a domain, that is not hosted on this server or when he wants a file
that is not present (error code 404).
If the file is on server, first it will try to look it up in its internal
memory cache. If the server has the file in memory already, then there is no need to read it from a disk
(and thus it saves time, because reading from disk is way more slower than reading from memory). If it
was not in the cache, server will read it from a disk and store to cache. Then, all required response
headers are prepared (such as content-lenght, status code, etag) and the response is flushed to client. At
the end, consumer thread just closes
socket
to
visitor.

## Client side cache ##
Client side cache
(browser cache)
reduces data transfers between visitor and client. It
is based
on
an
idea, that if the user had the file already, then there is no reason
to waste traffic and
transfer it again.
Instead, it will just be summoned from visitor's computer memory.
For example,
if
we visit
ed
Union's
website and there was no browser cache, every time we load
some
page the server would have to send
us Union's logo each single time. This does not only wastes traffic
or
resources, but also
significantly
slowers the loading time of the website.
On the other hand, we need to know when the logo changes
and in that case update it, so we won't see the same Union logo for years. For this, my server uses
eTags. Each time server sends file to client, it appends to the response a random hash code generated
for the specific file version. Then, when visitor wants to load a file that is in his cache already, he can
just send the server HEAD request and this hashcode. If the hashcode from user is equal to code in
server's memory, it will just reply "304 – content-not-modified". If it's not the same, server will send
back the whole file.

## Server side cache ##
Unlike client cache, server side cache improves performance of the server only. It stores recently sent
files in RAM memory, so every time user wants some file, server will try to look it up in memory, first.
So if there is some frequently used file, server doesn't need to access hard disk each time and read from
it. Reading from hard disk is significantly slower than reading from RAM memory.
For looking up a
file in memory, my server uses C++ STL structure
map.
For keeping
track of expired files, I put them
to a que. Each time file is used, it's moved to
the
front of que and is assigned timestamp. From time to
time server goes from the end of a que and deletes timeouted files from memory,
to prevent server of
using too much memory space.
## Logging ##
My server supports several logging levels either to console or to logfile. Log levels, as almost
everything else, is configurable over config file. It has up to 8 logging levels ranging from logging only
FATAL errors over NETWORK messages to DEBUG messages.
C
onfiguration
over config file
Everytime server is started, first it reads configuration file in its root directory named
server.conf.
Then,
it parses this file and reads the configuration for server to run. It is possible to configurate almost
everything. For example we can define
listening port,
how many threads should be used, how long files
should stay in cache, all domain names, its aliases and data directories. Last but not least, the logging
levels.

## Conclusion ##
After plenty of hours developing and debugging this server, I have finally managed to complete
the
project
to such a state, that it is able
to server static html website.
I
'm happy that I
have finally
developed application using POSIX threads library, which will definetily come in hand in the future.
Nevertheless to say, that I expected higher performance boost when using more threads. To be more
specific, there is less than significant improvement if using 10 threads instead of 1. In my opinion it is
due to my computer's processor, which is not that good
when it comes to parallel computing.
Also, I
might have used to much mutexes on wrong places so it makes the whole server to slow down
(unwanted passive waiting,
dead-lock
).
As one of the features for improvement in the feature I think of adding dynamic webpages support. The
only problem is, that I don't know yet, how to implement PHP executive enviroment to my code.
I
definitely would be happy If I could have used my server in feature to host my personal website
presentation!