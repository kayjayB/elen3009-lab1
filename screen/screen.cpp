#include "screen.h"

// 0 represents the top-left screen element
const string::size_type TOP_LEFT = 0; // the variable TOP_LEFT cannot be changed within the program

// Screen's constructor
Screen::Screen(string::size_type height, string::size_type width, char bkground):
	_height{height},// initialises _height with height
	_width{width},  // initialises _width with width
	_cursor{TOP_LEFT},		// initialises _cursor to the starting position
	_screen(height * width, bkground)	// size of _screen is height * width
										// all positions initialized with
										// character value of bkground
{ /* all the work is done with the member initialization list */ }

void Screen::forward()
{   // advance _cursor one screen element
	++_cursor;

	// wrap around if the _cursor is at the end of the screen
	if ( _cursor == _screen.size()) home();

	return;
}

void Screen::back()
{   // move _cursor backward one screen element
	// check for top of screen; wrap around
	if ( _cursor == TOP_LEFT )
		end();
	else
		--_cursor;

	return;
}

void Screen::up()
{   // move _cursor up one row of screen
	// do not wrap around
	if ( row() == 1 ) // at top?
	
	// Note: This can only be used for a square screen
		_cursor= _cursor+ _width*_height - _width +1;
	else
		_cursor -= _width;

	return;
}

void Screen::down()
{   // move _cursor down one row of screen
	// do not wrap around
	if ( row() == _height ) // at bottom?
	
	// Note: This can only be used for a square screen
		_cursor= _cursor - _width*_height + _width -1 ;
	
	else
		_cursor += _width;

	return;
}

void Screen::move( string::size_type row, string::size_type col )
{   // move _cursor to absolute position
	// valid screen position?
	if ( checkRange( row, col ) )
	{
		// row location
		auto row_loc = (row-1) * _width;
		_cursor = row_loc + col - 1;
	}

	return;
}

// This member function is not a necessity for clients since the functions home(),forward() etc already existed. 
// By creating this function, we are not making it easier for the user to use since the function still needs to be called with 
// Direction:: HOME etc.
void Screen::move(Direction dir)
{
	switch (dir ) {
		case Direction::HOME: home();
		break;
		case Direction::FORWARD: forward();
		break;
		case Direction::BACK: back();
		break;
		case Direction::UP: up();
		break;
		case Direction::DOWN: down();
		break;
		case Direction::END: end();
		break;
	}
	
	return;
}

char Screen::get( string::size_type row, string::size_type col )
{
	// position _cursor
	move( row, col );
	// the other get() member function
	return get();
}

void Screen::set( char ch )
{
	if ( ch == '\0' )
		cerr << "Screen::set warning: " << "null character (ignored).\n";
	else _screen[_cursor] = ch;

	return;
}

void Screen::set( const string& s ) // the formal parameter s receives the address of the actual parameter, 
// but cannot modify the contents of the actual parameter. 
{   // write string beginning at current _cursor position
	auto space = remainingSpace();
	auto len = s.size();
	if ( space < len ) {
		cerr << "Screen::set - Truncating, "
			<< "space available: " << space
			<< ", string length: " << len
			<< endl;
		len = space;
	}

	_screen.replace( _cursor, len, s );
	_cursor += len - 1;

	return;
}

void Screen::clear( char bkground )
{   // reset the cursor and clear the screen
	_cursor = TOP_LEFT;
	// assign the string
	_screen.assign(
		// with size() characters
		_screen.size(),
		// of value bkground
		bkground
		);

	return;
}

void Screen::reSize( string::size_type h, string::size_type w, char bkground )
{   // can only *increase* a screen's size to height h and width w
	// remember the content of the screen
	string local{_screen};
	auto local_pos = TOP_LEFT;

	// replaces the string to which _screen refers
	_screen.assign(      // assign the string
		h * w,           // with h * w characters
		bkground         // of value bkground
		);

	// copy content of old screen into the new one
	for ( string::size_type ix = 0; ix < _height; ++ix )
	{ // for each row
		string::size_type offset = w * ix; // row position
		for ( string::size_type iy = 0; iy < _width; ++iy )
			// for each column, assign the old value
			_screen.at(offset + iy) = local[ local_pos++ ]; // Returns the element at the position specified by offset+iy
	}

	_height = h;
	_width = w;
	// _cursor remains unchanged

	return;
}

void Screen::display() const
{
	for ( string::size_type ix = 0; ix < _height; ++ix ) // The function cannot modify the member variables of the class 
	{ // for each row
		string::size_type offset = _width * ix; // row position
		for ( string::size_type iy = 0; iy < _width; ++iy )
			// for each column, write element
			cout << _screen[ offset + iy ];
		cout << endl;
	}
	return;
}

bool Screen::checkRange( string::size_type row, string::size_type col ) const
{   // validate coordinates
	if (row < 1 || row > _height || col < 1 || col > _width)
	{
		cerr << "Screen coordinates ("<< row << ", " << col << " ) out of bounds.\n";
		return false;
	}
	return true;
}

string::size_type Screen::remainingSpace() const
{   // includes current position
	auto size = _width * _height;
	return(size - _cursor);
}

string::size_type Screen::row() const
{   // return current row
	return (_cursor + _width)/_width;
}

// This function does require access to the internal representation of the Screen class since it needs
// access to the height and the length of the screen. However, alternatively, these could be passed in 
// as inputs to the function by the user, in which case access to the internal representation is not required.
// By making the function a member of the screen class, the error checking is more accurate as there
// is no chance of the user inputting the incorrect screen size.

void Screen::emptySquares(string::size_type row, string::size_type col, string::size_type lengthSides) 
{
	bool check = checkRange(row,col);
	int checkColumn = col+lengthSides;
	int checkRow = row+lengthSides ;
	if (check)
	{	// Fill in the square
		if (checkRow <= _width)
		{
			if (checkColumn <= _height)
			{
				for (int i=row; i<row+lengthSides; i++)
				{
					for (int j=col; j<col+lengthSides; j++)
					{
						move(i,j);
						set('*'); 
					}
				}
	
						// Empty out the centre of the square
				for (int i=row+1; i<row+lengthSides-1; i++)
				{
					for (int j=col+1; j<col+lengthSides-1; j++)
					{
						move(i,j);
						set(' '); 
					}
				}
			}
			else 
			{
			cerr << "Size of square is too long for the height of the screen.\n";
			return;
			}
		}
		else
		{
			cerr << "Size of square is too long for the width of the screen.\n";
		}
	}
	else 
	{
		cerr << "Coordinates out of range.\n";
	}
}

	
