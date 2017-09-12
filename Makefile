#  Makefile --- replay X11 current selection as series of keystrokes

#  Copyright (C) 2017 by Walter C. Pelissero

#  Author: Walter C. Pelissero <walter@pelissero.de>
#  Project: replay selection

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2, or (at
# your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
RM = rm -f
CFLAGS = -g -Wall
LDFLAGS = -g
LDLIBS =


default: all

all: replay-selection

clean:
	$(RM) *.o

dist: clean
