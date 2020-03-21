char* toCharFunction(String StingToConvert)
{
  static char CharResp[40];
    
  if(StingToConvert){
      StingToConvert.toCharArray(CharResp, 40);
      return CharResp;
  }else{ 
        return nullptr;
      }
}
