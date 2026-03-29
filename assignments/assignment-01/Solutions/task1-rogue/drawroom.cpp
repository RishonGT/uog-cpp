#include<iostream>
#include<ncurses.h>

#include "drawroom.h"

// Build the top and bottom wall of the room
// tapes in the width of the room as input
void drawtopwall(int x)
{
    x = x + 1; // Include the space of the wall to make the play area correct
    for(int i{0}; i <= x; ++i)
    {
        if(i == 0 || i == x) 
            addch('+'); //corner of the rooms at the beginning and end
        else 
            addch('-'); //the in fill between corners
    }
}

void drawmidroom(int x)
{
    x = x + 1; // Include wall spacing for play area
    for(int i{0}; i <= x; ++i)
    {
        if(i == 0 || i == x)
            addch('|');
        else
            addch('.');
    }
}

void drawfullroom(int roomheight, int roomwidth)
{
    roomheight = roomheight + 1;
    for(int j{0}; j <= roomheight;)
    {
        if(j == 0 || j == roomheight)
            drawtopwall(roomwidth);
        else
            drawmidroom(roomwidth);
        ++j;       //increment so cursor is at the new line
        move(j,0); //move the cursor to newline
    }
}