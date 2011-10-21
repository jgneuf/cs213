int i;
int j;

void foo () {
  switch (i) {
    case 20:  j=10; break;
    case 21:  j=11; break;
    case 22:  j=12; break;
    case 23:  j=13; break;
    default:  j=14; break;
  }
}

void bar () {
  if (i==20)
    j=10;
  else if (i==21) 
    j = 11;
  else if (i==22) 
    j = 12;
  else if (i==23)
    j = 13;
  else
    j = 14;
}

void zot () {
  label jumpTable[6] = { L20, L21, L22, L23 };
  if (i < 20) goto DEFAULT;
  if (i > 23) goto DEFAULT;
  goto jumpTable[i-20];
L20:
  j = 10;
  goto CONT;
L21:
  j = 11;
  goto CONT;
L22:
  j = 12;
  goto CONT;
L23:
  j = 13;
  goto CONT;
DEFAULT:
  j = 14;
  goto CONT;
CONT:
}