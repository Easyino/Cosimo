int exponential(int base, int exponent){
  
  for(i = 1; exponent > 0; i *= base, exponent--){}
  return i;
}
