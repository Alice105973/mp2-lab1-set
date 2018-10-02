// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
int SIZE = sizeof(TELEM);

TBitField::TBitField(int len)
{
	BitLen = len;
	MemLen = ceil((double)BitLen/SIZE);
	pMem = new TELEM[MemLen];
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (MemLen - n/SIZE);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << n;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	int first1 = BitLen;
	while(!GetBit(first1)) first1--;
	return first1;
}

void TBitField::SetBit(const int n) // установить бит
{
  int i = MemLen - int(n/SIZE);
  int k = n % SIZE;
  pMem[i] = pMem[i] | (1 << k);
}

void TBitField::ClrBit(const int n) // очистить бит
{	
  int i = MemLen - int(n/SIZE);
  int k = n % SIZE;
  pMem[i] = pMem[i] & !(1 << k);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n > BitLen) throw "Error: check the number of bit";
	int i = MemLen - int(n/SIZE);
	int k = n % SIZE;
	int m = 1 << k;
	if (pMem[i] && m) return 1;
	else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	TBitField Tnew(bf);
	for (int i = 0; i < bf.MemLen; i++) Tnew.pMem[i] = bf.pMem[i];
	return Tnew;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if(GetLength() != bf.GetLength()) return 0;
	int res = 1;
	for (int i = 0; i < bf.MemLen; i++) res*=(bf.pMem[i]==pMem[i]);
	return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if(GetLength() != bf.GetLength()) return 1;
	int res = 1;
	for (int i = 0; i < bf.MemLen; i++) res*=(bf.pMem[i]==pMem[i]);
	return !res;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (GetLength() != bf.GetLength()) throw "Error: lengths don't match";
	TBitField Tnew(GetLength());
	for(int i = 0; i < Tnew.MemLen; i++) {
		Tnew.pMem[i] = pMem[i] | bf.pMem[i];
	}
	return Tnew;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (GetLength() != bf.GetLength()) throw "Error: lengths don't match";
	TBitField Tnew(GetLength());
	for(int i = 0; i < Tnew.MemLen; i++) {
		Tnew.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return Tnew;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField Tnew(GetLength());
	for(int i = 0; i < Tnew.MemLen; i++) {
		Tnew.pMem[i] = - 1 - pMem[i];
	}
	return Tnew;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	bool a;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> a;
		if (a) bf.SetBit(i);
		else bf.ClrBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = bf.BitLen - 1; i <= 0; i--) {
		ostr << bf.GetBit(i);
	}
	ostr << endl;
	return ostr;
}
