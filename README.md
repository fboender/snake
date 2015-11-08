Snake
=====

Copyright (c) 2000, Ferry Boender <ferry.boender@gmail.com>

Licensed under the General Public License (GPL), see COPYING file provided with
this program.


## What is Snake?

I really thought everybody knew what Snake was, but here's a description
anyway: Snake is a console (or XTerm if you will) game. The goal is to gather
as many points as possible. This goal is reached by eating the apples (or, like
in this version, the dollar signs. (I like money)) If you're snake's head hits
the wall, or tries to eat it's own tail, you die.


## Requirements

Snake requires the following programs to be installed on your system in order
to work:
	
* NCurses dev


## Installation

To compile snake, just go to the dir where you untarred snake-x.x.tar.gz, and
type
	
	make

After it has compiled, a binary should have emerged, appropriately called
'snake'. Run it by typing './snake'.
	
	
The makefile currently does not support the 'make install' command.  Perhaps
it will in the future. Until then, you will have to install snake by hand. 
	
After you've compiled it, you can copy the binary to any directoy you please,
since it requires no libraries what so ever. It is, however, a custem to copy
the binary to either the /usr/games/ (or any similar directory where games are
stored on your system).

You should copy the manual page (snake.1.gz) to the manual directory for games,
which is /usr/share/man/man6 (under Debian anyway)
	

## Usage

    usage: snake [-h|-d|-g|-n]
    
    -h      Show help
    -d #    Set delay in ms (Default 100)
    -g #    Set tail growth per dollar (Default 5)
    -n #    Set number of dollars in screen (Default 5)

	
## Copyright and contact information

Snake is Copyright by Ferry Boender,

    Ferry Boender
    ferry.boender@gmail.com
    http://www.electricmonk.nl
    
licensed under the General Public License (GPL)

Copyright (C), 2000 by Ferry Boender <ferry boender@gmail.com>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
        
This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.
    
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 675 Mass
Ave, Cambridge, MA 02139, USA.
            
For more information, see the COPYING file supplied with this program.

Please report any bugs, request and general comments to the email
address stated above.
