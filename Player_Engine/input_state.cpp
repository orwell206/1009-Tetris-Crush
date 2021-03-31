#pragma once
#include "../common_data.hpp"

struct InputState {
	private:
		u8 a;
		u8 d;
		u8 w;
		u8 s;
		u8 space;

		u8 da;
		u8 dd;
		u8 dw;
		u8 ds;
		u8 dspace;

	public:
		void set_a(u8 a){
			this->a = a;
		}

		void set_d(u8 d){
			this->d = d;
		}


		void set_w(u8 w){
			this->w = w;
		}


		void set_s(u8 s){
			this->s = s;
		}		

		void set_space(u8 space){
			this->space = space;
		}	

		void set_da(u8 da){
			this->da = da;
		}

		void set_dd(u8 dd){
			this->dd = dd;
		}


		void set_dw(u8 dw){
			this->dw = dw;
		}


		void set_ds(u8 ds){
			this->ds = ds;
		}		

		void set_dspace(u8 dspace){
			this->dspace = dspace;
		}	

		u8 get_a(){
			return this->a;
		}

		u8 get_d(){
			return this->d;
		}


		u8 get_w(){
			return this->w;
		}


		u8 get_s(){
			return this->s;
		}		

		u8 get_space(){
			return this->space;
		}	

		u8 get_da() const{
			return this->da;
		}

		u8 get_dd() const{
			return this->dd;
		}


		u8 get_dw() const{
			return this->dw;
		}


		u8 get_ds() const{
			return this->ds;
		}		

		u8 get_dspace() const{
			return this->dspace;
		}	
};