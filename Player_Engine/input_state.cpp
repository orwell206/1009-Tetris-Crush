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
		void set_a(u8 a)
		{
			/*
				Desc: Sets the value for the "a" control.
				
				Params: u8 (a)
			*/
			this->a = a;
		}

		void set_d(u8 d)
		{
			/*
				Desc: Sets the value for the "d" control.
				
				Params: u8 (d)
			*/		
			this->d = d;
		}


		void set_w(u8 w)
		{
			/*
				Desc: Sets the value for the "w" control.
				
				Params: u8 (w)
			*/
			this->w = w;
		}


		void set_s(u8 s)
		{
			/*
				Desc: Sets the value for the "s" control.
				
				Params: u8 (s)
			*/
			this->s = s;
		}		

		void set_space(u8 space)
		{
			/*
				Desc: Sets the value for the "space" control.
				
				Params: u8 (space)
			*/
			this->space = space;
		}	

		void set_da(u8 da)
		{
			/*
				Desc: Sets the value for the "da" control which is used as a modifier to the "a" control.
				
				Params: u8 (da)
			*/
			this->da = da;
		}

		void set_dd(u8 dd)
		{
			/*
				Desc: Sets the value for the "dd" control which is used as a modifier to the "d" control.
				
				Params: u8 (dd)
			*/
			this->dd = dd;
		}

		void set_dw(u8 dw)
		{
			/*
				Desc: Sets the value for the "dw" control which is used as a modifier to the "w" control.
				
				Params: u8 (dw)
			*/
			this->dw = dw;
		}


		void set_ds(u8 ds)
		{
			/*
				Desc: Sets the value for the "ds" control which is used as a modifier to the "s" control.
				
				Params: u8 (dd)
			*/
			this->ds = ds;
		}		

		void set_dspace(u8 dspace)
		{
			/*
				Desc: Sets the value for the "dd" control which is used as a modifier to the "space" control.
				
				Params: u8 (space)
			*/
			this->dspace = dspace;
		}	

		u8 get_a()
		{
			/*
				Desc: Gets the value for the "a" control.
				
				Params: None.
			*/
			return this->a;
		}

		u8 get_d()
		{
			/*
				Desc: Gets the value for the "d" control.
				
				Params: None.
			*/
			return this->d;
		}


		u8 get_w()
		{
			/*
				Desc: Gets the value for the "w" control.
				
				Params: None.
			*/
			return this->w;
		}


		u8 get_s()
		{
			/*
				Desc: Gets the value for the "s" control.
				
				Params: None.
			*/
			return this->s;
		}		

		u8 get_space()
		{
			/*
				Desc: Gets the value for the "space" control.
				
				Params: None.
			*/			
			return this->space;
		}	

		u8 get_da() const
		{
			/*
				Desc: Gets the value for the "da" control which is used as a modifier to the "a" control.
				
				Params: None.
			*/			
			return this->da;
		}

		u8 get_dd() const
		{
			/*
				Desc: Gets the value for the "dd" control which is used as a modifier to the "d" control.
				
				Params: None.
			*/						
			return this->dd;
		}

		u8 get_dw() const
		{
			/*
				Desc: Gets the value for the "dw" control which is used as a modifier to the "w" control.
				
				Params: None.
			*/			
			return this->dw;
		}

		u8 get_ds() const
		{
			/*
				Desc: Gets the value for the "ds" control which is used as a modifier to the "s" control.
				
				Params: None.
			*/			
			return this->ds;
		}		

		u8 get_dspace() const
		{
			/*
				Desc: Gets the value for the "dspace" control which is used as a modifier to the "space" control.
				
				Params: None.
			*/		
			return this->dspace;
		}	
};