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


#include "wallet.h"
#include "message.h"

using namespace std;

C_Wallet::C_Wallet()
{
	reset();
	cout << "Constructor C_Wallet() : done" << endl;
}


void C_Wallet::cliStatus()
{
	C_Message m;
	m.printM("Wallet - balance: " + to_string(m_balance) + ", credit: " + to_string(m_credit) + ", debit: " + to_string(m_debit) + "\n");
}

void C_Wallet::reset()
{
	m_credit = 0;
	m_debit = 0;
	m_balance = 0;
	m_progress_bar_max =1000;
}
