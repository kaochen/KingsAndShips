/*
This file is part of KingsAndShips.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*!
 * \file wallet.h
 */

#ifndef WALLET_H
#define WALLET_H

#include <iostream>

/*! \class C_Wallet
 * \brief C_Wallet is a singleton to exchange money between different class.
 * Creating a new tower has a cost and you want to display it on a specific buttons
 */

class C_Wallet
{
	public:
	static	C_Wallet& Instances() {return m_instance;};

  int getBalance(){return m_balance;};
  int getWalletMax (){return m_progress_bar_max;};
  void credit(int value){m_credit += value; refreshBalance();};
  void debit(int value){m_debit += value; refreshBalance();};
  void cliStatus();
  void reset();
  protected:

	private:
	C_Wallet& operator= (const C_Wallet&){return *this;}
	C_Wallet (const C_Wallet&){}

	static C_Wallet m_instance;
	C_Wallet();
	~C_Wallet();

  void refreshBalance(){m_balance = m_credit - m_debit;};

  int m_credit;
  int m_debit;
  int m_balance;
  int m_progress_bar_max;
};
#endif
