#pragma once
#include "../common_data.hpp"

struct InputState
{
	private:
		// Player controls.
		u8 a;
		u8 d;
		u8 w;
		u8 s;
		u8 space;

		// Modifiers that are used to modify the values of the player controls.
		u8 da;
		u8 dd;
		u8 dw;
		u8 ds;
		u8 dspace;

	public:
		void set_a(u8 a);				// Sets the value for the "a" control.
		void set_d(u8 d);				// Sets the value for the "d" control.
		void set_w(u8 w);				// Sets the value for the "w" control.
		void set_s(u8 s);				// Sets the value for the "s" control.
		void set_space(u8 space);		// Sets the value for the "space" control.
		void set_da(u8 da);				// Sets the value for the "da" control which is used as a modifier to the "a" control.
		void set_dd(u8 dd);				// Sets the value for the "dd" control which is used as a modifier to the "d" control.
		void set_dw(u8 dw);				// Sets the value for the "dw" control which is used as a modifier to the "w" control.
		void set_ds(u8 ds);				// Sets the value for the "ds" control which is used as a modifier to the "s" control.
		void set_dspace(u8 dspace);		// Sets the value for the "dspace" control which is used as a modifier to the "space" control.
		u8 get_a();						// Gets the value for the "a" control.
		u8 get_d();						// Gets the value for the "d" control.
		u8 get_w();						// Gets the value for the "w" control.
		u8 get_s();						// Gets the value for the "s" control.
		u8 get_space();					// Gets the value for the "space" control.
		u8 get_da() const;				// Gets the value for the "da" control which is used as a modifier to the "a" control
		u8 get_dd() const;				// Gets the value for the "dd" control which is used as a modifier to the "d" control.
		u8 get_dw() const;				// Gets the value for the "dw" control which is used as a modifier to the "w" control.
		u8 get_ds() const;				// Gets the value for the "ds" control which is used as a modifier to the "s" control.
		u8 get_dspace() const;			// Gets the value for the "dspace" control which is used as a modifier to the "space" control.
};

void InputState::set_a(u8 a)
{
	/*
		Desc: Sets the value for the "a" control.

		Params: u8 (a)
	*/
	this->a = a;
}

void InputState::set_d(u8 d)
{
	/*
		Desc: Sets the value for the "d" control.

		Params: u8 (d)
	*/
	this->d = d;
}


void InputState::set_w(u8 w)
{
	/*
		Desc: Sets the value for the "w" control.

		Params: u8 (w)
	*/
	this->w = w;
}


void InputState::set_s(u8 s)
{
	/*
		Desc: Sets the value for the "s" control.

		Params: u8 (s)
	*/
	this->s = s;
}

void InputState::set_space(u8 space)
{
	/*
		Desc: Sets the value for the "space" control.

		Params: u8 (space)
	*/
	this->space = space;
}

void InputState::set_da(u8 da)
{
	/*
		Desc: Sets the value for the "da" control which is used as a modifier to the "a" control.

		Params: u8 (da)
	*/
	this->da = da;
}

void InputState::set_dd(u8 dd)
{
	/*
		Desc: Sets the value for the "dd" control which is used as a modifier to the "d" control.

		Params: u8 (dd)
	*/
	this->dd = dd;
}

void InputState::set_dw(u8 dw)
{
	/*
		Desc: Sets the value for the "dw" control which is used as a modifier to the "w" control.

		Params: u8 (dw)
	*/
	this->dw = dw;
}


void InputState::set_ds(u8 ds)
{
	/*
		Desc: Sets the value for the "ds" control which is used as a modifier to the "s" control.

		Params: u8 (dd)
	*/
	this->ds = ds;
}

void InputState::set_dspace(u8 dspace)
{
	/*
		Desc: Sets the value for the "dspace" control which is used as a modifier to the "space" control.

		Params: u8 (space)
	*/
	this->dspace = dspace;
}

u8 InputState::get_a()
{
	/*
		Desc: Gets the value for the "a" control.

		Params: None.
	*/
	return this->a;
}

u8 InputState::get_d()
{
	/*
		Desc: Gets the value for the "d" control.

		Params: None.
	*/
	return this->d;
}


u8 InputState::get_w()
{
	/*
		Desc: Gets the value for the "w" control.

		Params: None.
	*/
	return this->w;
}


u8 InputState::get_s()
{
	/*
		Desc: Gets the value for the "s" control.

		Params: None.
	*/
	return this->s;
}

u8 InputState::get_space()
{
	/*
		Desc: Gets the value for the "space" control.

		Params: None.
	*/
	return this->space;
}

u8 InputState::get_da() const
{
	/*
		Desc: Gets the value for the "da" control which is used as a modifier to the "a" control.

		Params: None.
	*/
	return this->da;
}

u8 InputState::get_dd() const
{
	/*
		Desc: Gets the value for the "dd" control which is used as a modifier to the "d" control.

		Params: None.
	*/
	return this->dd;
}

u8 InputState::get_dw() const
{
	/*
		Desc: Gets the value for the "dw" control which is used as a modifier to the "w" control.

		Params: None.
	*/
	return this->dw;
}

u8 InputState::get_ds() const
{
	/*
		Desc: Gets the value for the "ds" control which is used as a modifier to the "s" control.

		Params: None.
	*/
	return this->ds;
}

u8 InputState::get_dspace() const
{
	/*
		Desc: Gets the value for the "dspace" control which is used as a modifier to the "space" control.

		Params: None.
	*/
	return this->dspace;
}
