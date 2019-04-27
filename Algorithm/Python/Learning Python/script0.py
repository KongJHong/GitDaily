# A first Python script
import sys
import decimal
from decimal import Decimal

decimal.getcontext().prec=4

dec = decimal.Decimal(1)/decimal.Decimal(7)
print(dec)

dec = Decimal(0.1) + Decimal(0.1) + Decimal(0.1) - Decimal(0.3)
dec = round(dec,3)
print(dec)


L1 = [2,3,4]
L2 = L1[:]
print(L1 is L2)